#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <istream>
#include <thread>
#include <mutex>

#include <barrier>

using namespace std;

/*
g++ bubbleSort.cpp -o bubbleSort.out -lpthread -std=c++2a
*/

void BucketSort(int *input, int arraySize);

int main(int argc, char *argv[])
{

    string line;
    getline(cin, line);
    cout << "[Input]:" << line << "\n";
    int inputCount = std::atoi(line.c_str());
    auto inputs = new int[inputCount]{};
    getline(cin, line);
    cout << "[Input]:" << line << "\n";

    std::stringstream ss{line};
    std::string buf;
    int counter = 0;
    while ((counter < inputCount) && std::getline(ss, buf, ' '))
    {
        inputs[counter] = atoi(buf.c_str());
        counter++;
    }

    std::vector<int> outPutList = {};
    BucketSort(inputs, counter);
    // std::sort(outPutList.begin(), outPutList.end() );

    cout << inputs[0] << " " << inputs[counter - 1] << "\n";
    for (int i = 0; i < counter; i++)
    {
        cout << inputs[i];
    }
    cout << "\n";

    delete[] inputs;
    return 0;
}

void BucketSortThread1(
    int *input, int arraySize,
    int myThreadNo, int ThreadNum,
    int *minMax, int *min, int *max)
{
    int range = arraySize / ThreadNum;
    int start = myThreadNo * range;
    int end = ((myThreadNo + 1) * range - 1) < arraySize ? (myThreadNo + 1) * range - 1 : arraySize - 1;
    // cout << "start : " << start << "  end:" << end << "  range" << range << "\n";
    int max_l, min_l;
    max_l = min_l = input[start];
    for (int index = start + 1; index <= end; index++)
    {
        // cout << "Max: " << max_l << " min:" << min_l << " Now:" << input[index] << "\n";
        if (input[index] > max_l)
        {
            max_l = input[index];
        }
        if (input[index] < min_l)
        {
            min_l = input[index];
        }
    }
    minMax[myThreadNo] = min_l;
    minMax[myThreadNo + ThreadNum] = max_l;
}

void BucketSortThread2(
    int *input, int arraySize,
    int myThreadNo, int ThreadNum,
    char **arrPtr, int *min, int *max, std::mutex *mtx)
{
    char *counter = *arrPtr;
    int range = arraySize / ThreadNum;
    int start = myThreadNo * range;
    int end = ((myThreadNo + 1) * range) - 1;
    cout << "ThreadNum: " << myThreadNo << " Start: " << start << "  end: " << end << " Range:" << range << " min:" << *min << " max:" << *max
         << "\n";

    for (int i = start; i <= end; i++)
    {
        int temp = input[i];
        std::lock_guard<std::mutex> lock(mtx[(temp - *min) % ThreadNum]);
        counter[temp - *min] += 1;
    }
}
void BucketSort(int *input, int arraySize)
{
    int max = INT_MIN;
    int min = INT_MAX;

    int NWORKERS = (arraySize > 100) ? 10 : 1;
    std::barrier<> sync{NWORKERS + 1};
    int nThread = NWORKERS;
    auto minMax = new int[nThread * 2]{};

    char *arrPtr = nullptr;
    std::mutex mtx_[NWORKERS]; // n thread分ロックする

    for (int i = 0; i < nThread; i++)
    {
        std::thread(
            [&, i]()
            {
                BucketSortThread1(input, arraySize, i, nThread,
                                  minMax, &min, &max);
                // 親スレッドへの最大/最小値取得完了通知
                sync.arrive_and_wait();
                // 親スレッドの準備待ち
                sync.arrive_and_wait();
                BucketSortThread2(input, arraySize, i, nThread,
                                  &arrPtr, &min, &max, mtx_);
                // 親スレッドへ完了通知
                sync.arrive_and_wait();
            })
            .detach();
    }

    // 子スレッド完了待ち
    sync.arrive_and_wait();
    for (int i = 0; i < nThread; i++)
    {
        if (minMax[i] < min)
            min = minMax[i];
        if (minMax[i + nThread] > max)
            max = minMax[i + nThread];
    }

    auto counter = new char[max - min + 1]{0};
    arrPtr = counter;
    // 子スレッドへ完了通知
    sync.arrive_and_wait();

    // 子プロセス完了待ち
    sync.arrive_and_wait();
    int writeIndex = 0;
    for (int i = 0; i < (max - min + 1); i++)
    {
        for (int j = 0; j < counter[i]; j++)
        {
            input[writeIndex] = i + min;
            writeIndex += 1;
        }
    }
}
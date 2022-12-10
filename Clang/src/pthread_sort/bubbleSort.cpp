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

void BubbleSort(int *input, int arraySize);

int main(int argc, char *argv[])
{

    string line;
    getline(cin, line);
    int inputCount = std::atoi(line.c_str());
    auto inputs = new int[inputCount]{};
    getline(cin, line);

    std::stringstream ss{line};
    std::string buf;
    int counter = 0;
    while (std::getline(ss, buf, ' '))
    {
        inputs[counter] = atoi(buf.c_str());
        counter++;
    }

    std::vector<int> outPutList = {};
    BubbleSort(inputs, counter);
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

void BubbleSort(int *input, int arraySize)
{
    std::mutex mtx_; // index
    // std::lock_guard<std::mutex> lock(mtx_);

    bool isChanged = false;
    int count = 0;
    int threadSize = arraySize / 2;

    // 0スタート or 1スタート
    int offset = 0;

    int counter = 0;
    auto getCount = [&counter, &mtx_]() -> int
    {
        std::lock_guard<std::mutex> lock(mtx_);
        int res = counter;
        counter += 2;
        return res;
    };

    int NWORKERS = 30;
    std::barrier<> sync{NWORKERS + 1};

    // n thread 起動
    for (int i = 0; i < NWORKERS; i++)
    {
        std::thread(
            [&, arraySize, getCount, i]()
            {
                while (count != 2)
                {
                    int index = getCount();
                    if (index + 1 < arraySize)
                    {
                        if (input[index] > input[index + 1])
                        {
                            auto temp = input[index + 1];
                            input[index + 1] = input[index];
                            input[index] = temp;
                            isChanged = true;
                        }
                    }
                    else
                    {
                        // 親スレッドに完了通知
                        sync.arrive_and_wait();
                        // 親スレッドの準備待ち
                        sync.arrive_and_wait();
                    }
                }
            })
            .detach();
    }

    // 親スレッド
    while (count != 2)
    {
        // 全スレッド完了待ち
        sync.arrive_and_wait();

        if (isChanged)
            count = 0;
        else
            count++;

        isChanged = false;
        offset = (offset + 1) % 2;
        counter = offset;

        // 子スレッドへの完了待ち
        sync.arrive_and_wait();
    }
}
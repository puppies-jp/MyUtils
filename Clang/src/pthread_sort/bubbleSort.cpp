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

void BubbleSortThread(int *input, int arraySize,
                      std::barrier<> *sync,
                      int myThreadNo,
                      int nThreads,
                      bool *isChanged,
                      int *count)
{
    int evenOdd = 0;
    int index = evenOdd + (myThreadNo * 2);
    bool thlocalChanged = false;

    while (*count != 2)
    {
        if (index + 1 < arraySize)
        {
            if (input[index] > input[index + 1])
            {
                auto temp = input[index + 1];
                input[index + 1] = input[index];
                input[index] = temp;
                *isChanged = true;
                thlocalChanged = true;
            }
            index = index + (nThreads * 2);
        }
        else
        {
            // 親スレッドに完了通知
            cout << "ThreadNo: " << myThreadNo << " isChanged: " << thlocalChanged << "\n";
            sync->arrive_and_wait();
            // 配列0番目スタート or 1番目スタートを切り替える
            evenOdd = (evenOdd + 1) % 2;
            index = evenOdd + (myThreadNo * 2);
            thlocalChanged = false;
            //  親スレッドの準備待ち
            sync->arrive_and_wait();
        }
    }
}

void BubbleSort(int *input, int arraySize)
{

    bool isChanged = false;
    int count = 0;

    int NWORKERS = 30;
    std::barrier<> sync{NWORKERS + 1};

    // n thread 起動
    for (int i = 0; i < NWORKERS; i++)
    {
        std::thread(
            [&, i]()
            {
                BubbleSortThread(
                    input, arraySize, &sync, i,
                    NWORKERS, &isChanged, &count);
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
        cout << "pairent flag clear  and count " << count << "\n";

        // 子スレッドへの完了待ち
        sync.arrive_and_wait();
    }
}
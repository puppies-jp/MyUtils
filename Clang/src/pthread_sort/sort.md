# Sortについて

また、ソートする上でのマルチスレッド戦略を書いていく

- [Bubble Sort](#Bubble)
- [ヒープソート(そのうち)]
- [マージソート(そのうち)]
- [シェルソート(そのうち)]
- [クイックソート(そのうち)]
- [基数ソート(そのうち)]
- [バケットソート](#Bucket)

## <a name=Bubble>Bubble Sort</a>

バブルソートは、隣り合う要素の大小を比較しながら整列させるソートアルゴリズム。 アルゴリズムが単純で実装も容易である一方、最悪時間計算量は O(n²) と遅いため、一般にはマージソートやヒープソートなど、より最悪時間計算量の小さな方法が利用される。

- マルチスレッドで実装するとこんな感じ

```cpp
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
            //cout << "ThreadNo: " << myThreadNo << " isChanged: " << thlocalChanged << "\n";
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
        //cout << "pairent flag clear  and count " << count << "\n";

        // 子スレッドへの完了待ち
        sync.arrive_and_wait();
    }
}
```

## <a name=Bucket>バケットソートについて</a>

手順1. 配列の要素の値をインデックスとして直接バケツへ移動
手順2. バケツの手前から順番に配列に戻してソート完了

手順は簡単だが、`少数が使えない`、`最大値が大きすぎるとそれだけ配列を確保しないといけない`
といった制約が多い

[非同期実装サンプル](https://github.com/puppies-jp/MyUtils/blob/gh-pages/Clang/src/pthread_sort/bucketSort.cpp)

## <a name=Quick>クイックソート</a>

ピボットと呼ぶ要素を軸に分割を繰り返して整列を行うアルゴリズム

```cpp
```

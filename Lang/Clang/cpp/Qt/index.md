# Qtについて

Qt は C++ で作成できる、クロスプラットフォームのGUIアプリケーションフレームワークです。

## 主な特徴

1. クロスプラットフォーム対応：Windows、macOS、Linux など複数のOS上で同じコードで動作
2. 豊富なウィジェット：ボタン、テキストボックス、テーブルなど多数のUI部品を提供
3. シグナル・スロット機構：イベント駆動型のプログラミングが可能
4. UIデザイナー：Qt Creator により WYSIWYG でUI設計できる
5. 国際化対応：多言語対応が容易

## シグナルとスロットとは

Qt のシグナルとスロットは、オブジェクト間の通信を実現するためのメカニズムです。シグナルはイベントが発生したことを通知し、スロットはその通知を受け取って処理を行います。

connect 関数を使って、シグナルとスロットを結びつけることができます。

```cpp
// 例: シグナルとスロットの接続
// sender: シグナルを発信するオブジェクト
// receiver: スロットを持つオブジェクト
// signalName: 発信されるシグナルの名前
// slotName: 呼び出されるスロットの名前
QObject::connect(sender, SIGNAL(signalName()), receiver, SLOT(slotName()));

// 例: QPushButton の clicked シグナルをスロットに接続
QPushButton* button = new QPushButton("Click Me", this);
QObject::connect(button, &QPushButton::clicked, this, [this](){ 
    // スロットの処理内容
    ui->label->setText("Clicked");
});
```

## ラベル変更について

C#のmethodinvokerのような機能はQtにはないが、以下ようにスレッドを作成して、UIの更新を行うことができます。

```cpp
/*
これらのヘッダは必要に応じてインクルードしてください。
#include <thread>
#include <chrono>
#include <ctime>
*/

//NOTE: キャプチャを参照にして、フラグとuiの参照を渡すのを忘れないこと
std::thread x([&](){
while(flg){    

    // 現在時刻を取得し、time_t に変換
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // 時間を文字列に変換して、UIのlineEditに表示
    auto str = std::ctime(&now_time);
    ui->lineEdit->setText(str);
    
    auto abs_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
    std::this_thread::sleep_until(abs_time);
    }    
});
// スレッドを切り離して、UIの更新を続けること
x.detach();
```

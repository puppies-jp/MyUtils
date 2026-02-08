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

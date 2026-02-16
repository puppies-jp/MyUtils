#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVulkanInstance>
#include <thread>
#include <chrono>
#include <ctime>

auto flg = true;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // レイアウトを無効化（絶対座標モード）
    this->setLayout(nullptr);
    //btn = new QPushButton("クリックしてください",this);
    //QPushButton btn2 = QPushButton("クリックしてください",this);
    //button.setc
    //this->AddWiget(button);
    // シグナルとスロットを接続（UIデザイナーで作成したボタンにひもづけ）
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    //connect(btn, &QPushButton::clicked, this, &MainWindow::onLabelChanged);

    std::thread x([&](){

        while(flg){
            auto now = std::chrono::system_clock::now();
            // time_t に変換
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);
            auto str = std::ctime(&now_time);
            ui->lineEdit->setText(str);
            auto abs_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
            std::this_thread::sleep_until(abs_time);

        }
    });
    x.detach();
}

MainWindow::~MainWindow()
{
    delete ui;
    flg =false;
}
static int count = 0;
void MainWindow::onButtonClicked() {
    // クリック時の処理（例: メッセージボックスを表示）
    QString plainText = ui->textEdit->toPlainText();
    count++;
    std::string tex = ( "count:：" + std::to_string(count));
    ui->pushButton->setText(tex.c_str());

    //QMessageBox::information(this, "情報1", "ボタンがクリックされました！");
    std::cout << "ボタンがクリックされました！:" << plainText.toStdString() << std::endl;
}

void MainWindow::onLabelChanged() {
    QString plainText = ui->textEdit->toPlainText();
    QMessageBox::information(this, "情報2", plainText);
    ui->textEdit->setPlainText("a");
    QPoint pt = btn->pos();
    pt.setX(pt.x() + 50);
    btn->move(pt); // X=50, Y=50,90,130...

}

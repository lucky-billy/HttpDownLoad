#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QString str = downLoad.get("http://34679y496m.oicp.vip/download/version.json");
//    qDebug() << str;

    downLoad.startDownLoad();
}

MainWindow::~MainWindow()
{
    delete ui;
}


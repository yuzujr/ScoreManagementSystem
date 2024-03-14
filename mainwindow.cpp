#pragma once
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mypushbutton.h"
#include "studentwindow.h"
#include "adminwindow.h"
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QTimer>

//TODO:内存优化，多线程，加密，测试入口，学生，管理员，账密文件，成绩文件
//TODOOOOO:把按钮动画设为默认

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //设置界面标题
    QLabel *title = new QLabel;
    title->setParent(this);
    QFont font;
    font.setFamily("方正粗黑宋简体");
    font.setPointSize(30);
    title->setFont(font);
    title->setText(QString("吉林大学成绩管理系统"));
    title->setGeometry(QRect(205, 150, 500, 100)); //设置位置和大小

    //按钮字体
    font.setFamily("华文中宋");
    font.setPointSize(20);

    //学生入口按钮
    myPushButton *studentEnterBtn = new myPushButton("://btn.png", "");
    studentEnterBtn->setParent(this);
    studentEnterBtn->setText("学生入口");
    studentEnterBtn->resize(QSize(200, 50));
    studentEnterBtn->move((this->width() - studentEnterBtn->width()) / 2, 300);

    //管理员入口按钮
    myPushButton *adminEnterBtn = new myPushButton("://btn.png", "");
    adminEnterBtn->setParent(this);
    adminEnterBtn->setText("管理员入口");
    adminEnterBtn->resize(QSize(200, 50));
    adminEnterBtn->move((this->width() - adminEnterBtn->width()) / 2, 400);

    //退出按钮
    myPushButton *exitBtn = new myPushButton("://btn.png", "");
    exitBtn->setParent(this);
    exitBtn->setText("退出系统");
    exitBtn->resize(QSize(200, 50));
    exitBtn->move((this->width() - exitBtn->width()) / 2, 500);
    connect(exitBtn, &myPushButton::clicked, [ = ]() {
        exit(0);
    });

    //学生窗口
    connect(studentEnterBtn, &myPushButton::clicked, [ = ]() {
        StudentWindow *studentWindow = new StudentWindow(this);
        QTimer::singleShot(300, this, [ = ]() {
            studentWindow->setGeometry(this->geometry());
            studentWindow->show();
            this->hide();
        });
        connect(studentWindow, &StudentWindow::backToMenu, [ = ]() {
            this->setGeometry(studentWindow->geometry());
            delete studentWindow;
            this->show();
        });//返回主菜单
        connect(studentWindow, &StudentWindow::closed, [ = ]() {
            exit(0);
        });//从学生界面直接退出
    });



    //管理员窗口
    connect(adminEnterBtn, &myPushButton::clicked, [ = ]()  {
        AdminWindow *adminWindow = new AdminWindow(this);
        QTimer::singleShot(300, this, [ = ]() {
            adminWindow->setGeometry(this->geometry());
            adminWindow->show();
            this->hide();
        });
        connect(adminWindow, &AdminWindow::backToMenu, [ = ]() {
            this->setGeometry(adminWindow->geometry());
            delete adminWindow;
            this->show();
        });//返回主界面
        connect(adminWindow, &AdminWindow::closed, [ = ]() {
            exit(0);
        });//从管理员界面直接退出
    });

}
void MainWindow::paintEvent(QPaintEvent *event) {
    //设置窗口图标
    this->setWindowIcon(QIcon("://JLUicon.png"));

    //设置窗口标题
    this->setWindowTitle("吉林大学成绩管理系统");

    //创建主界面
    QPainter painter(this);
    QPixmap pix;
    pix.load("://background.jpg");
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), pix);
}

MainWindow::~MainWindow() {
    delete ui;
}


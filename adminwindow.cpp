#pragma once
#include "adminwindow.h"
#include "loginwidget.h"
#include "mypushbutton.h"
#include "changepasswddialog.h"
#include "scoretable.h"
#include "student.h"
#include "student-grademanager.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>

extern int isAdmin;
extern int lineNumber;

//1.增加学生
//2.查找学生
//3.成绩总览(统计，按GPA、姓名首字母排名）(快速搜索ctrl+f）
//......

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow{parent} {
    //加载背景图
    this->backgroundPixmap.load("://background.jpg");
    //设置窗口图标
    this->setWindowIcon(QIcon("://JLUicon.png"));
    //设置窗口标题
    this->setWindowTitle("职工界面");
    //窗口大小
    this->setFixedSize(800, 600);
    //登录窗口
    LogInWidget *logIn = new LogInWidget(this);
    this->setCentralWidget(logIn);
    //取消登录
    connect(logIn, &LogInWidget::loginCanceled, [ = ]() {
        this->hide();
        delete logIn;
        emit this->backToMenu();
    });
    //调整登录框内容
    logIn->changeClient(QString("职工登录"));
    logIn->changeAccount(QString("职工账号:"));
    //登录成功
    connect(logIn, &LogInWidget::loginSucceed, [ = ]() {
        logIn->hide();
        this->show();
        delete logIn;
        //登录后操作
        //修改密码
        myPushButton *changePasswdBtn = new myPushButton(":/btn.png");
        changePasswdBtn->setParent(this);
        changePasswdBtn->setText("修改密码");
        changePasswdBtn->resize(200, 50);
        changePasswdBtn->move((this->width() - changePasswdBtn->width()) / 2, 400);
        connect(changePasswdBtn, &myPushButton::clicked, [ = ]() {
            QTimer::singleShot(200, [ = ]() {
                changePasswdDialog *cpd = new changePasswdDialog(this);
                cpd->setModal(1);//设为模态对话框
                this->setCentralWidget(cpd);
            });
        });
        changePasswdBtn->show();
        //3.退出登录
        myPushButton *exitBtn = new myPushButton(":/btn.png");
        exitBtn->setParent(this);
        exitBtn->setText("退出登录");
        exitBtn->resize(200, 50);
        exitBtn->move((this->width() - exitBtn->width()) / 2, 500);
        connect(exitBtn, &myPushButton::clicked, [ = ]() {
            QTimer::singleShot(200, [ = ]() {
                this->hide();
                this->parentWidget()->show();
            });
        });
        exitBtn->show();
    });
}

void AdminWindow::paintEvent(QPaintEvent *event) {
    //创建主界面
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), this->backgroundPixmap);
}

void AdminWindow::closeEvent(QCloseEvent *event) {
    emit closed();
    AdminWindow::closeEvent(event);
}

AdminWindow::~AdminWindow() {

}

#pragma once
#include "studentwindow.h"
#include "mypushbutton.h"
#include "loginwidget.h"
#include "changepasswddialog.h"
#include <QTimer>
#include <QMessageBox>

StudentWindow::StudentWindow(QWidget *parent)
    : QMainWindow(parent) {
    //设置窗口图标
    this->setWindowIcon(QIcon("://JLUicon.png"));
    //设置窗口标题
    this->setWindowTitle("学生界面");
    //登录窗口
    LogInWidget *logIn = new LogInWidget();
    this->setCentralWidget(logIn);
    //关闭后销毁
    logIn->setAttribute(Qt::WA_DeleteOnClose);
    //取消登录
    connect(logIn, &LogInWidget::loginCanceled, [ = ]() {
        QTimer::singleShot(200, this, [ = ]() {
            this->hide();
            emit this->backToMenu();
        });
    });
    //登录成功
    connect(logIn, &LogInWidget::loginSucceed, [ = ]() {
        logIn->hide();
        //登录后操作
        delete logIn;
        //1.查看成绩,排名

        //2.修改密码
        myPushButton *changePasswdBtn = new myPushButton(":/btn.png");
        changePasswdBtn->setParent(this);
        changePasswdBtn->setText("修改密码");
        changePasswdBtn->resize(200, 50);
        changePasswdBtn->move((this->width() - changePasswdBtn->width()) / 2, 400);
        connect(changePasswdBtn, &myPushButton::clicked, [ = ]() {
            changePasswdDialog *cpd = new changePasswdDialog(this);
            cpd->setModal(1);//设为模态对话框
            this->setCentralWidget(cpd);

        });
        changePasswdBtn->show();
        //3.退出登录
        myPushButton *exitBtn = new myPushButton(":/btn.png");
        exitBtn->setParent(this);
        exitBtn->setText("退出登录");
        exitBtn->resize(200, 50);
        exitBtn->move((this->width() - exitBtn->width()) / 2, 500);
        connect(exitBtn, &myPushButton::clicked, [ = ]() {
            this->hide();
            this->parentWidget()->show();
        });
        exitBtn->show();
    });
}

void StudentWindow::paintEvent(QPaintEvent *event) {
    //创建主界面
    QPainter painter(this);
    QPixmap pix;
    pix.load("://background.jpg");
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), pix);
}

void StudentWindow::closeEvent(QCloseEvent *event) {
    emit closed();
    StudentWindow::closeEvent(event);
}

StudentWindow::~StudentWindow() {
}

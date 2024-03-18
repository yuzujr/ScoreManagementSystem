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
    logIn->move(368, 102);
    logIn->show();
    //关闭后销毁
    logIn->setAttribute(Qt::WA_DeleteOnClose);
    //取消登录
    connect(logIn, &LogInWidget::loginCanceled, [ = ]() {
        delete logIn;
        this->hide();
        emit this->backToMenu();
    });
    //登录成功
    connect(logIn, &LogInWidget::loginSucceed, [ = ]() {
        logIn->hide();
        this->show();
        //登录后操作
        delete logIn;
        //1.查看成绩,排名按钮
        myPushButton *scoreInquireBtn = new myPushButton(":/btn.png");
        scoreInquireBtn->setParent(this);
        scoreInquireBtn->setText("查看成绩");
        scoreInquireBtn->resize(200, 50);
        scoreInquireBtn->move((this->width() - scoreInquireBtn->width()) / 2, 300);
        scoreInquireBtn->show();
        //2.修改密码
        myPushButton *changePasswdBtn = new myPushButton(":/btn.png");
        changePasswdBtn->setParent(this);
        changePasswdBtn->setText("修改密码");
        changePasswdBtn->resize(200, 50);
        changePasswdBtn->move((this->width() - changePasswdBtn->width()) / 2, 400);
        connect(changePasswdBtn, &myPushButton::clicked, [ = ]() {
            QTimer::singleShot(100, [ = ]() {
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
            this->hide();
            this->parentWidget()->show();
        });
        exitBtn->show();
        //查看成绩
        /*返回按钮*/
        myPushButton *backBtn = new myPushButton(":/btn.png");
        backBtn->setParent(this);
        backBtn->setText("返回菜单");
        backBtn->resize(200, 50);
        backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());
        connect(scoreInquireBtn, &myPushButton::clicked, [ = ]() {
            QTimer::singleShot(100, [ = ]() {
                changePasswdBtn->hide();
                scoreInquireBtn->hide();
                exitBtn->hide();
                backBtn->show();
            });
        });
        connect(backBtn, &myPushButton::clicked, [ = ]() {
            changePasswdBtn->show();
            scoreInquireBtn->show();
            exitBtn->show();
            backBtn->hide();
            //隐藏上面显示的信息
        });
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

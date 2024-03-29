#pragma once
#include "adminwindow.h"
#include "loginwidget.h"
#include "mypushbutton.h"
#include "changepasswddialog.h"
#include "scoretable.h"
#include "student.h"
#include "student-grademanager.h"
#include "addstudentdialog.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>

extern int isAdmin;
extern int lineNumber;

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
        //1.增加学生
        myPushButton *addStudentBtn = new myPushButton(":/btn.png");
        addStudentBtn->setParent(this);
        addStudentBtn->setText("增加学生");
        addStudentBtn->resize(200, 50);
        addStudentBtn->move((this->width() - addStudentBtn->width()) / 2, 100);
        addStudentBtn->show();
        connect(addStudentBtn, &myPushButton::clicked, [ = ]() {
            AddStudentDialog *addStudentDialog = new AddStudentDialog(this);
            addStudentDialog->setGeometry(this->geometry());
            addStudentDialog->setModal(1);
            addStudentDialog->show();
        });
        //2.查找学生
        myPushButton *searchStudentBtn = new myPushButton(":/btn.png");
        searchStudentBtn->setParent(this);
        searchStudentBtn->setText("查找学生");
        searchStudentBtn->resize(200, 50);
        searchStudentBtn->move((this->width() - searchStudentBtn->width()) / 2, 200);
        searchStudentBtn->show();
        //3.成绩总览(统计，按GPA、姓名首字母排名）(快速搜索ctrl+f）
        myPushButton *scoreOverviewBtn = new myPushButton(":/btn.png");
        scoreOverviewBtn->setParent(this);
        scoreOverviewBtn->setText("成绩总览");
        scoreOverviewBtn->resize(200, 50);
        scoreOverviewBtn->move((this->width() - scoreOverviewBtn->width()) / 2, 300);
        scoreOverviewBtn->show();
        //4.修改密码
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
        //5.退出登录
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

#pragma once
#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QMessageBox>
#include <QTimer>
#include "logInCheck.h"
#include "logInCheck.c"
#include "globalVar.h"

LogInWidget::LogInWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogInWidget) {
    ui->setupUi(this);
    this->move(356, 60);
    //只允许输入拉丁符号
    ui->lineEdit->setInputMethodHints(Qt::ImhLatinOnly);
    ui->lineEdit_2->setInputMethodHints(Qt::ImhLatinOnly);
    //切换顺序
    QWidget::setTabOrder(ui->lineEdit, ui->lineEdit_2);
    QWidget::setTabOrder(ui->lineEdit_2, ui->pushButton);
    QWidget::setTabOrder(ui->pushButton, ui->pushButton_2);
    //确认登录
    //1.通过按钮
    connect(ui->pushButton, &myPushButton::clicked, [ = ]() {
        QTimer::singleShot(300, this, [ = ]() {
            //账号密码不为空
            if (!ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty()) {
                //验证密码
                int res = 999;
                if (ui->label_3->text() == QString("学 生 登 录")) {
                    res = stuLogInCheck(ui->lineEdit->text().toUtf8().data(), ui->lineEdit_2->text().toUtf8().data());
                    if (checkResult(res)) {//登录成功
                        emit loginSucceed();
                    }
                } else {
                    res = adminLogInCheck(ui->lineEdit->text().toUtf8().data(), ui->lineEdit_2->text().toUtf8().data());
                    if (checkResult(res)) {//登录成功
                        emit loginSucceed();
                    }
                }
            } else {
                QMessageBox::warning(this, "警告", "账号或密码不能为空！");
            }
        });
    });
    //2.通过Enter键
    connect(this, &LogInWidget::loginCommitted, [ = ]() {
        QTimer::singleShot(300, this, [ = ]() {
            //账号密码不为空
            if (!ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty()) {
                //验证密码
                int res = 999;
                if (ui->label_3->text() == QString("学 生 登 录")) {
                    res = stuLogInCheck(ui->lineEdit->text().toUtf8().data(), ui->lineEdit_2->text().toUtf8().data());
                    if (checkResult(res)) {//登录成功
                        emit loginSucceed();
                    }
                } else {
                    res = adminLogInCheck(ui->lineEdit->text().toUtf8().data(), ui->lineEdit_2->text().toUtf8().data());
                    if (checkResult(res)) {//登录成功
                        emit loginSucceed();
                    }
                }
            } else {
                QMessageBox::warning(this, "警告", "账号或密码不能为空！");
            }
        });
    });

    //取消登录，返回主界面
    connect(ui->pushButton_2, &myPushButton::released, [ = ]() {
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();//清空输入框
        emit loginCanceled();
    });
}

void LogInWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // 如果按下了 Enter 键，执行登录操作
        emit loginCommitted();
    } else {
        QWidget::keyPressEvent(event);
    }
}

//改变界面文字
void LogInWidget::changeClient(QString client) {
    ui->label_3->setText(client);
}
void LogInWidget::changeAccount(QString account) {
    ui->label->setText(account);
}

//检查登录结果
bool LogInWidget::checkResult(int res) {
    switch (res) {
        case -1:
            QMessageBox::warning(this, "错误", "文件打开失败!");
            return false;
            break;
        case -2:
            QMessageBox::information(this, "提示", "账号或密码错误!");
            return false;
        case 1:
            QMessageBox::information(this, "提示", "登录成功!");
            return true;
        default:
            return false;
            break;
    }
}

LogInWidget::~LogInWidget() {
    delete ui;
}

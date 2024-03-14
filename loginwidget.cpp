#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "logInCheck.h"
#include "logInCheck.c"
#include "globalVar.h"
#include <QMessageBox>
#include <QTimer>

LogInWidget::LogInWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogInWidget) {
    ui->setupUi(this);
    this->move(35, 60);

    //确认登录
    connect(ui->pushButton, &myPushButton::clicked, [ = ]() {
        QTimer::singleShot(300, this, [ = ]() {
            //账号密码不为空
            if (!ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty()) {
                //验证密码
                int res = 999;
                if (ui->label_3->text() == QString("学 生 登 录")) {
                    res = stuLogInCheck(ui->lineEdit->text().toUtf8().data(), ui->lineEdit_2->text().toUtf8().data());
                    if (checkResult(res)) {//登录成功
                        qDebug() << lineNumber;
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
        QTimer::singleShot(300, this, [ = ]() {
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();//清空输入框
            emit loginCanceled();
        });
    });

    //测试入口
    connect(ui->pushButton_3, &QPushButton::clicked, [ = ]() {
        emit loginSucceed();
    });
}

//改变界面
void LogInWidget::changeClient(QString client) {
    ui->label_3->setText(client);
}
void LogInWidget::changeAccount(QString account) {
    ui->label->setText(account);
}


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

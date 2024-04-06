#pragma once
#include "changepasswddialog.h"
#include "ui_changepasswddialog.h"
#include "dataProcess.c"
#include "globalVar.h"

changePasswdDialog::changePasswdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changePasswdDialog) {
    ui->setupUi(this);
    char logInNum[100], logInPasswd[100];
    if (isStudent) {
        fscanf(stuFileptr, "%s %s", logInNum, logInPasswd);
        moveToLineStart(stuFileptr);//获取信息后归位
    } else {
        fscanf(workerFileptr, "%s %s", logInNum, logInPasswd);
        moveToLineStart(workerFileptr);
    }
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, [ = ]() {
        if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()) {
            QMessageBox::warning(this, "警告", "内容不得为空！");
        } //内容为空
        else if (strcmp(logInPasswd, ui->lineEdit->text().toUtf8().data()) != 0) {
            QMessageBox::warning(this, "错误", "原密码错误！");
        }//原密码错误
        else if (ui->lineEdit->text() == ui->lineEdit_2->text()) {
            QMessageBox::information(this, "提示", "新密码和旧密码相同");
        }//旧密码和原密码相同
        else {
            int ret = changePasswdTo(ui->lineEdit_2->text().toUtf8().data());
            if (ret == 0) {
                QMessageBox::information(this, "提示", "密码修改成功！");
            } else if (ret == -1) {
                QMessageBox::information(this, "提示", "新密码不得包含汉字或字符！请重新修改");
            } else if (ret == -2) {
                QMessageBox::information(this, "提示", "文件修改失败！");
            } else if (ret == -3) {
                QMessageBox::information(this, "提示", "密码过短，不得少于8位！");
            } else if (ret == -4) {
                QMessageBox::information(this, "提示", "密码过长，不得多于20位！");
            }
        }//修改密码
    });
}

changePasswdDialog::~changePasswdDialog() {
    delete ui;
}

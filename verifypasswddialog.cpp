#include "verifypasswddialog.h"
#include "ui_verifypasswddialog.h"
#include "globalVar.h"
#include "dataProcess.h"
#include <QMessageBox>

VerifyPasswdDialog::VerifyPasswdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerifyPasswdDialog) {
    ui->setupUi(this);
    this->setWindowTitle("验证密码");
    char fpasswd[100];
    fscanf(workerFileptr, "%s", fpasswd);
    fscanf(workerFileptr, "%s", fpasswd);
    moveToLineStart(workerFileptr);
    connect(ui->pushButton, &QPushButton::clicked, [ = ]() {
        if (!ui->lineEdit->text().isEmpty()) {
            if (strcmp(ui->lineEdit->text().toUtf8().data(), fpasswd) == 0) {
                emit verifySuccessful();
            }//登录成功
            else {
                QMessageBox::information(this, "提示", "密码错误！");
            }
        }
    });
    connect(ui->pushButton_2, &QPushButton::clicked, [ = ]() {
        this->close();
        deleteLater();
    });
}

VerifyPasswdDialog::~VerifyPasswdDialog() {
    delete ui;
}

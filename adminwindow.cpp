#pragma once
#include "adminwindow.h"
#include "loginwidget.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow{parent} {
    //设置窗口图标
    this->setWindowIcon(QIcon("://JLUicon.png"));
    //设置窗口标题
    this->setWindowTitle("管理员界面");
    LogInWidget *logIn = new LogInWidget(this);
    //关闭后销毁
    logIn->setAttribute(Qt::WA_DeleteOnClose);
    //取消登录
    connect(logIn, &LogInWidget::loginCanceled, [ = ]() {
        QTimer::singleShot(200, this, [ = ]() {
            this->hide();
            emit this->backToMenu();
        });
    });
    //调整登录框内容
    logIn->changeClient(QString("管理员登录"));
    logIn->changeAccount(QString("       管理员账号:"));

}

void AdminWindow::paintEvent(QPaintEvent *event) {
    //创建主界面
    QPainter painter(this);
    QPixmap pix;
    pix.load("://background.jpg");
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), pix);
}

void AdminWindow::closeEvent(QCloseEvent *event) {
    emit closed();
    AdminWindow::closeEvent(event);
}

AdminWindow::~AdminWindow() {

}

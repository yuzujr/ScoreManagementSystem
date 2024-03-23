#pragma once
#include "mypushbutton.h"
#include <QPixmap>
#include <QPropertyAnimation>
#include <QRect>
#include <QTimer>

//默认构造
myPushButton::myPushButton(QWidget *parent) {
    //图片
    this->normalImgPath = ":/btn.png";
    this->pressImgPath = NULL;
    QPixmap pixmap;
    bool ret = pixmap.load(this->normalImgPath);
    if (!ret) {
        qDebug("%s图片加载失败", normalImgPath);
    }
    this->setFixedSize(pixmap.width(), pixmap.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pixmap);
    this->setIconSize(QSize(pixmap.width(), pixmap.height()));
}

myPushButton::myPushButton(QString normalImg, QString pressImg) {
    //图片
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;
    QPixmap pixmap;
    bool ret = pixmap.load(this->normalImgPath);
    if (!ret) {
        qDebug("%s图片加载失败", normalImgPath);
    }
    this->setFixedSize(pixmap.width(), pixmap.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pixmap);
    this->setIconSize(QSize(pixmap.width(), pixmap.height()));

}

//向下跳跃
void myPushButton::zoomDown() {
    //创建动画对象
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "geometry");
    //设置动画时间
    animation1->setDuration(20);
    //创建起始位置
    animation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    //创建结束位置
    animation1->setEndValue(QRect(this->x(), this->y() + 1, this->width(), this->height()));
    //设置缓冲曲线
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation1->start();
}

//向上跳跃
void myPushButton::zoomUp() {
    //创建动画对象
    QPropertyAnimation *animation2 = new QPropertyAnimation(this, "geometry");
    //设置动画时间
    animation2->setDuration(20);
    //创建起始位置
    animation2->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    //创建结束位置
    animation2->setEndValue(QRect(this->x(), this->y() - 1, this->width(), this->height()));
    //设置缓冲曲线
    animation2->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation2->start();
}

//鼠标按下事件
void myPushButton::mousePressEvent(QMouseEvent *e) {
    if (pressImgPath != "") {
        QPixmap pixmap;
        bool ret = pixmap.load(pressImgPath);
        if (!ret) {
            qDebug("%s图片加载失败", pressImgPath);
        }

        this->setFixedSize(pixmap.width(), pixmap.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(), pixmap.height()));
    }
    this->zoomDown();
    return QPushButton::mousePressEvent(e);
}
//鼠标释放事件
void myPushButton::mouseReleaseEvent(QMouseEvent *e) {
    if (normalImgPath != "") {
        QPixmap pixmap;
        bool ret = pixmap.load(normalImgPath);
        if (!ret) {
            qDebug("%s图片加载失败", normalImgPath);
        }

        this->setFixedSize(pixmap.width(), pixmap.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pixmap);
        this->setIconSize(QSize(pixmap.width(), pixmap.height()));
    }
    QTimer::singleShot(150, this, [ = ]() {
        this->zoomUp();
    });
    return QPushButton::mouseReleaseEvent(e);
}

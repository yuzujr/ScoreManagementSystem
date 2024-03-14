#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QString>
#include <QPainter>
#include <QFont>

class myPushButton : public QPushButton {
public:
    explicit myPushButton(QWidget *parent = NULL);
    explicit myPushButton(QPushButton *parent = nullptr);
    //构造函数
    myPushButton(QString normalImg, QString pressImg = "");
    //向下跳跃
    void zoomDown();
    //向上跳跃
    void zoomUp();
    //鼠标事件
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    QString btnText;
    QString normalImgPath;//正常图片路径
    QString pressImgPath;//按下图片路径
    void setText(const QString &text) {
        btnText = text;
    }
    void paintEvent(QPaintEvent *event) override {
        QFont font;
        font.setFamily("华文中宋");
        font.setPointSize(20);
        this->setFont(font);
        QPushButton::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, btnText); // 在按钮中心绘制文本
    }
signals:
};

#endif // MYPUSHBUTTON_H

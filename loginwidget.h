#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
    class LogInWidget;
}

class LogInWidget : public QWidget {
    Q_OBJECT

public:
    explicit LogInWidget(QWidget *parent = nullptr);
    void changeClient(QString);
    void changeAccount(QString);
    bool checkResult(int);
    ~LogInWidget();

private:
    Ui::LogInWidget *ui;
signals:
    loginCanceled();
    loginSucceed();
};

#endif // LOGINWIDGET_H

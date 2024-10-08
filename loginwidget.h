#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QKeyEvent>

namespace Ui {
    class LogInWidget;
}

class LogInWidget : public QWidget {
    Q_OBJECT

  public:
    explicit LogInWidget(QWidget *parent = nullptr);
    //改变UI
    void changeClient(QString);
    void changeAccount(QString);
    //处理查找结果
    bool checkResult(int);
    ~LogInWidget();

  protected:
    void keyPressEvent(QKeyEvent *event);

  private:
    Ui::LogInWidget *ui;

  signals:
    loginCanceled();
    loginCommitted();
    loginSucceed();
};

#endif // LOGINWIDGET_H

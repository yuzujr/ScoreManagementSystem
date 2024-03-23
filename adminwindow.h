#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

class AdminWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit AdminWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    ~AdminWindow();
  private:
    QPixmap backgroundPixmap;
  signals:
    backToMenu();
    closed();
};

#endif // ADMINWINDOW_H

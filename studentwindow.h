#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QMainWindow>

class StudentWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit StudentWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    ~StudentWindow();
  signals:
    backToMenu();
    closed();
};

#endif // STUDENTWINDOW_H

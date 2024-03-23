#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QMainWindow>
#include "student.h"
#include "scoretable.h"

class StudentWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit StudentWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    ~StudentWindow();
  private:
    QPixmap backgroundPixmap;
  signals:
    backToMenu();
    closed();
};

#endif // STUDENTWINDOW_H

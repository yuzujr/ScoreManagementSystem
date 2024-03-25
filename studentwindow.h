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

  protected:

  private:
    QPixmap backgroundPixmap;
    QPixmap topMarginPixmap;

  signals:
    backToMenu();
    closed();
};

#endif // STUDENTWINDOW_H

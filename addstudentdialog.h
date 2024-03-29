#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QPainter>

namespace Ui {
    class AddStudentDialog;
}

class AddStudentDialog : public QDialog {
    Q_OBJECT

  public:
    explicit AddStudentDialog(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    ~AddStudentDialog();

  private:
    Ui::AddStudentDialog *ui;
    QPixmap backgroundPixmap;
};

#endif // ADDSTUDENTDIALOG_H

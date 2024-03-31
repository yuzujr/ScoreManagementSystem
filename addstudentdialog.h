#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QPainter>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include "student.h"
#include "student-grademanager.h"

namespace Ui {
    class AddStudentDialog;
}

class AddStudentDialog : public QDialog {
    Q_OBJECT

  public:
    explicit AddStudentDialog(QWidget *parent = nullptr, stu_list *studentList = nullptr);
    void paintEvent(QPaintEvent *event);
    Student newStudent;
    ~AddStudentDialog();

  private:
    Ui::AddStudentDialog *ui;
    QPixmap backgroundPixmap;
  signals:
    addStudentSuccessful();
    addStudentCanceled();
};

#endif // ADDSTUDENTDIALOG_H

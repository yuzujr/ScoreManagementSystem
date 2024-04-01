#ifndef FINDSTUDENTDIALOG_H
#define FINDSTUDENTDIALOG_H

#include <QDialog>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include "student.h"
#include "student-grademanager.h"
#include "addstudentdialog.h"

namespace Ui {
    class FindStudentDialog;
}

class FindStudentDialog : public QDialog {
    Q_OBJECT

  public:
    explicit FindStudentDialog(QWidget *parent = nullptr, stu_list *studentList = nullptr);
    ~FindStudentDialog();

  private:
    Ui::FindStudentDialog *ui;
};

#endif // FINDSTUDENTDIALOG_H

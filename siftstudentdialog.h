#ifndef SIFTSTUDENTDIALOG_H
#define SIFTSTUDENTDIALOG_H

#include "globalVar.h"
#include "student.h"
#include "student-grademanager.h"
#include <QDialog>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <Worker.h>

namespace Ui {
    class SiftStudentDialog;
}

class SiftStudentDialog : public QDialog {
    Q_OBJECT

  public:
    explicit SiftStudentDialog(QWidget *parent = nullptr, stu_list *head = nullptr, char course[] = nullptr, char college[] = nullptr);
    ~SiftStudentDialog();
    //查找结果链表
    stu_list *findListHead;

  private:
    Ui::SiftStudentDialog *ui;

  signals:
    siftSuccessful();
    findVoid();
};

#endif // SIFTSTUDENTDIALOG_H

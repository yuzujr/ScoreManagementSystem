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
    explicit AddStudentDialog(QWidget *parent = nullptr, stu_list *studentList = nullptr, bool isResult = 0, char course[] = nullptr);
    void paintEvent(QPaintEvent *event);
    void setStudent(int ret);//设置查找到的学生
    void editStudent(stu_list *studentList);
    Student newStudent;
    int findCnt = 0; //找到个数
    int studentIndex = 0; //显示的学生序号
    bool isStudentSelected = false; //是否已选择学生
    char m_course[100];//课程名
    int m_index;//课程索引
    void Calculate(Student *a);
    ~AddStudentDialog();

  private:
    Ui::AddStudentDialog *ui;
    QPixmap backgroundPixmap;

  signals:
    addStudentSuccessful();
    addStudentCanceled();
};

#endif // ADDSTUDENTDIALOG_H

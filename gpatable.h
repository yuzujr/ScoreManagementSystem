#ifndef GPATABLE_H
#define GPATABLE_H

#include <QWidget>
#include <QLabel>
#include <QTableWidgetItem>
#include "student.h"
#include "student-grademanager.h"
#include "siftstudentdialog.h"
#include "Worker.h"

namespace Ui {
    class GPATable;
}

class GPATable : public QWidget {
    Q_OBJECT

  public:
    explicit GPATable(QWidget *parent = nullptr, stu_list *head = nullptr, char course[] = nullptr, char college[] = nullptr);
    // 清空表格内容
    void clearTable();
    //打印表格内容
    void printTable(stu_list *head, int page);
    //判断学生是否有对应课程
    int isStudentValid(stu_list *student);//返回课程索引，若没有该课程返回-1
    //成绩升序
    stu_list *sort_ascend_by_grade(stu_list *head, int gradeIndex);
    //成绩降序
    stu_list *sort_descend_by_grade(stu_list *head, int gradeIndex);
    //平均分
    double averageGrade(stu_list *head);
    //更新统计数据
    void updateStatistics();
    //及格率
    double passRate(stu_list *head);
    //GPA平均数
    double averageGPA(stu_list *head);
    //GPA中位数
    double medianGPA();
    ~GPATable();

    //属性
    QPixmap NonePixmap;
    QLabel pixmapLabel;
    QLabel *info;
    stu_list *head;
    stu_list *findListHead = nullptr;
    bool isSifted = false;
    char m_course[100];
    char m_college[100];
    int m_index;//课程索引

  private:
    Ui::GPATable *ui;

  signals:
    Sifted();
    unSift();
};

#endif // GPATABLE_H

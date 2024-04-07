#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include "mypushbutton.h"
#include "gpatable.h"
#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>

class AdminWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit AdminWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    void paginationConnect(GPATable *table, QLabel *info, myPushButton *left10Btn, myPushButton *left1Btn, myPushButton *right1Btn, myPushButton *right10Btn,
        QLineEdit *pageNumberEdit, stu_list *listHead);
    // 更新界面函数
    void updateUI(int currentPage, int pageNumber, QLineEdit *pageNumberEdit, QLabel *info);
    //判断学生是否有对应课程
    int isStudentValid(stu_list *student);
    char m_course[100];
    //导入成绩
    int TxTtoList(stu_list *phead, int MKnum, char fileName[]);
    //计算GPA
    void Calculate(Student *a);
    ~AdminWindow();
  private:
    QPixmap backgroundPixmap;
  signals:
    backToMenu();
    closed();
};

#endif // ADMINWINDOW_H

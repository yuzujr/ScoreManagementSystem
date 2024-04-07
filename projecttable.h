#ifndef PROJECTTABLE_H
#define PROJECTTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>
#include "student.h"

namespace Ui {
    class ProjectTable;
}

class ProjectTable : public QWidget {
    Q_OBJECT

  public:
    explicit ProjectTable(QWidget *parent = nullptr, Student *stu = nullptr);
    // 清空表格内容
    void clearTable();
    //打印论文
    void printPaper(Student *stu, int page);
    //打印奖项
    void printAward(Student *stu, int page);
    //属性判断
    bool isPaper() {
        return m_isPaper;
    }
    bool isAward() {
        return !m_isPaper;
    }
    //翻页功能
    void pageTurning(Student *stu, int page);
    ~ProjectTable();
    QPixmap NonePixmap;
    QLabel pixmapLabel;

  private:
    Ui::ProjectTable *ui;
    bool m_isPaper;

  signals:
    paperClicked();
    awardClicked();
};

#endif // PROJECTTABLE_H

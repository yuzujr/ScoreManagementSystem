#ifndef PROJECTTABLE_H
#define PROJECTTABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
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
    void printPaper(Student *stu, int page);
    void printAward(Student *stu, int page);
    bool isPaper() {
        return m_isPaper;
    }
    bool isAward() {
        return !m_isPaper;
    }
    void pageTurning(Student *stu, int page);
    ~ProjectTable();

  private:
    Ui::ProjectTable *ui;
    bool m_isPaper;
  signals:
    paperClicked();
    awardClicked();
};

#endif // PROJECTTABLE_H

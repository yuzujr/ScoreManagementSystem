#ifndef SCORETABLE_H
#define SCORETABLE_H

#include <QWidget>
#include <QTableWidgetItem>
#include "student.h"

namespace Ui {
    class ScoreTable;
}

class ScoreTable : public QWidget {
    Q_OBJECT

  public:
    explicit ScoreTable(QWidget *parent = nullptr);
    // 清空表格内容
    void clearTable();
    void printTable(Student *stu, int page);
    ~ScoreTable();

  private:
    Ui::ScoreTable *ui;
};

#endif // SCORETABLE_H
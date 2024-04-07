#ifndef SCORETABLE_H
#define SCORETABLE_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QLabel>
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
    //打印表格内容
    void printTable(Student *stu, int page);
    ~ScoreTable();
    QPixmap NonePixmap;
    QLabel pixmapLabel;
  private:
    Ui::ScoreTable *ui;

};

#endif // SCORETABLE_H

#ifndef GPATABLE_H
#define GPATABLE_H

#include <QWidget>
#include <QLabel>
#include <QTableWidgetItem>
#include "student.h"
#include "student-grademanager.h"

namespace Ui {
    class GPATable;
}

class GPATable : public QWidget {
    Q_OBJECT

  public:
    explicit GPATable(QWidget *parent = nullptr);
    // 清空表格内容
    void clearTable();
    void printTable(stu_list *head, int page);
    QPixmap NonePixmap;
    QLabel pixmapLabel;
    ~GPATable();

  private:
    Ui::GPATable *ui;
};

#endif // GPATABLE_H

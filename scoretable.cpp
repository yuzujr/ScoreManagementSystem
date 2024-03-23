#include "scoretable.h"
#include "ui_scoretable.h"

extern Student *student;

char courses[100][100] = {"666", "555", "444", "666", "666", "666", "666", "666", "666", "666", "666", "666"};
float credits[100] = {
    4.0, // 高等数学的绩点
    3.5, // 线性代数的绩点
    3.7, // 概率论与数理统计的绩点
    3.2, // 大学物理的绩点
    3.6, // 化学基础的绩点
    3.8, // 生物学导论的绩点
    3.4, // 地球科学的绩点
    3.9,
    2.5,
    3.5,
    4.0,
};

ScoreTable::ScoreTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreTable) {
    ui->setupUi(this);
}

// 添加数据到表格
void ScoreTable::printTable(Student *stu, int page) {
    int startIndex = (page - 1) * 10;
    for (int row = 0; row < 10; ++row) {
        //添加单个课程信息
        int courseIndex = stu->stu_course_grade[startIndex][0];
        qDebug() << QString::fromUtf8(courses[courseIndex]);
        QTableWidgetItem *courseName = new QTableWidgetItem(QString::fromUtf8(courses[courseIndex]));
        this->ui->scoreTable->setItem(row, 0, courseName);
        ++startIndex;
    }
}

ScoreTable::~ScoreTable() {
    delete ui;
}

#include "scoretable.h"
#include "ui_scoretable.h"

extern Student *student;

char courses[100][100] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
float credits[100] = {
    4.0,
    3.5,
    3.7,
    3.2,
    3.6,
    3.8,
    3.4,
    3.9,
    2.5,
    3.5,
    4.0,
    2.0,
    4.0,
    3.5,
    3.7,
    3.2,
    3.6,
    3.8,
    3.4,
    3.9,
    2.5,
    3.5,
    4.0,
    2.0,
};

ScoreTable::ScoreTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScoreTable) {
    ui->setupUi(this);
    // 设置表格的边框线
    this->ui->scoreTable->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent; }");
    this->ui->scoreTable->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent; }");
    this->ui->scoreTable->setCornerButtonEnabled(false);
}

// 清空表格内容
void ScoreTable::clearTable() {
    for (int row = 0; row < this->ui->scoreTable->rowCount(); ++row) {
        for (int col = 0; col < this->ui->scoreTable->columnCount(); ++col) {
            QTableWidgetItem *item = this->ui->scoreTable->item(row, col);
            if (item) {
                delete item;
                this->ui->scoreTable->setItem(row, col, nullptr);
            }
        }
    }
}

// 添加数据到表格
void ScoreTable::printTable(Student *stu, int page) {
    clearTable();//打印前先清空表格
    int startIndex = (page - 1) * 10;//开始读取的索引值
    int courseNumber = stu->stu_course_num;
    int pageNumber = (courseNumber + 10 - 1) / 10;
    //循环地添加单行数据
    for (int row = 0; page < pageNumber && row < 10 || page == pageNumber
        && row < stu->stu_course_num - 10 * (page - 1)/*防止打印多余信息*/; ++row, ++startIndex) {
        //添加课程名
        int courseIndex = stu->stu_course_grade[startIndex][0];
        QTableWidgetItem *courseNameItem = new QTableWidgetItem(QString::fromUtf8(courses[courseIndex]));
        this->ui->scoreTable->setItem(row, 0, courseNameItem);
        //添加总成绩
        QTableWidgetItem *scoreItem = new QTableWidgetItem(QString::number(stu->stu_course_grade[startIndex][1]));
        this->ui->scoreTable->setItem(row, 1, scoreItem);
        //添加学分
        QTableWidgetItem *creditItem = new QTableWidgetItem(QString::number(credits[courseIndex]));
        this->ui->scoreTable->setItem(row, 2, creditItem);
        //添加绩点
        QTableWidgetItem *gradePointItem = new QTableWidgetItem(QString::number(stu->stu_course_grade[startIndex][2]));
        this->ui->scoreTable->setItem(row, 3, gradePointItem);
        //添加课序号
        QTableWidgetItem *courseNumberItem = new QTableWidgetItem(QString::number(stu->stu_course_grade[startIndex][0]));
        this->ui->scoreTable->setItem(row, 4, courseNumberItem);
        //添加是否及格
        QTableWidgetItem *isPassedItem = new QTableWidgetItem(stu->stu_course_grade[startIndex][1] >= 60 ? QString("是") : QString("否"));
        this->ui->scoreTable->setItem(row, 5, isPassedItem);
    }
}

ScoreTable::~ScoreTable() {
    delete ui;
}

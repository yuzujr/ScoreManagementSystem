#include "gpatable.h"
#include "ui_gpatable.h"

extern int CntNum(stu_list *head);

GPATable::GPATable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GPATable) {
    ui->setupUi(this);
    // 设置表格的边框线
    this->ui->scoreTable->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent; }");
    this->ui->scoreTable->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent; }");
    this->ui->comboBox_2->setCurrentIndex(-1);
}

// 清空表格内容
void GPATable::clearTable() {
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
void GPATable::printTable(stu_list *head, int page) {
    clearTable();//打印前先清空表格
    int startIndex = (page - 1) * 10;//开始读取的索引值
    int studentNumber = CntNum(head);
    if (head->next == NULL) { //链表为空
        this->ui->scoreTable->setRowCount(0);
        this->ui->scoreTable->setColumnCount(0);
        pixmapLabel.setParent(this->ui->scoreTable);
        pixmapLabel.setPixmap(this->NonePixmap);
        pixmapLabel.resize(this->NonePixmap.size());
        pixmapLabel.move((this->width() - NonePixmap.width()) / 2, 100);
        pixmapLabel.setStyleSheet("border: none;");
        pixmapLabel.show();
        QLabel *info = new QLabel("这里还什么都没有哦", this->ui->scoreTable);
        info->move((this->width() - info->width()) / 2 - 33, 300);
        info->setStyleSheet("border: none; font: 14pt 宋体;");
        info->show();
        return;
    }//成绩表为空
    stu_list *startStudent = head->next;
    for (int i = 0; i < startIndex; i++, startStudent = startStudent->next); //定位此页开始学生
    int pageNumber = (studentNumber + 10 - 1) / 10;
    //循环地添加单行数据
    for (int row = 0; page < pageNumber && row < 10 || page == pageNumber
        && row < studentNumber - 10 * (page - 1)/*防止打印多余信息*/; ++row, startStudent = startStudent->next) {
        //学生姓名
        QTableWidgetItem *studentNameItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_name));
        this->ui->scoreTable->setItem(row, 0, studentNameItem);
        //学号
        QTableWidgetItem *numberItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_number));
        this->ui->scoreTable->setItem(row, 1, numberItem);
        //学院
        QTableWidgetItem *collegeItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_college));
        this->ui->scoreTable->setItem(row, 2, collegeItem);
        //专业
        QTableWidgetItem *majorItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_major));
        this->ui->scoreTable->setItem(row, 3, majorItem);
        //班级
        QTableWidgetItem *classNumItem = new QTableWidgetItem(QString::number(startStudent->m_stu.stu_classnum));
        this->ui->scoreTable->setItem(row, 4, classNumItem);
        //GPA
        QTableWidgetItem *GPAItem = new QTableWidgetItem(QString::number(startStudent->m_stu.stu_grade_point));
        this->ui->scoreTable->setItem(row, 5, GPAItem);
    }
}

GPATable::~GPATable() {
    delete ui;
}

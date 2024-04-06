#include "gpatable.h"
#include "ui_gpatable.h"
#include "addstudentdialog.h"
#include "globalVar.h"
#include "dataProcess.h"
#include <QShortcut>

extern int CntNum(stu_list *head);

GPATable::GPATable(QWidget *parent, stu_list *head, char course[], char college[]) :
    QWidget(parent),
    ui(new Ui::GPATable) {
    ui->setupUi(this);
    strcpy(m_course, course);
    strcpy(m_college, college);
    this->head = head;

    // 设置表格的边框线
    ui->scoreTable->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent; }");
    ui->scoreTable->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent; }");
    ui->sortSelect->setCurrentIndex(-1);

    //教师端ui
    if (!isAdmin) {
        QStringList horizontalHeaderLabels;
        horizontalHeaderLabels << "姓名" << "学号" << "学院" << "专业" << "班级" << "总成绩"; // 添加表头标签
        ui->scoreTable->setHorizontalHeaderLabels(horizontalHeaderLabels);
        ui->sortSelect->setItemText(0, "成绩升序");
        ui->sortSelect->setItemText(1, "成绩降序");
        ui->label->setText("《" + QString::fromUtf8(course) + "》的所有成绩");
    }
    qDebug() << "9";
    //空表提示
    info = new QLabel("这里还什么都没有哦", this->ui->scoreTable);
    //筛选
    connect(ui->siftBtn, &QPushButton::clicked, [ = ]() {
        if (findListHead != nullptr) {
            delete[] findListHead;
        }
        SiftStudentDialog *siftDialog = new SiftStudentDialog(this, head, m_course, m_college);
        siftDialog->setModal(1);//设为模态对话框
        siftDialog->show();
        connect(siftDialog, &SiftStudentDialog::siftSuccessful, [ = ]() {
            this->findListHead = siftDialog->findListHead;
            siftDialog->deleteLater();//转交表头后销毁
            this->ui->scoreTable->setShowGrid(true);
            if (findListHead != nullptr) {
                printTable(findListHead, 1);
                this->isSifted = true;
                ui->sortSelect->setCurrentIndex(-1);
                ui->sortSelect->setEnabled(true);
                updateStatistics();
                emit Sifted();
            } else {
                printTable(head, 1);
                this->isSifted = false;
                ui->sortSelect->setCurrentIndex(-1);
                ui->sortSelect->setEnabled(true);
                updateStatistics();
                emit unSift();
            }
        });
        connect(siftDialog, &SiftStudentDialog::findVoid, [ = ]() {
            this->findListHead = NULL;
            siftDialog->deleteLater();
            this->ui->scoreTable->setShowGrid(false);
            clearTable();
            pixmapLabel.setParent(this->ui->scoreTable);
            pixmapLabel.setPixmap(this->NonePixmap);
            pixmapLabel.resize(this->NonePixmap.size());
            pixmapLabel.move((this->width() - NonePixmap.width()) / 2, 100);
            pixmapLabel.setStyleSheet("border: none;");
            pixmapLabel.show();
            info->move((this->width() - info->width()) / 2 - 33, 300);
            info->setStyleSheet("border: none; font: 14pt 宋体;");
            info->show();
            this->isSifted = true;
            ui->sortSelect->setCurrentIndex(-1);
            ui->sortSelect->setEnabled(false);
            init_find_result();
            updateStatistics();
            emit Sifted();
        });//未找到
    });
    qDebug() << "10";
    //排序
    connect(ui->sortSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), [ = ](int index) {
        switch (index) {
            case 0://GPA升序
                if (isSifted) {
                    sort_ascend(findListHead);
                    printTable(findListHead, 1);
                } else {
                    sort_ascend(head);
                    printTable(head, 1);
                }
                break;
            case 1://GPA降序
                if (isSifted) {
                    sort_descend(findListHead);
                    printTable(findListHead, 1);
                } else {
                    sort_descend(head);
                    printTable(head, 1);
                }
                break;
            case 2://姓名升序
                if (isSifted) {
                    sort_ascend_by_name(findListHead);
                    printTable(findListHead, 1);
                } else {
                    sort_ascend_by_name(head);
                    printTable(head, 1);
                }
                break;
            case 3://姓名降序
                if (isSifted) {
                    sort_descend_by_name(findListHead);
                    printTable(findListHead, 1);
                } else {
                    sort_descend_by_name(head);
                    printTable(head, 1);
                }
                break;
            default:
                break;
        }
    });
    qDebug() << "10";
    //快捷查找(Admin)
    if (isAdmin) {
        QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);
        connect(shortcut, &QShortcut::activated, [ = ]() {
            QList<QTableWidgetItem *> selectedItems = ui->scoreTable->selectedItems();
            if (selectedItems.isEmpty()) {
                QMessageBox::information(this, "提示", "未选中任何单元格，请重试！");
            } else if (selectedItems.size() == 1) {
                QTableWidgetItem *currentItem = selectedItems.first();
                int ret = find_stu_num(head, ui->scoreTable->item(currentItem->row(), 1)->text().toUtf8().data());
                if (ret != 0) { //找到学号对应的学生
                    AddStudentDialog *retStudentDialog = new AddStudentDialog(this, head, true);
                    retStudentDialog->setWindowTitle("查找学生");
                    retStudentDialog->setModal(true);
                    retStudentDialog->findCnt = ret;
                    retStudentDialog->setStudent(0);
                    retStudentDialog->show();
                } else { //已被删除
                    QMessageBox::information(this, "提示", "该学生可能已被删除，重新进入本界面后刷新！");
                }
            } else {//选中多个
                QMessageBox::information(this, "提示", "只能选中一个单元格，请重试！");
            }
        });
    }
    //统计信息
    if (isAdmin) { //管理员
        if (isSifted) {
            ui->statistics->addItem("当前人数 " + QString::number(CntNum(findListHead)));
            ui->statistics->addItem("平均GPA " + QString::number(averageGPA(findListHead)));
            ui->statistics->addItem("GPA中位数 " + QString::number(medianGPA()));
        } else {
            ui->statistics->addItem("当前人数 " + QString::number(CntNum(head)));
            ui->statistics->addItem("平均GPA " + QString::number(averageGPA(head)));
            ui->statistics->addItem("GPA中位数 " + QString::number(medianGPA()));
        }
    } else {
        if (isSifted) { //已筛选
            ui->statistics->addItem("当前人数 " + QString::number(CntNum(findListHead)));
            ui->statistics->addItem("平均分 " + QString::number(averageGrade(findListHead)));
            ui->statistics->addItem("及格率 " + QString::number(passRate(findListHead) * 100) + "%");
        } else {
            ui->statistics->addItem("当前人数 " + QString::number(CntNum(head)));
            ui->statistics->addItem("平均分 " + QString::number(averageGrade(head)));
            ui->statistics->addItem("及格率 " + QString::number(passRate(head) * 100) + "%");
        }
    }
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
    pixmapLabel.hide();
    info->hide();
    qDebug() << head->m_stu.stu_name;
    int startIndex = (page - 1) * 10;//开始读取的索引值
    int studentNumber = CntNum(head);
    // if (!isAdmin) {
    //     studentNumber = 0;
    //     stu_list *tail = head->next;
    //     for (; tail != NULL; tail = tail->next) {
    //         if (isStudentValid(tail) != -1) {
    //             studentNumber++;
    //         }
    //     }
    // }
    //链表为空
    if (head->next == NULL) {
        this->ui->scoreTable->setRowCount(0);
        this->ui->scoreTable->setColumnCount(0);
        pixmapLabel.setParent(this->ui->scoreTable);
        pixmapLabel.setPixmap(this->NonePixmap);
        pixmapLabel.resize(this->NonePixmap.size());
        pixmapLabel.move((this->width() - NonePixmap.width()) / 2, 100);
        pixmapLabel.setStyleSheet("border: none;");
        pixmapLabel.show();
        info = new QLabel("这里还什么都没有哦", this->ui->scoreTable);
        info->move((this->width() - info->width()) / 2 - 33, 300);
        info->setStyleSheet("border: none; font: 14pt 宋体;");
        info->show();
        return;
    }
    stu_list *startStudent = head->next;
    for (int i = 0; i < startIndex; i++, startStudent = startStudent->next);//定位此页开始学生
    int pageNumber = (studentNumber + 10 - 1) / 10;
    //循环地添加单行数据
    if (isAdmin) {
        this->ui->scoreTable->setRowCount(10);
        this->ui->scoreTable->setColumnCount(6);//重置行列数
        for (int row = 0; page < pageNumber && row < 10 || page == pageNumber
            && row < studentNumber - 10 * (page - 1)/*防止打印多余信息*/; ++row, startStudent = startStudent->next) {
            //学生姓名
            QTableWidgetItem *studentNameItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_name));
            studentNameItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 0, studentNameItem);
            //学号
            QTableWidgetItem *numberItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_number));
            numberItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 1, numberItem);
            //学院
            QTableWidgetItem *collegeItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_college));
            collegeItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 2, collegeItem);
            //专业
            QTableWidgetItem *majorItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_major));
            majorItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 3, majorItem);
            //班级
            QTableWidgetItem *classNumItem = new QTableWidgetItem(QString::number(startStudent->m_stu.stu_classnum));
            classNumItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 4, classNumItem);
            //GPA
            QTableWidgetItem *GPAItem = new QTableWidgetItem(QString::number(startStudent->m_stu.stu_grade_point));
            GPAItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 5, GPAItem);
        }
    } else {
        this->ui->scoreTable->setRowCount(10);
        this->ui->scoreTable->setColumnCount(6);//重置行列数
        for (int row = 0; page < pageNumber && row < 10 || page == pageNumber
            && row < studentNumber - 10 * (page - 1)/*防止打印多余信息*/; ++row, startStudent = startStudent->next) {
            int ret = isStudentValid(startStudent);
            qDebug() << ret;
            //学生姓名
            QTableWidgetItem *studentNameItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_name));
            studentNameItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 0, studentNameItem);
            //学号
            QTableWidgetItem *numberItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_number));
            numberItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 1, numberItem);
            //学院
            QTableWidgetItem *collegeItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_college));
            collegeItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 2, collegeItem);
            //专业
            QTableWidgetItem *majorItem = new QTableWidgetItem(QString::fromUtf8(startStudent->m_stu.stu_major));
            majorItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 3, majorItem);
            //班级
            QTableWidgetItem *classNumItem = new QTableWidgetItem(QString::number(startStudent->m_stu.stu_classnum));
            classNumItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 4, classNumItem);
            //总成绩
            QTableWidgetItem *gradeItem = new QTableWidgetItem(QString::number(startStudent->m_stu.stu_course_grade[ret][1]));
            gradeItem->setTextAlignment(Qt::AlignCenter);
            this->ui->scoreTable->setItem(row, 5, gradeItem);
        }
    }
}

int GPATable::isStudentValid(stu_list *student) {
    for (int i = 0; i < student->m_stu.stu_course_num; i++) {
        if (strcmp(allCourses[(int)student->m_stu.stu_course_grade[i][0]], m_course) == 0) {
            return i;
        }
    }
    return 0;
}


stu_list *GPATable::sort_ascend_by_grade(stu_list *head, int gradeIndex) {
    stu_list *p0, *p, *r0, *r, *tmp;
    p0 = r0 = tmp = NULL;
    p0 = head;
    p = head->next;
    while (p != NULL) {
        r = head->next;
        while (r->m_stu.stu_course_grade[gradeIndex][1] < p->m_stu.stu_course_grade[gradeIndex][1] && r != p) {
            r0 = r;
            r = r->next;
        }
        if (r != p) {
            tmp = p;
            stu_list *q = p->next;
            p0->next = q;
            if (q != NULL) {
                q->prev = p0;
            }
            p = p0;
            if (r == head->next) {
                head->next = tmp;
                tmp->next = r;
                r->prev = tmp;
                tmp->prev = head;
            } else {
                r0->next = tmp;
                tmp->next = r;
                r->prev = tmp;
                tmp->prev = r0;
            }
        }
        p0 = p;
        p = p->next;
    }
    return head;
}

stu_list *GPATable::sort_descend_by_grade(stu_list *head, int gradeIndex) {
    stu_list *p0, *p, *r0, *r, *tmp;
    p0 = r0 = tmp = NULL;
    p0 = head;
    p = head->next;
    while (p != NULL) {
        r = head->next;
        while (r->m_stu.stu_course_grade[gradeIndex][1] > p->m_stu.stu_course_grade[gradeIndex][1] && r != p) {
            r0 = r;
            r = r->next;
        }
        if (r != p) {
            tmp = p;
            stu_list *q = p->next;
            p0->next = q;
            if (q != NULL) {
                q->prev = p0;
            }
            p = p0;
            if (r == head->next) {
                head->next = tmp;
                tmp->next = r;
                r->prev = tmp;
                tmp->prev = head;
            } else {
                r0->next = tmp;
                tmp->next = r;
                r->prev = tmp;
                tmp->prev = r0;
            }
        }
        p0 = p;
        p = p->next;
    }
    return head;
}

double GPATable::averageGrade(stu_list *head) {
    double sum = 0;
    stu_list *p;
    if (head != nullptr) {
        p = head->next;
    } else {
        return 0;
    }
    //查找课程索引
    int index = 0;
    int collegeIndex = findCollegeIndex(m_college);
    qDebug() << "nmd";
    qDebug() << collegeIndex;
    qDebug() << p->m_stu.stu_major;
    int majorIndex = findMajorIndex(collegeIndex, p->m_stu.stu_major);
    qDebug() << "nmd";
    for (; majorCourses[collegeIndex][majorIndex][index] != 0 && strcmp(m_course, allCourses[majorCourses[collegeIndex][majorIndex][index]]) != 0; index++);
    qDebug() << "nmd";
    m_index = index;
    qDebug() << "nmd";
    while (p != NULL) {
        sum += p->m_stu.stu_course_grade[m_index][1];
        p = p->next;
    }
    qDebug() << "nmd";
    return sum / CntNum(head);
}

double GPATable::passRate(stu_list *head) {
    double sum = 0;
    stu_list *p;
    if (head != nullptr) {
        p = head->next;
    } else {
        return 0;
    }
    //查找课程索引
    int index = 0;
    int collegeIndex = findCollegeIndex(m_college);
    while (p != NULL) {
        int majorIndex = findMajorIndex(collegeIndex, p->m_stu.stu_major);
        for (; majorCourses[collegeIndex][majorIndex][index] != '\0' && strcmp(m_course, allCourses[majorCourses[collegeIndex][majorIndex][index]]) != 0; index++);
        m_index = index;
        if (p->m_stu.stu_course_grade[m_index][1] > 60) {
            sum++;
        }
        p = p->next;
    }
    return sum / CntNum(head);
}

double GPATable::averageGPA(stu_list *head) {
    double sum = 0;
    stu_list *p;
    if (head != nullptr) {
        p = head->next;
    } else {
        return 0;
    }
    while (p != NULL) {
        sum += p->m_stu.stu_grade_point;
        p = p->next;
    }
    return sum / CntNum(head);
}

double GPATable::medianGPA() {
    double median = 0;
    stu_list *phead = build();
    load_data(phead);
    sort_ascend(phead);
    stu_list *p = phead->next;
    int medianNumber = CntNum(phead) / 2;
    while (p != NULL && medianNumber--) {
        median = p->m_stu.stu_grade_point;
        p = p->next;
    }
    delete[] phead;
    return median;
}

void GPATable::updateStatistics() {
    ui->statistics->clear();
    ui->statistics->addItem("统计信息");
    if (findListHead == nullptr) {
        return;
    }
    //统计信息
    if (isAdmin) { //管理员

    } else {
        if (isSifted) { //已筛选
            ui->statistics->addItem("当前人数 " + QString::number(CntNum(findListHead)));
            ui->statistics->addItem("平均分 " + QString::number(averageGrade(findListHead)));
            ui->statistics->addItem("及格率 " + QString::number(passRate(findListHead) * 100) + "%");
        } else {
            ui->statistics->addItem("当前人数 " + QString::number(CntNum(head)));
            ui->statistics->addItem("平均分 " + QString::number(averageGrade(head)));
            ui->statistics->addItem("及格率 " + QString::number(passRate(head) * 100) + "%");
        }
    }
}

GPATable::~GPATable() {
    delete ui;
}

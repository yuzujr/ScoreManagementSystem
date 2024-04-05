#pragma once
#include "adminwindow.h"
#include "loginwidget.h"
#include "changepasswddialog.h"
#include "addstudentdialog.h"
#include "globalVar.h"
#include "findstudentdialog.h"
#include "statistics.c"
#include "student.h"
#include "student-grademanager.c"
#include "Worker.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QIntValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QFont>
#include <QShortcut>
#include <QFileDialog>

//TODO：1.修改一下日期的样式，完成奖项、论文编辑操作，完成新建学生操作
//2.查找->修改（基于新建学生窗口）、删除（验证密码）
//3.成绩总览（基本信息+GPA；ctrl+f快速查找->查看详细信息）
//4.实现教师层级...

//为了延长该变量的生命周期，将它设置为全局变量
extern int currentPage;//当前页数

inline int countDigits(int n) {//计算十进制整数位数
    if (n == 0) {
        return 1; // 如果整数为 0，则位数为 1
    }
    // 计算以 10 为底的对数，加上 1 的整数部分即为位数
    return static_cast<int>(log10(std::abs(n))) + 1;
}

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow{parent} {
    //加载背景图
    this->backgroundPixmap.load("://background.jpg");
    //设置窗口图标
    this->setWindowIcon(QIcon("://JLUicon.png"));
    //设置窗口标题
    this->setWindowTitle("职工界面");
    //窗口大小
    this->setFixedSize(800, 600);
    //登录窗口
    LogInWidget *logIn = new LogInWidget(this);
    this->setCentralWidget(logIn);
    //取消登录
    connect(logIn, &LogInWidget::loginCanceled, [ = ]() {
        this->hide();
        delete logIn;
        emit this->backToMenu();
    });
    //调整登录框内容
    logIn->changeClient(QString("职工登录"));
    logIn->changeAccount(QString("职工账号:"));
    //字体
    QFont font;
    font.setPointSize(12);
    font.setFamily("宋体");
    //登录成功
    connect(logIn, &LogInWidget::loginSucceed, [ = ]() {
        logIn->hide();
        this->show();
        delete logIn;
        //登录后操作
        //0.初始化
        //录入职工信息
        Worker worker;
        fscanf(workerFileptr, "%s %s", &worker.workerNumber, &worker.passwd);
        fscanf(workerFileptr, "%d", &worker.m_isAdmin);
        if (!worker.m_isAdmin) {//如果是职工继续扫描
            fscanf(workerFileptr, "%s %s", &worker.stu_college, &worker.teachingCourse);
        }
        moveToLineStart(workerFileptr);
        strcpy(m_course, worker.teachingCourse);
        stu_list *listHead;
        if (!worker.m_isAdmin) {
            stu_list *allList = build();
            load_data(allList);
            find_stu_college(allList, worker.stu_college);
            listHead = build_find_result();
            delete[] allList;
        } else {
            listHead = build();
            load_data(listHead);
        }
        //测试输出
        // stu_list *p = listHead->next;
        // while (p != NULL) {
        //     qDebug("姓名：%s\n", p->m_stu.stu_name);
        //     qDebug("学号：%s  密码：%s\n", p->m_stu.stu_number, p->m_stu.stu_password);
        //     qDebug("学院：%s  专业：%s  绩点：%.1lf\n", p->m_stu.stu_college, p->m_stu.stu_major, p->m_stu.stu_grade_point);
        //     qDebug("课程及其成绩：\n");
        //     for (int i = 0; i < p->m_stu.stu_course_num; i++) {
        //         qDebug("课程%d：%s  成绩：%.1lf\n", i + 1, allCourses[(int)p->m_stu.stu_course_grade[i][0]], p->m_stu.stu_course_grade[i][1]);
        //     }
        //     qDebug("获奖信息：\n");
        //     for (int i = 0; i < p->m_stu.stu_award_num; i++) {
        //         qDebug("奖项%d：%s\n", i + 1, p->m_stu.stu_award[i]);
        //     }
        //     p = p->next;
        // }

        //增加学生(Admin)
        myPushButton *addStudentBtn = new myPushButton(":/btn.png");
        if (worker.m_isAdmin) {
            addStudentBtn->setParent(this);
            addStudentBtn->setText("增加学生");
            addStudentBtn->resize(200, 50);
            addStudentBtn->move((this->width() - addStudentBtn->width()) / 2, 100);
            addStudentBtn->show();
            connect(addStudentBtn, &myPushButton::clicked, [ = ]() {
                AddStudentDialog *addStudentDialog = new AddStudentDialog(this, listHead);
                addStudentDialog->setGeometry(this->geometry());
                addStudentDialog->setModal(1);
                addStudentDialog->show();
                connect(addStudentDialog, &AddStudentDialog::addStudentSuccessful, [ = ]() {
                    delete addStudentDialog;
                    load_data(listHead);
                    QMessageBox::information(this, "提示", "添加成功！");
                });//添加成功
            });
        } else {
            delete addStudentBtn;
        }

        //查找学生(Admin)
        myPushButton *searchStudentBtn = new myPushButton(":/btn.png");
        //导入成绩(Worker)
        myPushButton *importGradesBtn = new myPushButton(":/btn.png");
        if (worker.m_isAdmin) {
            searchStudentBtn->setParent(this);
            searchStudentBtn->setText("查找学生");
            searchStudentBtn->resize(200, 50);
            searchStudentBtn->move((this->width() - searchStudentBtn->width()) / 2, 200);
            searchStudentBtn->show();
            connect(searchStudentBtn, &myPushButton::clicked, [ = ]() {
                FindStudentDialog *findStudentDialog = new FindStudentDialog(this, listHead);
                findStudentDialog->move(this->x() + (this->width() - findStudentDialog->width()) / 2, this->y() + 100);
                findStudentDialog->setModal(1);
                findStudentDialog->show();
            });
        } else {
            delete searchStudentBtn;
            //导入成绩(Worker)
            importGradesBtn->setParent(this);
            importGradesBtn->setText("导入成绩");
            importGradesBtn->resize(200, 50);
            importGradesBtn->move((this->width() - importGradesBtn->width()) / 2, 200);
            importGradesBtn->show();
            connect(importGradesBtn, &myPushButton::clicked, [ = ]() {
                int courseIndex = 0;
                for (; strcmp(allCourses[courseIndex], worker.teachingCourse) != 0; courseIndex++);
                QString fileName = QFileDialog::getOpenFileName(nullptr, "选择文件", "", "文本文件 (*.txt)");
                if (!fileName.isEmpty()) {
                    int addNumber = TxTtoList(listHead, courseIndex, fileName.toUtf8().data());
                    QMessageBox::information(this, "提示", "成功导入" + QString::number(addNumber) + "条成绩！");
                    qDebug() << courseIndex;
                } else {
                    return;
                }
            });
        }

        //3.成绩总览
        myPushButton *scoreOverviewBtn = new myPushButton(":/btn.png");
        scoreOverviewBtn->setParent(this);
        scoreOverviewBtn->setText("成绩总览");
        scoreOverviewBtn->resize(200, 50);
        scoreOverviewBtn->move((this->width() - scoreOverviewBtn->width()) / 2, 300);
        scoreOverviewBtn->show();
        /*返回按钮*/
        myPushButton *backBtn = new myPushButton(":/btn.png");
        backBtn->setParent(this);
        backBtn->setText("返回菜单");
        backBtn->resize(200, 50);
        backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());
        //成绩表
        GPATable *table = new GPATable(this, listHead, worker.teachingCourse, worker.stu_college);
        table->NonePixmap.load(":/None.png");
        //数据
        int studentNumber = CntNum(listHead);//学生总数
        if (!isAdmin) {
            studentNumber = 0;
            stu_list *tail = listHead->next;
            while (tail != NULL) {
                if (isStudentValid(tail) != -1) {
                    studentNumber++;
                    tail = tail->next;
                } else {
                    stu_list *tmp = tail;
                    tail = tail->next;
                    delete_stu(tmp);
                }
            }
        }
        const int pageNumber = (studentNumber + 10 - 1) / 10;//总页数
        /*第i页  总页数 j  跳转：k*/
        QLabel *info = new QLabel("第" + QString::number(currentPage) + "页" + "  总页数 " + QString::number(
                pageNumber) + "  跳转：", this);
        info->setFont(font);
        info->adjustSize();
        info->move(275, 557);
        //变量长度
        int *varLenth = new int(countDigits(currentPage) + countDigits(pageNumber));
        /*跳转文本框*/
        QLineEdit *pageNumberEdit = new QLineEdit(this);
        pageNumberEdit->setFont(font);
        pageNumberEdit->setFixedSize(45, 30);
        pageNumberEdit->move(455 + (*varLenth - 2) * 5, 550); //每多一个数字，右移5个像素
        QRegularExpression regExp("\\d{0,4}"); // 使用正则表达式限制输入为最多4位数字
        QValidator *validator = new QRegularExpressionValidator(regExp, this);
        pageNumberEdit->setValidator(validator);
        /*左翻10页按钮*/
        myPushButton *left10Btn = new myPushButton(":/left10.png");
        left10Btn->setParent(this);
        left10Btn->move(20, 550);
        /*左翻1页按钮*/
        myPushButton *left1Btn = new myPushButton(":/left1.png");
        left1Btn->setParent(this);
        left1Btn->move(60, 550);
        /*右翻1页按钮*/
        myPushButton *right1Btn = new myPushButton(":/right1.png");
        right1Btn->setParent(this);
        right1Btn->move(100, 550);
        /*右翻10页按钮*/
        myPushButton *right10Btn = new myPushButton(":/right10.png");
        right10Btn->setParent(this);
        right10Btn->move(140, 550);
        paginationConnect(table, info, left10Btn, left1Btn, right1Btn, right10Btn, pageNumberEdit, listHead);
        //筛选后
        connect(table, &GPATable::Sifted, [ = ]() {
            paginationConnect(table, info, left10Btn, left1Btn, right1Btn, right10Btn, pageNumberEdit, table->findListHead);
        });
        //取消筛选
        connect(table, &GPATable::unSift, [ = ]() {
            paginationConnect(table, info, left10Btn, left1Btn, right1Btn, right10Btn, pageNumberEdit, listHead);
        });

        //4.修改密码
        myPushButton *changePasswdBtn = new myPushButton(":/btn.png");
        changePasswdBtn->setParent(this);
        changePasswdBtn->setText("修改密码");
        changePasswdBtn->resize(200, 50);
        changePasswdBtn->move((this->width() - changePasswdBtn->width()) / 2, 400);
        connect(changePasswdBtn, &myPushButton::clicked, [ = ]() {
            QTimer::singleShot(200, [ = ]() {
                changePasswdDialog *cpd = new changePasswdDialog(this);
                cpd->setModal(1);//设为模态对话框
                this->setCentralWidget(cpd);
            });
        });
        changePasswdBtn->show();

        //5.退出登录
        myPushButton *exitBtn = new myPushButton(":/btn.png");
        exitBtn->setParent(this);
        exitBtn->setText("退出登录");
        exitBtn->resize(200, 50);
        exitBtn->move((this->width() - exitBtn->width()) / 2, 500);
        connect(exitBtn, &myPushButton::clicked, [ = ]() {
            QTimer::singleShot(200, [ = ]() {
                this->hide();
                this->parentWidget()->show();
            });
        });
        exitBtn->show();

        //查看成绩，界面操作
        connect(scoreOverviewBtn, &myPushButton::clicked, [ = ]() {
            if (worker.m_isAdmin) {
                addStudentBtn->hide();
                searchStudentBtn->hide();
            } else {
                importGradesBtn->hide();
            }
            scoreOverviewBtn->hide();
            changePasswdBtn->hide();
            exitBtn->hide();
            left10Btn->show();
            left1Btn->show();
            right1Btn->show();
            right10Btn->show();
            info->show();
            pageNumberEdit->show();
            backBtn->show();
            this->setCentralWidget(table);
            table->printTable(listHead, 1);
            table->show();
        });

        //返回管理员界面
        connect(backBtn, &myPushButton::clicked, [ = ]() {
            if (worker.m_isAdmin) {
                addStudentBtn->show();
                searchStudentBtn->show();
            } else {
                importGradesBtn->show();
            }
            scoreOverviewBtn->show();
            changePasswdBtn->show();
            exitBtn->show();
            left10Btn->hide();
            left1Btn->hide();
            right1Btn->hide();
            right10Btn->hide();
            info->hide();
            pageNumberEdit->hide();
            backBtn->hide();
            table->hide();
        });
    });
}

void AdminWindow::paginationConnect(GPATable *table, QLabel *info, myPushButton *left10Btn, myPushButton *left1Btn, myPushButton *right1Btn,
    myPushButton *right10Btn, QLineEdit *pageNumberEdit, stu_list *listHead) {
    currentPage = 1;
    //数据
    int studentNumber = 0; //学生总数
    if (listHead != NULL) {
        studentNumber = CntNum(listHead);
    }
    if (!isAdmin) {
        studentNumber = 0;
        if (listHead != NULL) {
            stu_list *tail = listHead->next;
            while (tail != NULL) {
                if (isStudentValid(tail) != -1) {
                    studentNumber++;
                    tail = tail->next;
                }
            }
        }
    }
    const int pageNumber = (studentNumber + 10 - 1) / 10;//总页数
    //更新ui
    updateUI(currentPage, pageNumber, pageNumberEdit, info);
    //解连接
    disconnect(pageNumberEdit, nullptr, nullptr, nullptr);
    disconnect(left10Btn, nullptr, nullptr, nullptr);
    disconnect(left1Btn, nullptr, nullptr, nullptr);
    disconnect(right1Btn, nullptr, nullptr, nullptr);
    disconnect(right10Btn, nullptr, nullptr, nullptr);
    if (studentNumber == 0) {
        return;
    }
    //跳转到某页
    connect(pageNumberEdit, &QLineEdit::returnPressed, [ =, &currentPage ]() {
        if (pageNumberEdit->text().isEmpty());
        else {
            int newPageNumber = pageNumberEdit->text().toInt();//防止001，049等数字出现
            if (newPageNumber == 0) {
                newPageNumber = 1;
            }//如果页码为0，跳转到第一页
            if (newPageNumber > pageNumber) {
                newPageNumber = pageNumber;
            }//如果页码大于最大页码，跳转到最后一页
            pageNumberEdit->setText(QString::number(newPageNumber));
            currentPage = newPageNumber;
            table->printTable(listHead, newPageNumber);
            updateUI(currentPage, pageNumber, pageNumberEdit, info);
        }
    });
    //左翻10页按钮
    connect(left10Btn, &myPushButton::clicked, [ =, &currentPage ]() {
        if (currentPage == 1);//不做任何处理
        else if (currentPage - 10 < 1) {
            table->printTable(listHead, 1);
            currentPage = 1;
            updateUI(currentPage, pageNumber, pageNumberEdit, info);
        } //左边界
        else {
            table->printTable(listHead, currentPage - 10);
            currentPage -= 10;
            updateUI(currentPage, pageNumber, pageNumberEdit, info);
        }
    });
    /*左翻1页按钮*/
    connect(left1Btn, &myPushButton::clicked, [ =, &currentPage ]() {
        if (currentPage == 1); //不做任何处理
        else {
            table->printTable(listHead, currentPage - 1);
            currentPage -= 1;
            updateUI(currentPage, pageNumber, pageNumberEdit, info);
        }
    });
    /*右翻1页按钮*/
    connect(right1Btn, &myPushButton::clicked, [ =, &currentPage]() {
        if (currentPage == pageNumber); //不做任何处理
        else {
            table->printTable(listHead, currentPage + 1);
            currentPage += 1;
            updateUI(currentPage, pageNumber, pageNumberEdit, info);
        }
    });
    /*右翻10页按钮*/
    connect(right10Btn, &myPushButton::clicked, [ =, &currentPage ]() {
        if (currentPage == pageNumber);//不做任何处理
        else if (currentPage + 10 > pageNumber) {
            table->printTable(listHead, pageNumber);
            currentPage = pageNumber;
            updateUI(currentPage, pageNumber, pageNumberEdit, info);
        } //右边界
        else {
            table->printTable(listHead, currentPage + 10);
            currentPage += 10;
            updateUI(currentPage, pageNumber, pageNumberEdit, info);
        }
    });
}

// 更新界面函数
void AdminWindow::updateUI(int currentPage, int pageNumber, QLineEdit *pageNumberEdit, QLabel *info) {
    info->setText("第" + QString::number(currentPage) + "页" + "  总页数 " + QString::number(pageNumber) + "  跳转：");
    int varLength = countDigits(currentPage) + countDigits(pageNumber);
    pageNumberEdit->move(455 + (varLength - 2) * 5, 550);
}

int AdminWindow::isStudentValid(stu_list *student) {
    for (int i = 0; i < student->m_stu.stu_course_num; i++) {
        if (strcmp(allCourses[(int)student->m_stu.stu_course_grade[i][0]], m_course) == 0) {
            return i;
        }
    }
    return -1;
}

int AdminWindow::TxTtoList(stu_list *phead, int MKnum, char fileName[]) {
    FILE *Out = fopen(fileName, "r");
    if (Out == NULL) {
        qDebug() << "文件打开失败\n";
        return -1;
    } else {
        int addNumber = 0;
        while (!feof(Out)) {
            char arr[100] = { 0 };
            double mark = 0;
            fscanf(Out, "%s %lf", arr, &mark);
            fscanf(Out, "\n");
            for (int i = 0; i < 100; i++) {
                find_result[i] = NULL;
            }
            int number = 0;
            stu_list *p = phead->next;
            while (p != NULL) {
                if (!strcmp(p->m_stu.stu_number, arr)) {
                    find_result[number] = p;
                    number++;
                }
                p = p->next;
            }
            int i = 0;
            for (i = 0; i < find_result[0]->m_stu.stu_course_num; i++) {
                if (find_result[0]->m_stu.stu_course_grade[i][0] == MKnum) {
                    find_result[0]->m_stu.stu_course_grade[i][1] = mark;
                    addNumber++;
                }
            }

        }
        save_data(phead);
        return addNumber;
    }
}

void AdminWindow::paintEvent(QPaintEvent *event) {
    //创建主界面
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), this->backgroundPixmap);
}

void AdminWindow::closeEvent(QCloseEvent *event) {
    emit closed();
    AdminWindow::closeEvent(event);
}

AdminWindow::~AdminWindow() {

}

#pragma once
#include "adminwindow.h"
#include "loginwidget.h"
#include "mypushbutton.h"
#include "changepasswddialog.h"
#include "student.h"
#include "student-grademanager.c"
#include "addstudentdialog.h"
#include "globalVar.h"
#include "findstudentdialog.h"
#include "gpatable.h"
#include "statistics.c"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>

//TODO：1.修改一下日期的样式，完成奖项、论文编辑操作，完成新建学生操作
//2.查找->修改（基于新建学生窗口）、删除（验证密码）
//3.成绩总览（基本信息+GPA；ctrl+f快速查找->查看详细信息）
//4.实现教师层级...
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
    //登录成功
    connect(logIn, &LogInWidget::loginSucceed, [ = ]() {
        logIn->hide();
        this->show();
        delete logIn;
        //登录后操作
        //0.初始化链表
        stu_list *listHead = build();
        load_data(listHead);
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

        //1.增加学生
        myPushButton *addStudentBtn = new myPushButton(":/btn.png");
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
            });//添加成功
        });

        //2.查找学生
        myPushButton *searchStudentBtn = new myPushButton(":/btn.png");
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

        //3.成绩总览(统计，按GPA、姓名首字母排名）(快速搜索ctrl+f）
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
        GPATable *table = new GPATable(this);
        table->NonePixmap.load(":/None.png");
        //数据
        const int studentNumber = CntNum(listHead);//学生总数
        const int pageNumber = (studentNumber + 10 - 1) / 10;//总页数
        /*x-y 总记录数 z  总页数 j  跳转：k*/
        // QLabel *info = new QLabel(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(currentPage * 10) + " 总记录数 " + QString::number(
        //         studentNumber) + "  总页数 " + QString::number(
        //         pageNumber) + "  跳转：", this);
        // info->setFont(font);
        // info->adjustSize();
        // info->move(190, 557);
        // //变量长度
        // int *varLenth = new int(countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(studentNumber) + countDigits(pageNumber));
        // /*跳转文本框*/
        // QLineEdit *pageNumberEdit = new QLineEdit(this);
        // pageNumberEdit->setFont(font);
        // pageNumberEdit->setFixedSize(45, 30);
        // pageNumberEdit->move(455 + (*varLenth - 6) * 5, 550); //每多一个数字，右移5个像素
        // // 设置只能输入数字
        // QRegularExpression regExp("\\d{0,4}"); // 使用正则表达式限制输入为最多4位数字
        // QValidator *validator = new QRegularExpressionValidator(regExp, this);
        // pageNumberEdit->setValidator(validator);
        // //跳转到某页
        // connect(pageNumberEdit, &QLineEdit::returnPressed, [ =, &currentPage ]() {
        //     if (pageNumberEdit->text().isEmpty());
        //     else {
        //         int newPageNumber = pageNumberEdit->text().toInt();//防止001，049等数字出现
        //         if (newPageNumber == 0) {
        //             newPageNumber = 1;
        //         }//如果页码为0，跳转到第一页
        //         if (newPageNumber > pageNumber) {
        //             newPageNumber = pageNumber;
        //         }//如果页码大于最大页码，跳转到最后一页
        //         pageNumberEdit->setText(QString::number(newPageNumber));
        //         currentPage = newPageNumber;
        //         table->printTable(student, newPageNumber);
        //         info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(studentNumber) + " 总记录数 " + QString::number(
        //                 studentNumber) + "  总页数 " + QString::number(
        //                 pageNumber) + "  跳转：");
        //         int *varLenth = new int(countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(studentNumber) + countDigits(pageNumber));
        //         pageNumberEdit->move(455 + (*varLenth - 6) * 5, 550); //每多一个数字，右移5个像素
        //     }
        // });
        // /*左翻10页按钮*/
        // myPushButton *left10Btn = new myPushButton(":/left10.png");
        // left10Btn->setParent(this);
        // left10Btn->move(20, 550);
        // connect(left10Btn, &myPushButton::clicked, [ =, &currentPage ]() {
        //     if (currentPage == 1);//不做任何处理
        //     else if (currentPage - 10 < 1) {
        //         table->printTable(student, 1);
        //         currentPage = 1;
        //         info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(studentNumber) + " 总记录数 " + QString::number(
        //                 studentNumber) + "  总页数 " + QString::number(
        //                 pageNumber) + "  跳转：");
        //         *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(studentNumber) + countDigits(pageNumber);
        //         pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
        //     } //左边界
        //     else {
        //         table->printTable(student, currentPage - 10);
        //         currentPage -= 10;
        //         info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(studentNumber) + " 总记录数 " + QString::number(
        //                 studentNumber) + "  总页数 " + QString::number(
        //                 pageNumber) + "  跳转：");
        //         *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(studentNumber) + countDigits(pageNumber);
        //         pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
        //     }
        // });
        // /*左翻1页按钮*/
        // myPushButton *left1Btn = new myPushButton(":/left1.png");
        // left1Btn->setParent(this);
        // left1Btn->move(60, 550);
        // connect(left1Btn, &myPushButton::clicked, [ =, &currentPage ]() {
        //     if (currentPage == 1); //不做任何处理
        //     else {
        //         table->printTable(student, currentPage - 1);
        //         currentPage -= 1;
        //         info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(studentNumber) + " 总记录数 " + QString::number(
        //                 studentNumber) + "  总页数 " + QString::number(
        //                 pageNumber) + "  跳转：");
        //         *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(studentNumber) + countDigits(pageNumber);
        //         pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
        //     }
        // });
        // /*右翻1页按钮*/
        // myPushButton *right1Btn = new myPushButton(":/right1.png");
        // right1Btn->setParent(this);
        // right1Btn->move(100, 550);
        // connect(right1Btn, &myPushButton::clicked, [ =, &currentPage]() {
        //     if (currentPage == pageNumber); //不做任何处理
        //     else {
        //         table->printTable(student, currentPage + 1);
        //         currentPage += 1;
        //         info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(studentNumber) + " 总记录数 " + QString::number(
        //                 studentNumber) + "  总页数 " + QString::number(
        //                 pageNumber) + "  跳转：");
        //         *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(studentNumber) + countDigits(pageNumber);
        //         pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
        //     }
        // });
        // /*右翻10页按钮*/
        // myPushButton *right10Btn = new myPushButton(":/right10.png");
        // right10Btn->setParent(this);
        // right10Btn->move(140, 550);
        // connect(right10Btn, &myPushButton::clicked, [ =, &currentPage ]() {
        //     if (currentPage == pageNumber);//不做任何处理
        //     else if (currentPage + 10 > pageNumber) {
        //         table->printTable(student, pageNumber);
        //         currentPage = pageNumber;
        //         info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(studentNumber) + " 总记录数 " + QString::number(
        //                 studentNumber) + "  总页数 " + QString::number(
        //                 pageNumber) + "  跳转：");
        //         *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(studentNumber) + countDigits(pageNumber);
        //         pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
        //     } //右边界
        //     else {
        //         table->printTable(student, currentPage + 10);
        //         currentPage += 10;
        //         info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(studentNumber) + " 总记录数 " + QString::number(
        //                 studentNumber) + "  总页数 " + QString::number(
        //                 pageNumber) + "  跳转：");
        //         *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(studentNumber) + countDigits(pageNumber);
        //         pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
        //     }
        // });

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
            addStudentBtn->hide();
            searchStudentBtn->hide();
            scoreOverviewBtn->hide();
            changePasswdBtn->hide();
            exitBtn->hide();
            // left10Btn->show();
            // left1Btn->show();
            // right1Btn->show();
            // right10Btn->show();
            // info->show();
            // pageNumberEdit->show();
            backBtn->show();
            this->setCentralWidget(table);
            table->printTable(listHead, 1);
            table->show();
        });
    });
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

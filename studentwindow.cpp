#pragma once
#include "studentwindow.h"
#include "mypushbutton.h"
#include "loginwidget.h"
#include "changepasswddialog.h"
#include "student-grademanager.h"
#include "dataProcess.h"
#include "projecttable.h"
#include "globalVar.h"
#include <QTimer>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QIntValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QPixmap>
#include <cmath>

// extern Student *student;

//为了延长该变量的生命周期，将它设置为全局变量
int currentPage = 1;//成绩当前页数
int paperCurPage = 1;
int awardCurPage = 1;

int countDigits(int n) {//计算十进制整数位数
    if (n == 0) {
        return 1; // 如果整数为 0，则位数为 1
    }
    // 计算以 10 为底的对数，加上 1 的整数部分即为位数
    return static_cast<int>(log10(std::abs(n))) + 1;
}

StudentWindow::StudentWindow(QWidget *parent)
    : QMainWindow(parent) {

    //加载背景图
    this->backgroundPixmap.load(":/background.jpg");
    this->topMarginPixmap.load(":/topMargin.png");

    //字体
    QFont font;
    font.setPointSize(12);
    font.setFamily("宋体");

    //设置窗口图标
    this->setWindowIcon(QIcon("://JLUicon.png"));

    //设置窗口标题
    this->setWindowTitle("学生界面");

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

    //登录成功
    connect(logIn, &LogInWidget::loginSucceed, [ =, &currentPage ]() {
        logIn->hide();
        delete logIn;
        //登录后操作

        //0.学生初始化
        student = loadSingleStudent();
        ScoreTable *table = new ScoreTable(this);
        ProjectTable *ptable = new ProjectTable(this, student);
        table->NonePixmap.load(":/None.png");
        ptable->NonePixmap.load(":/None.png");

        //1.查看成绩,排名按钮
        myPushButton *scoreInquireBtn = new myPushButton(":/btn.png");
        scoreInquireBtn->setParent(this);
        scoreInquireBtn->setText("查看成绩");
        scoreInquireBtn->resize(200, 50);
        scoreInquireBtn->move((this->width() - scoreInquireBtn->width()) / 2, 200);
        scoreInquireBtn->show();

        //2.综合类项目入口按钮
        myPushButton *projectBtn = new myPushButton(":/btn.png");
        projectBtn->setParent(this);
        projectBtn->setText("综合类项目");
        projectBtn->resize(200, 50);
        projectBtn->move((this->width() - projectBtn->width()) / 2, 300);
        projectBtn->show();

        //3.修改密码
        myPushButton *changePasswdBtn = new myPushButton(":/btn.png");
        changePasswdBtn->setParent(this);
        changePasswdBtn->setText("修改密码");
        changePasswdBtn->resize(200, 50);
        changePasswdBtn->move((this->width() - changePasswdBtn->width()) / 2, 400);
        connect(changePasswdBtn, &myPushButton::clicked, [ = ]() {
            QTimer::singleShot(100, [ = ]() {
                changePasswdDialog *cpd = new changePasswdDialog(this);
                cpd->setModal(1);//设为模态对话框
                this->setCentralWidget(cpd);
            });
        });
        changePasswdBtn->show();

        //4.退出登录
        myPushButton *exitBtn = new myPushButton(":/btn.png");
        exitBtn->setParent(this);
        exitBtn->setText("退出登录");
        exitBtn->resize(200, 50);
        exitBtn->move((this->width() - exitBtn->width()) / 2, 500);
        connect(exitBtn, &myPushButton::clicked, [ = ]() {
            this->hide();
            this->parentWidget()->show();
        });
        exitBtn->show();

        //查看成绩
        /*返回按钮*/
        myPushButton *backBtn = new myPushButton(":/btn.png");
        backBtn->setParent(this);
        backBtn->setText("返回菜单");
        backBtn->resize(200, 50);
        backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

        //数据
        const int courseNumber = student->stu_course_num;//总课程数
        const int pageNumber = (courseNumber + 10 - 1) / 10;//总页数

        /*x-y 总记录数 z  总页数 j  跳转：k*/
        QLabel *info = new QLabel(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(currentPage * 10) + " 总记录数 " + QString::number(
                courseNumber) + "  总页数 " + QString::number(
                pageNumber) + "  跳转：", this);
        info->setFont(font);
        info->adjustSize();
        info->move(190, 557);

        //变量长度
        int *varLenth = new int(countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(courseNumber) + countDigits(pageNumber));
        /*跳转文本框*/
        QLineEdit *pageNumberEdit = new QLineEdit(this);
        pageNumberEdit->setFont(font);
        pageNumberEdit->setFixedSize(45, 30);
        pageNumberEdit->move(455 + (*varLenth - 6) * 5, 550); //每多一个数字，右移5个像素

        // 设置只能输入数字
        QRegularExpression regExp("\\d{0,4}"); // 使用正则表达式限制输入为最多4位数字
        QValidator *validator = new QRegularExpressionValidator(regExp, this);
        pageNumberEdit->setValidator(validator);

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
                table->printTable(student, newPageNumber);
                info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(courseNumber) + " 总记录数 " + QString::number(
                        courseNumber) + "  总页数 " + QString::number(
                        pageNumber) + "  跳转：");
                int *varLenth = new int(countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(courseNumber) + countDigits(pageNumber));
                pageNumberEdit->move(455 + (*varLenth - 6) * 5, 550); //每多一个数字，右移5个像素
            }
        });

        /*左翻10页按钮*/
        myPushButton *left10Btn = new myPushButton(":/left10.png");
        left10Btn->setParent(this);
        left10Btn->move(20, 550);
        connect(left10Btn, &myPushButton::clicked, [ =, &currentPage ]() {
            if (currentPage == 1);//不做任何处理
            else if (currentPage - 10 < 1) {
                table->printTable(student, 1);
                currentPage = 1;
                info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(courseNumber) + " 总记录数 " + QString::number(
                        courseNumber) + "  总页数 " + QString::number(
                        pageNumber) + "  跳转：");
                *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(courseNumber) + countDigits(pageNumber);
                pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
            } //左边界
            else {
                table->printTable(student, currentPage - 10);
                currentPage -= 10;
                info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(courseNumber) + " 总记录数 " + QString::number(
                        courseNumber) + "  总页数 " + QString::number(
                        pageNumber) + "  跳转：");
                *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(courseNumber) + countDigits(pageNumber);
                pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
            }
        });

        /*左翻1页按钮*/
        myPushButton *left1Btn = new myPushButton(":/left1.png");
        left1Btn->setParent(this);
        left1Btn->move(60, 550);
        connect(left1Btn, &myPushButton::clicked, [ =, &currentPage ]() {
            if (currentPage == 1); //不做任何处理
            else {
                table->printTable(student, currentPage - 1);
                currentPage -= 1;
                info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(courseNumber) + " 总记录数 " + QString::number(
                        courseNumber) + "  总页数 " + QString::number(
                        pageNumber) + "  跳转：");
                *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(courseNumber) + countDigits(pageNumber);
                pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
            }
        });

        /*右翻1页按钮*/
        myPushButton *right1Btn = new myPushButton(":/right1.png");
        right1Btn->setParent(this);
        right1Btn->move(100, 550);
        connect(right1Btn, &myPushButton::clicked, [ =, &currentPage]() {
            if (currentPage == pageNumber); //不做任何处理
            else {
                table->printTable(student, currentPage + 1);
                currentPage += 1;
                info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(courseNumber) + " 总记录数 " + QString::number(
                        courseNumber) + "  总页数 " + QString::number(
                        pageNumber) + "  跳转：");
                *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(courseNumber) + countDigits(pageNumber);
                pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
            }
        });

        /*右翻10页按钮*/
        myPushButton *right10Btn = new myPushButton(":/right10.png");
        right10Btn->setParent(this);
        right10Btn->move(140, 550);
        connect(right10Btn, &myPushButton::clicked, [ =, &currentPage ]() {
            if (currentPage == pageNumber);//不做任何处理
            else if (currentPage + 10 > pageNumber) {
                table->printTable(student, pageNumber);
                currentPage = pageNumber;
                info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(courseNumber) + " 总记录数 " + QString::number(
                        courseNumber) + "  总页数 " + QString::number(
                        pageNumber) + "  跳转：");
                *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(courseNumber) + countDigits(pageNumber);
                pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
            } //右边界
            else {
                table->printTable(student, currentPage + 10);
                currentPage += 10;
                info->setText(QString::number((currentPage - 1) * 10 + 1) + "-" + QString::number(courseNumber) + " 总记录数 " + QString::number(
                        courseNumber) + "  总页数 " + QString::number(
                        pageNumber) + "  跳转：");
                *varLenth = countDigits((currentPage - 1) * 10 + 1) + countDigits(currentPage * 10) + countDigits(courseNumber) + countDigits(pageNumber);
                pageNumberEdit->move(455 + (*varLenth - 6) * 6, 550); //每多一个数字，右移6个像素
            }
        });

        //学生形象，账号信息
        QPixmap *image = new QPixmap(":/image.png");
        QLabel *imageLabel = new QLabel(this);
        imageLabel->setPixmap(*image);
        imageLabel->move(650, 5);
        imageLabel->show();
        imageLabel->resize(QSize(image->size()));
        QLabel *infoLabel = new QLabel(this);
        infoLabel->setFont(font);
        infoLabel->setText(QString::fromUtf8(student->stu_number) + "\n" + QString::fromUtf8(student->stu_name));
        infoLabel->adjustSize();
        infoLabel->move(725, 10);
        infoLabel->show();

        //顶部边缘图片
        QLabel *topMarginLabel = new QLabel(this);
        topMarginLabel->setPixmap(this->topMarginPixmap);
        topMarginLabel->move(0, 0);
        topMarginLabel->resize(800, 50);

        //查看成绩，界面操作
        connect(scoreInquireBtn, &myPushButton::clicked, [ = ]() {
            if (changePasswdBtn->isVisible()) {
                changePasswdBtn->hide();
            }
            if (scoreInquireBtn->isVisible()) {
                scoreInquireBtn->hide();
            }
            if (projectBtn->isVisible()) {
                projectBtn->hide();
            }
            if (exitBtn->isVisible()) {
                exitBtn->hide();
            }
            if (imageLabel->isVisible()) {
                imageLabel->hide();
            }
            if (infoLabel->isVisible()) {
                infoLabel->hide();
            }
            left10Btn->show();
            left1Btn->show();
            right1Btn->show();
            right10Btn->show();
            info->show();
            pageNumberEdit->show();
            backBtn->show();
            this->setCentralWidget(table);
            table->printTable(student, 1);
            table->show();
            topMarginLabel->show();
        });

        /*左翻1页按钮*/
        myPushButton *left1Btn2 = new myPushButton(":/left1.png");
        left1Btn2->setParent(this);
        left1Btn2->move(60, 550);

        /*右翻1页按钮*/
        myPushButton *right1Btn2 = new myPushButton(":/right1.png");
        right1Btn2->setParent(this);
        right1Btn2->move(100, 550);

        /*返回按钮*/
        myPushButton *backBtn2 = new myPushButton(":/btn.png");
        backBtn2->setParent(this);
        backBtn2->setText("返回菜单");
        backBtn2->resize(200, 50);
        backBtn2->move(this->width() - backBtn2->width(), this->height() - backBtn2->height());

        //QLabel
        QLabel *pageLabel = new QLabel(this);

        //综合类项目，界面操作
        connect(projectBtn, &QPushButton::clicked, [ = ] {
            if (changePasswdBtn->isVisible())
                changePasswdBtn->hide();
            if (scoreInquireBtn->isVisible())
                scoreInquireBtn->hide();
            if (projectBtn->isVisible())
                projectBtn->hide();
            if (exitBtn->isVisible())
                exitBtn->hide();
            if (imageLabel->isVisible())
                imageLabel->hide();
            if (infoLabel->isVisible())
                infoLabel->hide();
            backBtn2->show();
            this->setCentralWidget(ptable);
            ptable->show();
        });
        int awardNum = student->stu_award_num;
        int paperNum = student->stu_paper_num;
        connect(ptable, &ProjectTable::awardClicked, [ = ]() {
            awardCurPage = 1;
            left1Btn2->show();
            right1Btn2->show();
            QString pageString = "第" + QString::number(awardCurPage) + "页" + "   共" + QString::number(awardNum) + "页";
            pageLabel->setText(pageString);
            pageLabel->setFont(font);
            pageLabel->adjustSize();
            pageLabel->move(190, 557);
            pageLabel->show();
            //先断开其他连接
            disconnect(left1Btn2, &QPushButton::clicked, nullptr, nullptr);
            disconnect(right1Btn2, &QPushButton::clicked, nullptr, nullptr);
            connect(left1Btn2, &myPushButton::clicked, [ = ]() {
                if (awardCurPage == 1); //不做任何处理
                else {
                    ptable->clearTable();
                    ptable->printAward(student, awardCurPage - 1);
                    awardCurPage -= 1;
                    pageLabel->setText("第" + QString::number(awardCurPage) + "页" + "   共" + QString::number(awardNum) + "页");
                }
            });

            connect(right1Btn2, &myPushButton::clicked, [ = ]() {
                if (awardCurPage == awardNum); //不做任何处理
                else {
                    ptable->clearTable();
                    ptable->printAward(student, awardCurPage + 1);
                    awardCurPage += 1;
                    pageLabel->setText("第" + QString::number(awardCurPage) + "页" + "   共" + QString::number(awardNum) + "页");
                }
            });
        });
        connect(ptable, &ProjectTable::paperClicked, [ = ]() {
            paperCurPage = 1;
            left1Btn2->show();
            right1Btn2->show();
            QString pageString = "第" + QString::number(paperCurPage) + "页" + "   共" + QString::number(paperNum) + "页";
            pageLabel->setText(pageString);
            pageLabel->setFont(font);
            pageLabel->adjustSize();
            pageLabel->move(190, 557);
            pageLabel->show();
            //先断开其他连接
            disconnect(left1Btn2, &QPushButton::clicked, nullptr, nullptr);
            disconnect(right1Btn2, &QPushButton::clicked, nullptr, nullptr);
            connect(left1Btn2, &myPushButton::clicked, [ = ]() {
                if (paperCurPage == 1); //不做任何处理
                else {
                    ptable->clearTable();
                    ptable->printPaper(student, paperCurPage - 1);
                    paperCurPage -= 1;
                    pageLabel->setText("第" + QString::number(paperCurPage) + "页" + "   共" + QString::number(paperNum) + "页");
                }
            });

            connect(right1Btn2, &myPushButton::clicked, [ =, &paperCurPage]() {
                if (paperCurPage == paperNum); //不做任何处理
                else {
                    ptable->clearTable();
                    ptable->printPaper(student, paperCurPage + 1);
                    paperCurPage += 1;
                    pageLabel->setText("第" + QString::number(paperCurPage) + "页" + "   共" + QString::number(paperNum) + "页");
                }
            });
        });
        //返回学生界面
        connect(backBtn, &myPushButton::clicked, [ = ]() {
            changePasswdBtn->show();
            scoreInquireBtn->show();
            exitBtn->show();
            imageLabel->show();
            infoLabel->show();
            projectBtn->show();
            left10Btn->hide();
            left1Btn->hide();
            right1Btn->hide();
            right10Btn->hide();
            info->hide();
            pageNumberEdit->hide();
            backBtn->hide();
            table->hide();
            topMarginLabel->hide();
        });
        //素质项目返回学生界面
        connect(backBtn2, &myPushButton::clicked, [ = ]() {
            changePasswdBtn->show();
            scoreInquireBtn->show();
            exitBtn->show();
            imageLabel->show();
            infoLabel->show();
            projectBtn->show();
            ptable->hide();
            right1Btn2->hide();
            left1Btn2->hide();
            pageLabel->hide();
            backBtn2->hide();
            ptable->clearTable();
        });
    });
}

void StudentWindow::paintEvent(QPaintEvent *event) {
    //创建主界面
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), this->backgroundPixmap);
}

void StudentWindow::closeEvent(QCloseEvent *event) {
    emit closed();
    StudentWindow::closeEvent(event);
}

StudentWindow::~StudentWindow() {
}

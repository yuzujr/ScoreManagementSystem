#include "siftstudentdialog.h"
#include "ui_siftstudentdialog.h"

//TODO:当前筛选显示，筛选界面翻页

SiftStudentDialog::SiftStudentDialog(QWidget *parent, stu_list *head, char course[], char college[]) :
    QDialog(parent),
    ui(new Ui::SiftStudentDialog) {
    ui->setupUi(this);
    //选择框
    //暂时禁用班级输入
    ui->classEdit->setEnabled(false);
    //默认学院、专业、课程
    ui->collegeSelect->addItem("无");
    ui->majorSelect->addItem("无");
    for (int i = 0; colleges[i][0] != '\0' && i < MAX_COLLEGE_COUNT; i++) {
        ui->collegeSelect->addItem(colleges[i]);
    }
    if (!isAdmin) {
        ui->collegeSelect->setEnabled(false);
        int index = 0;
        for (; strcmp(colleges[index], college) != 0 && index < MAX_COLLEGE_COUNT; index++);
        ui->collegeSelect->setCurrentIndex(index + 1);
        for (int i = 0; majors[index][i][0] != '\0' && i < MAX_MAJOR_PER_COLLEGE; i++) {
            ui->majorSelect->addItem(majors[index][i]);
            ui->classEdit->setEnabled(true);
        }
    }
    if (isAdmin) {
        ui->collegeSelect->setCurrentIndex(0);
    }
    ui->majorSelect->setCurrentIndex(0);
    //学院改变后，更改专业
    connect(ui->collegeSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        ui->majorSelect->clear();
        ui->majorSelect->addItem("无");
        if (index == 0) { //如果学院选择无
            ui->classEdit->clear();
            ui->classEdit->setEnabled(false);
            return;
        }
        for (int i = 0; majors[index - 1][i][0] != '\0' && i < MAX_MAJOR_PER_COLLEGE; i++) {
            ui->majorSelect->addItem(majors[index - 1][i]);
        }
        ui->majorSelect->setCurrentIndex(0);
        ui->classEdit->setEnabled(true);
    });
    //选择专业后可编辑班级
    connect(ui->majorSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        ui->classEdit->setEnabled(true);
    });
    //班级设置只能输入数字
    QRegularExpression regExp("\\d{0,2}"); // 使用正则表达式限制输入为两位数字
    QValidator *validator = new QRegularExpressionValidator(regExp, this);
    ui->classEdit->setValidator(validator);
    //确认
    connect(ui->okBtn, &QPushButton::clicked, [ = ]() {
        if (ui->collegeSelect->currentIndex() == 0) { //学院为空
            findListHead = NULL;
            this->close();
            emit siftSuccessful();
        } else if (ui->majorSelect->currentIndex() == 0) { //专业为空
            if (ui->classEdit->text().isEmpty()) { //班级为空
                //仅学院
                int ret = find_stu_college(head, ui->collegeSelect->currentText().toUtf8().data());
                findListHead = NULL;
                if (ret != 0) {
                    findListHead = build_find_result();
                    this->close();
                    emit siftSuccessful();
                } else {//未找到
                    this->close();
                    emit findVoid();
                }
            } else {
                //学院、班级
                int ret = find_stu_class(head, ui->collegeSelect->currentText().toUtf8().data(), ui->classEdit->text().toInt(), NULL);
                findListHead = NULL;
                if (ret != 0) {
                    findListHead = build_find_result();
                    this->close();
                    emit siftSuccessful();
                } else {//未找到
                    this->close();
                    emit findVoid();
                }
            }
        } else { //专业不为空
            if (ui->classEdit->text().isEmpty()) { //班级为空
                //学院、专业
                int ret = find_stu_major(head, ui->majorSelect->currentText().toUtf8().data());
                findListHead = NULL;
                if (ret != 0) {
                    findListHead = build_find_result();
                    this->close();
                    emit siftSuccessful();
                } else {//未找到
                    this->close();
                    emit findVoid();
                }
            } else {
                //学院、专业、班级
                int ret = find_stu_class(head, ui->collegeSelect->currentText().toUtf8().data(), ui->classEdit->text().toInt(), ui->majorSelect->currentText().toUtf8().data());
                findListHead = NULL;
                if (ret != 0) {
                    findListHead = build_find_result();
                    this->close();
                    emit siftSuccessful();
                } else {//未找到
                    this->close();
                    emit findVoid();
                }
            }
        }
    });
    //取消
    connect(ui->cancelBtn, &QPushButton::clicked, [ = ]() {
        this->close();
        deleteLater();
    });
}

SiftStudentDialog::~SiftStudentDialog() {
    delete ui;
}

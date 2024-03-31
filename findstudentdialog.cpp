#include "findstudentdialog.h"
#include "ui_findstudentdialog.h"
#include "globalVar.h"

FindStudentDialog::FindStudentDialog(QWidget *parent, stu_list *studentList) :
    QDialog(parent),
    ui(new Ui::FindStudentDialog) {
    ui->setupUi(this);
    this->ui->lineEdit->setEnabled(false);//选择前禁用
    QRegularExpression regExp("\\d{0,100}"); // 使用正则表达式限制输入为数字
    QValidator *validator = new QRegularExpressionValidator(regExp, this);
    connect(ui->numberRadioButton, &QRadioButton::toggled, [ = ]() {
        ui->lineEdit->clear();
        ui->lineEdit->setValidator(validator);
        this->ui->lineEdit->setEnabled(true);
    });
    connect(ui->nameRadioButton, &QRadioButton::toggled, [ = ]() {
        ui->lineEdit->clear();
        ui->lineEdit->setValidator(NULL);
        this->ui->lineEdit->setEnabled(true);
    });
    //查询
    connect(ui->pushButton, &QPushButton::clicked, [ = ]() {
        int ret = 0;
        if (ui->nameRadioButton->isChecked()) {
            char *name = ui->lineEdit->text().toUtf8().data();
            ret = find_stu_num(studentList, name);
            if (ret > 1);
        } else {
            char *number = ui->lineEdit->text().toUtf8().data();
            ret = find_stu_num(studentList, number);
        }
        if (ret == 1) {

        }
    });
    //返回
    connect(ui->pushButton_2, &QPushButton::clicked, [ = ]() {
        this->close();
        this->deleteLater();
    });
}

FindStudentDialog::~FindStudentDialog() {
    delete ui;
}

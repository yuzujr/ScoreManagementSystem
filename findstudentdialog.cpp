#include "findstudentdialog.h"
#include "ui_findstudentdialog.h"
#include "globalVar.h"

FindStudentDialog::FindStudentDialog(QWidget *parent, stu_list *studentList, char course[]) :
    QDialog(parent),
    ui(new Ui::FindStudentDialog) {
    ui->setupUi(this);
    if (course != nullptr) {
        strcpy(m_course, course);
    }

    //选择前禁用
    this->ui->lineEdit->setEnabled(false);
    // 使用正则表达式限制输入为数字
    QRegularExpression regExp("\\d{0,100}");
    QValidator *validator = new QRegularExpressionValidator(regExp, this);
    //设置只能选择一种查找方式
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
        int ret = 0;//结果个数
        if (ui->nameRadioButton->isChecked()) {
            char *name = ui->lineEdit->text().toUtf8().data();
            ret = find_stu_name(studentList, name);
        } else {
            char *number = ui->lineEdit->text().toUtf8().data();
            ret = find_stu_num(studentList, number);
        }
        if (ret == 0) {
            QMessageBox::information(this, "查找失败", "未找到指定学生");
            return;
        }
        if (isAdmin) { //管理员端：修改学生信息
            AddStudentDialog *retStudentDialog = new AddStudentDialog(this, studentList, true);
            retStudentDialog->setWindowTitle("查找学生");
            retStudentDialog->setModal(true);
            retStudentDialog->findCnt = ret;
            retStudentDialog->setStudent(0);
            retStudentDialog->show();
        } else { //教师端：修改成绩
            AddStudentDialog *retStudentDialog = new AddStudentDialog(this, studentList, true, m_course);
            retStudentDialog->setWindowTitle("查找学生");
            retStudentDialog->setModal(true);
            retStudentDialog->findCnt = ret;
            retStudentDialog->setStudent(0);
            retStudentDialog->show();
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

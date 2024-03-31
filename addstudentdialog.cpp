#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"
#include "addawarddialog.h"
#include "addpaperdialog.h"
#include "globalVar.h"


AddStudentDialog::AddStudentDialog(QWidget *parent, stu_list *studentList, bool isResult) :
    QDialog(parent),
    ui(new Ui::AddStudentDialog) {
    ui->setupUi(this);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->label_10->setVisible(false);
    ui->editBtn->setVisible(false);
    ui->deleteBtn->setVisible(false);
    if (isResult) {
        ui->pushButton->setVisible(true);
        ui->pushButton_2->setVisible(true);
        ui->label_10->setVisible(true);
        ui->editBtn->setVisible(true);
        ui->deleteBtn->setVisible(true);
        set
    }
    //学生初始化
    for (int i = 0; i < 100; i++) {
        newStudent.stu_award[i].award_winner_num = 0; //获奖者数量
        newStudent.stu_award[i].is_extra_credit = 0; //是否加分（分值或0）
        newStudent.stu_award[i].competition_level = NULL; //大赛级别（A /B /C）
    }
    newStudent.stu_award_num = 0; //获奖数量
    for (int i = 0; i < 100; i++) {
        newStudent.stu_paper[i].writer_num = 0; //作者数量
        newStudent.stu_paper[i].paper_extra_credit = 0; //论文加分（分值或零）
    }
    newStudent.stu_paper_num = 0; //论文数量
    newStudent.stu_course_num = 0; //课程数量
    newStudent.stu_grade_point = 0; //GPA
    newStudent.stu_classnum = 0; //班级
    //背景图
    {
        this->backgroundPixmap.load(":/background.jpg");
    }
    //选择框
    //默认学院、专业、课程
    for (int i = 0; colleges[i][0] != '\0' && i < MAX_COLLEGE_COUNT; i++) {
        ui->collegeSelect->addItem(colleges[i]);
    }
    for (int i = 0; majors[0][i][0] != '\0' && i < MAX_MAJOR_PER_COLLEGE; i++) {
        ui->majorSelect->addItem(majors[0][i]);

    }
    for (int i = 0; majorCourses[0][0][i] != 0 && i < MAX_COURSES_PER_MAJOR; i++) {
        ui->courseSelect->addItem(allCourses[majorCourses[0][0][i] - 1]);
    }
    //学院改变后，更改专业和课程
    connect(ui->collegeSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        ui->majorSelect->clear();
        for (int i = 0; majors[index][i][0] != '\0' && i < MAX_MAJOR_PER_COLLEGE; i++) {
            ui->majorSelect->addItem(majors[index][i]);
        }
        ui->courseSelect->clear();
        for (int i = 0; majorCourses[index][0][i] != 0 && i < MAX_COURSES_PER_MAJOR; i++) {
            ui->courseSelect->addItem(allCourses[majorCourses[index][0][i] - 1]);
        }
    });
    //专业改变后，更改课程
    connect(ui->majorSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        ui->courseSelect->clear();
        for (int i = 0; majorCourses[ui->collegeSelect->currentIndex()][index][i] != 0 && i < MAX_COURSES_PER_MAJOR; i++) {
            ui->courseSelect->addItem(allCourses[majorCourses[ui->collegeSelect->currentIndex()][index][i] - 1]);
        }
    });

    // 学号、班级设置只能输入数字
    QRegularExpression regExp("\\d{0,100}"); // 使用正则表达式限制输入为数字
    QValidator *validator = new QRegularExpressionValidator(regExp, this);
    ui->numberEdit->setValidator(validator);
    ui->classEdit->setValidator(validator);

    //添加奖项
    connect(ui->addAwardBtn, &QPushButton::clicked, [ = ]() {
        if (ui->awardSelect->count() >= 100) {
            QMessageBox::warning(this, "警告", "最多添加100个奖项！");
            return;
        }
        AddAwardDialog *addAwardDialog = new AddAwardDialog(this);
        addAwardDialog->move(this->x() + 100, this->y() + 75);
        addAwardDialog->setModal(1);
        addAwardDialog->show();
        //成功添加
        connect(addAwardDialog, &AddAwardDialog::addAwardSuccessful, [ = ]() {
            ui->awardSelect->addItem(addAwardDialog->newAward.award_name);
            newStudent.stu_award[newStudent.stu_award_num] = addAwardDialog->newAward;
            ui->awardSelect->setCurrentIndex(newStudent.stu_award_num);
            newStudent.stu_award_num++;
        });
    });

    //删除奖项
    connect(ui->deleteAwardBtn, &QPushButton::clicked, [ = ]() {
        //删除第99个
        if (ui->awardSelect->currentIndex() == 99) {
            Award emptyAward;
            newStudent.stu_award[99] = emptyAward;
        }
        //其余情况
        for (int i = ui->awardSelect->currentIndex(); i < newStudent.stu_award_num && i + 1 < 100; i++) {
            if (ui->awardSelect->currentIndex() == -1) {
                return;
            }
            newStudent.stu_award[i] = newStudent.stu_award[i + 1];
        }
        //更新数量
        newStudent.stu_award_num--;
        //删去ui中的对应奖项
        ui->awardSelect->removeItem(ui->awardSelect->currentIndex());
    });

    //编辑奖项
    connect(ui->editAwardBtn, &QPushButton::clicked, [ = ]() {
        if (ui->awardSelect->count() == 0) {
            return;
        }
        //获取当前选中的奖项
        Award oldAward = newStudent.stu_award[ui->awardSelect->currentIndex()];
        //基于"新建奖项"对话框，将信息填入，等待修改
        AddAwardDialog *editAwardDialog = new AddAwardDialog(this);
        editAwardDialog->setFocusPolicy(Qt::NoFocus);
        editAwardDialog->move(this->x() + 100, this->y() + 75);
        editAwardDialog->setModal(1);
        editAwardDialog->setAward(oldAward);
        editAwardDialog->show();
        //成功添加
        connect(editAwardDialog, &AddAwardDialog::addAwardSuccessful, [ = ]() {
            //更新奖项
            newStudent.stu_award[ui->awardSelect->currentIndex()] = editAwardDialog->newAward;
            //更新awardSelect的对应名称
            ui->awardSelect->setItemText(ui->awardSelect->currentIndex(), QString::fromUtf8(editAwardDialog->newAward.award_name));
        });
    });
    //添加论文
    connect(ui->addPaperBtn, &QPushButton::clicked, [ = ]() {
        if (ui->paperSelect->count() >= 100) {
            QMessageBox::warning(this, "警告", "最多添加100个奖项！");
            return;
        }
        AddPaperDialog *addPaperDialog = new AddPaperDialog(this);
        addPaperDialog->move(this->x() + 100, this->y() + 75);
        addPaperDialog->setModal(1);
        addPaperDialog->show();
        //成功添加
        connect(addPaperDialog, &AddPaperDialog::addPaperSuccessful, [ = ]() {
            ui->paperSelect->addItem(addPaperDialog->newPaper.paper_name);
            newStudent.stu_paper[newStudent.stu_paper_num] = addPaperDialog->newPaper;
            ui->paperSelect->setCurrentIndex(newStudent.stu_paper_num);
            newStudent.stu_paper_num++;
        });
    });

    //删除论文
    connect(ui->deletePaperBtn, &QPushButton::clicked, [ = ]() {
        //删除第99个
        if (ui->paperSelect->currentIndex() == 99) {
            Paper emptyPaper;
            newStudent.stu_paper[99] = emptyPaper;
        }
        //其余情况
        for (int i = ui->paperSelect->currentIndex(); i < newStudent.stu_paper_num && i + 1 < 100; i++) {
            if (ui->paperSelect->currentIndex() == -1) {
                return;
            }
            newStudent.stu_paper[i] = newStudent.stu_paper[i + 1];
        }
        //更新数量
        newStudent.stu_paper_num--;
        //删去ui中的对应奖项
        ui->paperSelect->removeItem(ui->paperSelect->currentIndex());
    });

    //编辑论文
    connect(ui->editPaperBtn, &QPushButton::clicked, [ = ]() {
        if (ui->paperSelect->count() == 0) {
            return;
        }
        //获取当前选中的论文
        Paper oldPaper = newStudent.stu_paper[ui->paperSelect->currentIndex()];
        //基于"新建论文"对话框，将信息填入，等待修改
        AddPaperDialog *editPaperDialog = new AddPaperDialog(this);
        editPaperDialog->setFocusPolicy(Qt::NoFocus);
        editPaperDialog->move(this->x() + 100, this->y() + 75);
        editPaperDialog->setModal(1);
        editPaperDialog->setPaper(oldPaper);
        editPaperDialog->show();
        //成功添加
        connect(editPaperDialog, &AddPaperDialog::addPaperSuccessful, [ = ]() {
            //更新论文
            newStudent.stu_paper[ui->paperSelect->currentIndex()] = editPaperDialog->newPaper;
            //更新paperSelect的对应名称
            ui->paperSelect->setItemText(ui->paperSelect->currentIndex(), QString::fromUtf8(editPaperDialog->newPaper.paper_name));
        });
    });

    //1.取消添加
    connect(ui->cancelBtn, &QPushButton::clicked, [ = ]() {
        // 弹出对话框询问用户是否取消操作
        QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "确定要取消操作吗？",
                QMessageBox::Yes | QMessageBox::No);
        // 如果用户选择了 "No"，则不关闭对话框
        if (result == QMessageBox::No) {
            return;
        }
        emit addStudentCanceled();
        deleteLater(); // 删除对话框自身
    });
    //2.确认添加
    connect(ui->okBtn, &QPushButton::clicked, [ = ]() {
        //检查必填项是否为空
        if (ui->nameEdit->text().isEmpty() || ui->numberEdit->text().isEmpty() || ui->classEdit->text().isEmpty()) {
            //弹出警告
            QMessageBox::warning(this, "警告", "必填项不得为空！");
            return;
        }
        //检查学号是否重复
        if (find_stu_num(studentList, ui->numberEdit->text().toUtf8().data()) != 0) {
            //弹出警告
            QMessageBox::warning(this, "警告", "学号重复！");
            return;
        }
        // 弹出对话框询问用户是否确认操作
        QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "是否确认操作？",
                QMessageBox::Yes | QMessageBox::No);
        // 如果用户选择了 "No"，则不关闭对话框
        if (result == QMessageBox::No) {
            return;
        }
        //读入其他信息
        //学号
        strcpy(newStudent.stu_number, ui->numberEdit->text().toUtf8().data());
        //姓名
        strcpy(newStudent.stu_name, ui->nameEdit->text().toUtf8().data());
        //学院
        strcpy(newStudent.stu_college, ui->collegeSelect->itemText(ui->collegeSelect->currentIndex()).toUtf8().data());
        //专业
        strcpy(newStudent.stu_major, ui->majorSelect->itemText(ui->majorSelect->currentIndex()).toUtf8().data());
        //班级
        newStudent.stu_classnum = ui->classEdit->text().toInt();
        //所选课程编号
        int courseNum = 0;
        for (; majorCourses[ui->collegeSelect->currentIndex()][ui->majorSelect->currentIndex()][courseNum] != 0 && courseNum < MAX_COURSES_PER_MAJOR; courseNum++) {
            newStudent.stu_course_grade[courseNum][0] = majorCourses[ui->collegeSelect->currentIndex()][ui->majorSelect->currentIndex()][courseNum] - 1;
            newStudent.stu_course_grade[courseNum][1] = 0; //成绩为0
            newStudent.stu_course_grade[courseNum][2] = 0; //绩点为0
        }
        newStudent.stu_course_num = courseNum;
        //密码(默认设为jlu@学号)
        newStudent.stu_password[sizeof(newStudent.stu_password) - 1] = '\0';
        strcat(newStudent.stu_password, "jlu@");
        strcat(newStudent.stu_password, newStudent.stu_number);
        emit addStudentSuccessful();
        QMessageBox::information(this, "提示", "添加成功！");
    });
}

void AddStudentDialog::paintEvent(QPaintEvent *event) {
    //创建主界面
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), this->backgroundPixmap);
}

void AddStudentDialog::setStudent(Student findStudent) {
    ui->nameEdit->setText(QString::fromUtf8(findStudent.stu_name));
}

AddStudentDialog::~AddStudentDialog() {
    delete ui;
}

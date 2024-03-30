#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"
#include "addawarddialog.h"
#include "addpaperdialog.h"
#include "globalVar.h"

AddStudentDialog::AddStudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudentDialog) {
    ui->setupUi(this);
    //背景图
    this->backgroundPixmap.load(":/background.jpg");
    //选择框
    for (int i = 0; colleges[i][0] != '\0' && i < MAX_COLLEGE_COUNT; i++) {
        ui->collegeSelect->addItem(colleges[i]);
    }
    for (int i = 0; majors[0][i][0] != '\0' && i < MAX_MAJOR_PER_COLLEGE; i++) {
        ui->majorSelect->addItem(majors[0][i]);

    }
    for (int i = 0; majorCourses[0][0][i] != '\0' && i < MAX_COURSES_PER_MAJOR; i++) {
        ui->courseSelect->addItem(allCourses[int(majorCourses[0][0][i] - '0')]);
    }//默认学院、专业、课程
    connect(ui->collegeSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        ui->majorSelect->clear();
        for (int i = 0; majors[index][i][0] != '\0' && i < MAX_MAJOR_PER_COLLEGE; i++) {
            ui->majorSelect->addItem(majors[index][i]);
        }
        ui->courseSelect->clear();
        for (int i = 0; majorCourses[index][0][i] != '\0' && i < MAX_COURSES_PER_MAJOR; i++) {
            ui->courseSelect->addItem(allCourses[int(majorCourses[index][0][i] - '0')]);
        }
    });//学院改变后，更改专业和课程
    connect(ui->majorSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        ui->courseSelect->clear();
        for (int i = 0; majorCourses[ui->collegeSelect->currentIndex()][index][i] != '\0' && i < MAX_COURSES_PER_MAJOR; i++) {
            ui->courseSelect->addItem(allCourses[int(majorCourses[ui->collegeSelect->currentIndex()][index][i] - '0')]);
        }
    });//专业改变后，更改课程

    newStudent.stu_award_num = 0;
    newStudent.stu_paper_num = 0;
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
            newStudent.stu_award_num++;
        });
    });
    //删除奖项
    connect(ui->deleteAwardBtn, &QPushButton::clicked, [ = ]() {
        ui->awardSelect->removeItem(ui->awardSelect->currentIndex());
        for (int i = ui->awardSelect->currentIndex(); i < newStudent.stu_award_num && i + 1 < 100; i++) {
            newStudent.stu_award[i] = newStudent.stu_award[i + 1];
        }
        newStudent.stu_award_num--;
    });

    //编辑奖项

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
            newStudent.stu_paper_num++;
        });
    });
    //删除论文
    connect(ui->deletePaperBtn, &QPushButton::clicked, [ = ]() {
        ui->paperSelect->removeItem(ui->paperSelect->currentIndex());
        for (int i = ui->paperSelect->currentIndex(); i < newStudent.stu_paper_num && i + 1 < 100; i++) {
            newStudent.stu_paper[i] = newStudent.stu_paper[i + 1];
        }
        newStudent.stu_paper_num--;
    });
    //编辑论文
}

void AddStudentDialog::paintEvent(QPaintEvent *event) {
    //创建主界面
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), this->backgroundPixmap);
}

AddStudentDialog::~AddStudentDialog() {
    delete ui;
}

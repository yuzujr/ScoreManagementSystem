#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"

const int MAX_COLLEGE_COUNT = 100;
const int MAX_MAJOR_PER_COLLEGE = 10;
const int MAX_NAME_LENGTH = 100;

// 初始化 college 数组
char colleges[MAX_COLLEGE_COUNT][MAX_NAME_LENGTH] = {
    "计算机科学与技术学院",
    "电子工程学院",
    "化学与化工学院"
    // 其他学院名称可以继续添加
};

// 初始化 major 数组
char majors[MAX_COLLEGE_COUNT][MAX_MAJOR_PER_COLLEGE][MAX_NAME_LENGTH] = {
    // 计算机学院的专业
    {
        "计算机科学与技术",
        "软件工程",
        "物联网工程"
        // 其他专业名称可以继续添加
    },
    // 电子工程学院的专业
    {
        "电子信息工程",
        "通信工程"
        // 其他专业名称可以继续添加
    },
    // 化学与化工学院的专业
    {
        "化学",
        "化学工程"
        // 其他专业名称可以继续添加
    }
    // 其他学院的专业信息可以继续添加
};

AddStudentDialog::AddStudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStudentDialog) {
    ui->setupUi(this);
    this->backgroundPixmap.load(":/background.jpg");
    for (int i = 0; colleges[i][0] != '\0' && i < MAX_COLLEGE_COUNT; i++) {
        this->ui->collegeSelect->addItem(colleges[i]);
    }
    for (int i = 0; majors[0][i][0] != '\0' && i < MAX_MAJOR_PER_COLLEGE; i++) {
        ui->majorSelect->addItem(majors[0][i]);
    }//默认学院、专业
    connect(ui->collegeSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
        ui->majorSelect->clear();
        for (int i = 0; majors[index][i][0] != '\0' && i < MAX_MAJOR_PER_COLLEGE; i++) {
            ui->majorSelect->addItem(majors[index][i]);
        }
    });
}

void AddStudentDialog::paintEvent(QPaintEvent *event) {
    //创建主界面
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), this->backgroundPixmap);
}

AddStudentDialog::~AddStudentDialog() {
    delete ui;
}

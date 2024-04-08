#include "addstudentdialog.h"
#include "ui_addstudentdialog.h"
#include "addawarddialog.h"
#include "addpaperdialog.h"
#include "globalVar.h"
#include "verifypasswddialog.h"
#include "dataProcess.h"

AddStudentDialog::AddStudentDialog(QWidget *parent, stu_list *studentList, bool isResult, char course[]) :
    QDialog(parent),
    ui(new Ui::AddStudentDialog) {
    ui->setupUi(this);
    if (course != nullptr) {
        strcpy(m_course, course);
    }

    //默认隐藏查找结果的按钮
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->label_10->setVisible(false);
    ui->editBtn->setVisible(false);
    ui->deleteBtn->setVisible(false);

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

    if (isResult) { //如果是查询结果页
        setStudent(0);
        connect(ui->pushButton, &QPushButton::clicked, [ = ]() {
            if (this->studentIndex > 0) {
                setStudent(--studentIndex);
            }
        });//上一页

        connect(ui->pushButton_2, &QPushButton::clicked, [ = ]() {
            if (&find_result[studentIndex + 1]->m_stu != NULL) {
                setStudent(++studentIndex);
            }
        });//下一页

        if (!isAdmin) { //教师端UI
            //禁用删除
            ui->deleteBtn->setVisible(false);
            //查找课程索引
            int index = 0;
            for (; index < ui->courseSelect->count() && strcmp(m_course, ui->courseSelect->itemText(index).toUtf8().data()) != 0; index++) {
                m_index = index;
            }
            //课程选择设置为对应索引，并禁用
            ui->courseSelect->setCurrentIndex(m_index);
            ui->courseSelect->setEnabled(false);
            ui->gradeEdit->setEnabled(false);
        } else {
            ui->gradeEdit->setVisible(false);
        }
    } else {
        ui->gradeEdit->setVisible(false);
    }

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
        Calculate(&newStudent);
        insert_stu(studentList, &newStudent);
        save_data(studentList);
        emit addStudentSuccessful();
    });
    if (isResult) {
        //重连okBtn
        disconnect(ui->okBtn, nullptr, nullptr, nullptr);
        connect(ui->okBtn, &QPushButton::clicked, [ = ]() {
            this->isStudentSelected = true;
            ui->pushButton->setVisible(false);
            ui->pushButton_2->setVisible(false);
            ui->okBtn->setVisible(false);
            ui->label_10->setVisible(false);
            ui->editBtn->setEnabled(true);
            if (isAdmin) {
                ui->deleteBtn->setEnabled(true);
            }
        });
        //重连cancelBtn
        disconnect(ui->cancelBtn, nullptr, nullptr, nullptr);
        connect(ui->cancelBtn, &QPushButton::clicked, [ = ]() {
            emit addStudentCanceled();
            deleteLater(); // 删除对话框自身
        });
        //删除
        connect(ui->deleteBtn, &QPushButton::clicked, [ = ]() {
            VerifyPasswdDialog *verifyDialog = new VerifyPasswdDialog(this);
            verifyDialog->setModal(true);
            verifyDialog->show();
            connect(verifyDialog, &VerifyPasswdDialog::verifySuccessful, [ = ]() {
                // 执行删除操作
                delete_stu(find_result[studentIndex]);
                save_data(studentList);
                QMessageBox::information(this, "提示", "删除成功！");
                this->close();
                deleteLater();
            });
        });
        //编辑
        connect(ui->editBtn, &QPushButton::clicked, [ = ]() {
            ui->editBtn->setEnabled(true);
            ui->cancelBtn->setText("取消");
            ui->okBtn->setVisible(true);
            if (isAdmin) {
                ui->nameEdit->setEnabled(true);
                ui->numberEdit->setEnabled(true);
                ui->classEdit->setEnabled(true);
                ui->collegeSelect->setEnabled(true);
                ui->majorSelect->setEnabled(true);
                ui->courseSelect->setEnabled(true);
                ui->awardSelect->setEnabled(true);
                ui->paperSelect->setEnabled(true);
                ui->addAwardBtn->setEnabled(true);
                ui->deleteAwardBtn->setEnabled(true);
                ui->editAwardBtn->setEnabled(true);
                ui->addPaperBtn->setEnabled(true);
                ui->deletePaperBtn->setEnabled(true);
                ui->editPaperBtn->setEnabled(true);
                ui->deleteBtn->setEnabled(true);
            } else {
                ui->gradeEdit->setEnabled(true);
            }
            //重连cancelBtn
            disconnect(ui->cancelBtn, nullptr, nullptr, nullptr);
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
            //重连okBtn
            disconnect(ui->okBtn, nullptr, nullptr, nullptr);
            connect(ui->okBtn, &QPushButton::clicked, [ = ]() {
                editStudent(studentList);
            });
        });
    }
}

void AddStudentDialog::paintEvent(QPaintEvent *event) {
    //创建主界面
    QPainter painter(this);
    painter.drawPixmap(0, 0, this->width() * 1.2, this->height(), this->backgroundPixmap);
}

void AddStudentDialog::setStudent(int studentIndex) {
    if (studentIndex == 0) {
        ui->pushButton->setVisible(true);
        ui->pushButton_2->setVisible(true);
        ui->label_10->setVisible(true);
        ui->editBtn->setVisible(true);
        if (isAdmin) {
            ui->deleteBtn->setVisible(true);
        }
        ui->nameEdit->setEnabled(false);
        ui->numberEdit->setEnabled(false);
        ui->classEdit->setEnabled(false);
        ui->collegeSelect->setEnabled(false);
        ui->majorSelect->setEnabled(false);
        ui->courseSelect->setEnabled(false);
        ui->awardSelect->setEnabled(false);
        ui->paperSelect->setEnabled(false);
        ui->addAwardBtn->setEnabled(false);
        ui->deleteAwardBtn->setEnabled(false);
        ui->editAwardBtn->setEnabled(false);
        ui->addPaperBtn->setEnabled(false);
        ui->deletePaperBtn->setEnabled(false);
        ui->editPaperBtn->setEnabled(false);
        ui->deleteBtn->setEnabled(false);
        ui->editBtn->setEnabled(false);
        ui->cancelBtn->setText("返回");
    }//先让用户选择要查找的学生
    newStudent = find_result[studentIndex]->m_stu;//更新newStudent
    //显示信息
    QString cntInfo = "第" + QString::number(studentIndex + 1) + "个  " + "共找到" + QString::number(findCnt) + "个";
    QString selectInfo = "  确认要查找的学生后点击确认";
    QString labelText = cntInfo + selectInfo;
    ui->label_10->setText(labelText);
    ui->nameEdit->setText(QString::fromUtf8(newStudent.stu_name));
    ui->numberEdit->setText(QString::fromUtf8(newStudent.stu_number));
    ui->classEdit->setText(QString::number(newStudent.stu_classnum));
    int collegeIndex = findCollegeIndex(newStudent.stu_college);
    ui->collegeSelect->setCurrentIndex(collegeIndex);
    ui->majorSelect->setCurrentIndex(findMajorIndex(collegeIndex, (&find_result[studentIndex]->m_stu)->stu_major));
    if (!isAdmin) {//保证切换后仍是特定课程
        //查找课程索引
        int index = 0;
        for (; index < ui->courseSelect->count() && strcmp(m_course, ui->courseSelect->itemText(index).toUtf8().data()) != 0; index++);
        m_index = index;
        //课程选择设置为对应索引，并禁用
        ui->courseSelect->setCurrentIndex(m_index);
        ui->courseSelect->setEnabled(false);
        //填入成绩
        ui->gradeEdit->setText(QString::number(newStudent.stu_course_grade[m_index][1]));
    }
    //添加奖项、论文
    ui->awardSelect->clear();//防止重复调用后显示多余信息
    for (int i = 0; newStudent.stu_award[i].competition_level != NULL; i++) {
        ui->awardSelect->addItem(newStudent.stu_award[i].award_name);
    }
    ui->paperSelect->clear();
    for (int i = 0; newStudent.stu_paper[i].paper_name[0] != '\0'; i++) {
        ui->paperSelect->addItem(newStudent.stu_paper[i].paper_name);
    }
}

void AddStudentDialog::editStudent(stu_list *studentList) {
    stu_list *confirmedStudent = find_result[studentIndex];
    //检查必填项是否为空
    if (ui->nameEdit->text().isEmpty() || ui->numberEdit->text().isEmpty() || ui->classEdit->text().isEmpty()) {
        //弹出警告
        QMessageBox::warning(this, "警告", "必填项不得为空！");
        return;
    }
    //检查学号是否重复
    if (find_stu_num(studentList, ui->numberEdit->text().toUtf8().data()) != 0
        && strcmp(confirmedStudent->m_stu.stu_number, ui->numberEdit->text().toUtf8().data()) != 0) {
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
        newStudent.stu_course_grade[courseNum][0] = majorCourses[ui->collegeSelect->currentIndex()][ui->majorSelect->currentIndex()][courseNum];
    }
    newStudent.stu_course_num = courseNum;
    if (!isAdmin) {
        newStudent.stu_course_grade[m_index][1] = ui->gradeEdit->text().toDouble();
    }
    strcpy(confirmedStudent->m_stu.stu_number, newStudent.stu_number);
    strcpy(confirmedStudent->m_stu.stu_password, newStudent.stu_password);
    strcpy(confirmedStudent->m_stu.stu_name, newStudent.stu_name);
    strcpy(confirmedStudent->m_stu.stu_college, newStudent.stu_college);
    strcpy(confirmedStudent->m_stu.stu_major, newStudent.stu_major);
    confirmedStudent->m_stu.stu_award_num = newStudent.stu_award_num;
    for (int i = 0; i < newStudent.stu_award_num && i < 100; ++i) {
        confirmedStudent->m_stu.stu_award[i] = newStudent.stu_award[i];
    }
    confirmedStudent->m_stu.stu_paper_num = newStudent.stu_paper_num;
    for (int i = 0; i < newStudent.stu_paper_num && i < 100; ++i) {
        confirmedStudent->m_stu.stu_paper[i] = newStudent.stu_paper[i];
    }
    confirmedStudent->m_stu.stu_course_num = newStudent.stu_course_num;
    for (int i = 0; i < newStudent.stu_course_num && i < 100; ++i) {
        for (int j = 0; j < 3; ++j) {
            confirmedStudent->m_stu.stu_course_grade[i][j] = newStudent.stu_course_grade[i][j];
        }
    }
    confirmedStudent->m_stu.stu_grade_point = newStudent.stu_grade_point;
    confirmedStudent->m_stu.stu_classnum = newStudent.stu_classnum;
    //读入新成绩
    if (!isAdmin) {
        confirmedStudent->m_stu.stu_course_grade[m_index][1] = newStudent.stu_course_grade[m_index][1];
        confirmedStudent->m_stu.stu_course_grade[m_index][2] = updateCredit(confirmedStudent->m_stu.stu_course_grade[m_index][1]);
    }
    //更新GPA
    Calculate(&confirmedStudent->m_stu);
    //替换原学生
    save_data(studentList);//保存数据
    load_data(studentList);//加载数据
    this->close();
    this->deleteLater();
    QMessageBox::information(this, "提示", "修改成功！");
}

void AddStudentDialog::Calculate(Student *a) {
    double gpa = 0;
    double sum = 0; //课程绩点影响的gpa计算记录
    double max;//取各奖项中贡献（不超过0.40）（不考虑其他项加分）

    float sum_Young = 0;//暂时记录总学分
    for (int i = 0; i < a->stu_course_num; i++) {
        sum_Young += credits[(int)a->stu_course_grade[i][0]];
    }
    for (int i = 0; i < a->stu_course_num; i++) {
        double g = a->stu_course_grade[i][1];//暂时记录该项成绩
        //分段记录绩点并计算gpa
        if (g >= 90) {
            a->stu_course_grade[i][2] = 4.0;
            sum += 4.0 * credits[(int)a->stu_course_grade[i][0]];
            continue;
        } else if (g >= 85 && g < 90) {
            a->stu_course_grade[i][2] = 3.7;
            sum += 3.7 * credits[(int)a->stu_course_grade[i][0]];
            continue;
        } else if (g >= 82 && g < 85) {
            a->stu_course_grade[i][2] = 3.3;
            sum += 3.3 * credits[(int)a->stu_course_grade[i][0]];
            continue;
        } else if (g >= 78 && g < 82) {
            a->stu_course_grade[i][2] = 3.0;
            sum += 3.0 * credits[(int)a->stu_course_grade[i][0]];
            continue;
        } else if (g >= 75 && g < 78) {
            a->stu_course_grade[i][2] = 2.7;
            sum += 2.7 * credits[(int)a->stu_course_grade[i][0]];
            continue;
        } else if (g >= 72 && g < 75) {
            a->stu_course_grade[i][2] = 2.3;
            sum += 2.3 * credits[(int)a->stu_course_grade[i][0]];
            continue;
        } else if (g >= 68 && g < 72) {
            a->stu_course_grade[i][2] = 2.0;
            sum += 2.0 * credits[(int)a->stu_course_grade[i][0]];
            continue;
        } else if (g >= 64 && g < 68) {
            a->stu_course_grade[i][2] = 1.5;
            sum += 1.5 * credits[(int)a->stu_course_grade[i][0]];
            continue;
        } else if (g >= 60 && g < 64) {
            a->stu_course_grade[i][2] = 1.0;
            sum += 1.0 * credits[(int)a->stu_course_grade[i][0]];
            continue;
        } else {
            a->stu_course_grade[i][2] = 0.0;
            continue;
        }
    }
    gpa = sum / sum_Young;//加权平均计算gpa（课程部分）
    double credit;//总额外加分
    double credit1 = 0.00; //比较出获奖中加分最多的作为该类最终加分
    for (int i = 0; i < a->stu_award_num; i++) {
        if (a->stu_award[i].is_extra_credit > credit) {
            credit1 = a->stu_award[i].is_extra_credit;
        }
    }

    double credit2 = 0.00; //比较出论文中加分最多的作为该类最终加分
    for (int i = 0; i < a->stu_award_num; i++) {
        if (a->stu_paper[i].paper_extra_credit > credit2) {
            credit2 = a->stu_paper[i].paper_extra_credit;
        }
    }
    if (credit1 + credit2 >= 0.4) {
        credit = 0.40;
    } else {
        credit = credit1 + credit2;  //保证额外加分不超过0.4
    }
    gpa += credit;
    a->stu_grade_point = gpa;
}

AddStudentDialog::~AddStudentDialog() {
    delete ui;
}

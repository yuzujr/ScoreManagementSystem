#include "addawarddialog.h"
#include "ui_addawarddialog.h"

AddAwardDialog::AddAwardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAwardDialog) {
    ui->setupUi(this);
    ui->GPA_edit->setEnabled(false);//选“是”之前禁用编辑
    connect(ui->radioButton, &QRadioButton::clicked, [ = ]() {
        ui->GPA_edit->setEnabled(true);
    });//选“是”启用编辑
    connect(ui->radioButton_2, &QRadioButton::clicked, [ = ]() {
        ui->GPA_edit->clear();
        ui->GPA_edit->setEnabled(false);
    });//选“否”禁用编辑
    QDoubleValidator validator(0.0, 4.0, 2); // 设置范围为 -1000.0 到 1000.0，精度为小数点后两位
    validator.setNotation(QDoubleValidator::StandardNotation);// 设置符号允许的范围为 QDoubleValidator::StandardNotation
    validator.setDecimals(2);// 设置小数点后允许的位数为 2
    QValidator *validatorPtr = &validator;
    ui->GPA_edit->setValidator(validatorPtr);//只允许输入浮点数

    //1.取消添加
    connect(ui->cancelBtn, &QPushButton::clicked, [ = ]() {
        // 弹出对话框询问用户是否取消操作
        QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "确定要取消操作吗？",
                QMessageBox::Yes | QMessageBox::No);
        // 如果用户选择了 "No"，则不关闭对话框
        if (result == QMessageBox::No) {
            return;
        }
        emit addAwardCanceled();
        deleteLater(); // 删除对话框自身
    });

    //2.确认添加
    connect(ui->okBtn, &QPushButton::clicked, [ = ]() {
        //检查必填项是否为空
        if (ui->nameEdit->text().isEmpty() || ui->hostEdit->text().isEmpty() || ui->winnerEdit->text().isEmpty() || !ui->radioButton->isChecked()
            && !ui->radioButton_2->isChecked() || ui->radioButton->isChecked() && ui->GPA_edit->text().isEmpty()) {
            //弹出警告
            QMessageBox::warning(this, "警告", "必填项不得为空！");
            return;
        }
        // 检查用户输入是否合法
        if (ui->GPA_edit->hasAcceptableInput()) {
            // 如果输入合法，不做任何操作
        } else {
            // 如果输入不合法，弹出警告
            QMessageBox::warning(this, "警告", "请输入合法的浮点数！");
            // 清空用户输入
            ui->GPA_edit->clear();
            return;
        }
        strcpy(newAward.award_name, ui->nameEdit->text().toUtf8().data());//奖项名称
        strcpy(newAward.award_hosted_by, ui->hostEdit->text().toUtf8().data());//主办方
        QString allWinner = ui->winnerEdit->text();//获奖者
        //拆分获奖者
        QStringList winnners = allWinner.split("；");
        for (int i = 0; i < winnners.size(); ++i) {
            QString winnner = winnners[i];
            strcpy(newAward.award_allwinner[i], winnner.toUtf8().data());
        }
        if (ui->radioButton->isChecked()) {
            newAward.is_extra_credit = ui->GPA_edit->text().toDouble();
        }
        newAward.competition_level = ui->levelSelect->currentText()[0].toLatin1();
        strcpy(newAward.award_time, ui->dateEdit->date().toString("yyyy-MM-dd").toUtf8().data());
        // 弹出对话框询问用户是否确认操作
        QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "是否确认操作？",
                QMessageBox::Yes | QMessageBox::No);
        // 如果用户选择了 "No"，则不关闭对话框
        if (result == QMessageBox::No) {
            return;
        }
        emit addAwardSuccessful();
        deleteLater(); // 删除对话框自身
    });
}

AddAwardDialog::~AddAwardDialog() {
    delete ui;
}

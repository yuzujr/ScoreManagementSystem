#include "addpaperdialog.h"
#include "ui_addpaperdialog.h"

AddPaperDialog::AddPaperDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPaperDialog) {
    ui->setupUi(this);
    newPaper.writer_num = 0; //作者数量
    newPaper.paper_extra_credit = 0; //论文加分（分值或零）
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
    //设置时间格式为2001-01-01
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    // 设置初始日期
    QDate initialDate = QDate::fromString("2024-01-01", "yyyy-MM-dd");
    ui->dateEdit->setDate(initialDate);

    //1.取消添加
    connect(ui->cancelBtn, &QPushButton::clicked, [ = ]() {
        // 弹出对话框询问用户是否取消操作
        QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "确定要取消操作吗？",
                QMessageBox::Yes | QMessageBox::No);
        // 如果用户选择了 "No"，则不关闭对话框
        if (result == QMessageBox::No) {
            return;
        }
        emit addPaperCanceled();
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
        // 弹出对话框询问用户是否确认操作
        QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "是否确认操作？",
                QMessageBox::Yes | QMessageBox::No);
        // 如果用户选择了 "No"，则不关闭对话框
        if (result == QMessageBox::No) {
            return;
        }
        strcpy(newPaper.paper_name, ui->nameEdit->text().toUtf8().data());//论文名称
        strcpy(newPaper.paper_periodicalname, ui->hostEdit->text().toUtf8().data());//期刊名称
        QString allWinner = ui->winnerEdit->text();//作者
        //拆分作者
        newPaper.writer_num = 0;
        QStringList winnners = allWinner.split("；");
        for (int i = 0; i < winnners.size(); ++i) {
            QString winnner = winnners[i];
            strcpy(newPaper.paper_allwriter[i], winnner.toUtf8().data());
            newPaper.writer_num++;
        }
        if (ui->radioButton->isChecked()) {
            newPaper.paper_extra_credit = ui->GPA_edit->text().toDouble();
        }
        strcpy(newPaper.paper_time, ui->dateEdit->date().toString("yyyy-MM-dd").toUtf8().data());
        emit addPaperSuccessful();
        deleteLater(); // 删除对话框自身
    });
}

//设置待修改论文
void AddPaperDialog::setPaper(Paper oldPaper) {
    newPaper = oldPaper;
    //论文名称
    ui->nameEdit->setText(QString::fromUtf8(newPaper.paper_name));
    //期刊名称
    ui->hostEdit->setText(QString::fromUtf8(newPaper.paper_periodicalname));
    //所有作者
    QString winnersText;
    for (int i = 0; i < newPaper.writer_num; i++) {
        if (!winnersText.isEmpty()) {
            winnersText += "；";
        }
        winnersText += QString::fromUtf8(newPaper.paper_allwriter[i]);
    }
    ui->winnerEdit->setText(winnersText);
    //是否加分
    if (newPaper.paper_extra_credit == 0) { //不加分
        ui->radioButton_2->setChecked(true);
    } else { //加分
        ui->radioButton->setChecked(true);
        ui->GPA_edit->setEnabled(true);
        ui->GPA_edit->setText(QString::number(newPaper.paper_extra_credit));
    }
    //获奖日期
    QDate date = QDate::fromString(QString::fromUtf8(newPaper.paper_time), "yyyy-MM-dd");
    ui->dateEdit->setDate(date);
}

AddPaperDialog::~AddPaperDialog() {
    delete ui;
}

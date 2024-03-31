#include "projecttable.h"
#include "ui_projecttable.h"
#include <QFont>

ProjectTable::ProjectTable(QWidget *parent, Student *stu)
    : QWidget(parent)
    , ui(new Ui::ProjectTable) {
    ui->setupUi(this);
    // 设置表格的边框线
    this->ui->tableWidget->setStyleSheet("background-color:transparent;");
    this->ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent;border: 1px solid black; }");
    this->ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: transparent; border: 1px solid black;}");
    //查看论文
    connect(ui->pushButton, &QPushButton::clicked, [ = ]() {
        m_isPaper = true;
        emit paperClicked();
        clearTable();
        printPaper(stu, 1);
    });
    //查看奖项
    connect(ui->pushButton_2, &QPushButton::clicked, [ = ]() {
        m_isPaper = false;
        emit awardClicked();
        clearTable();
        printAward(stu, 1);
    });
}

void ProjectTable::clearTable() {
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);
    for (int row = 0; row < this->ui->tableWidget->rowCount(); ++row) {
        for (int col = 0; col < this->ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = this->ui->tableWidget->item(row, col);
            if (item) {
                delete item;
                this->ui->tableWidget->setItem(row, col, nullptr);
            }
        }
    }
}
void ProjectTable::printPaper(Student *stu, int page) {
    if (stu->stu_paper_num == 0) {
        this->ui->tableWidget->setRowCount(0);
        this->ui->tableWidget->setColumnCount(0);
        pixmapLabel.setParent(this->ui->tableWidget);
        pixmapLabel.setPixmap(this->NonePixmap);
        pixmapLabel.resize(this->NonePixmap.size());
        pixmapLabel.move((this->width() - NonePixmap.width()) / 2, 100);
        pixmapLabel.setStyleSheet("border: none;");
        pixmapLabel.show();
        QLabel *info = new QLabel("这里还什么都没有哦", this->ui->tableWidget);
        info->move((this->width() - info->width()) / 2 - 33, 300);
        info->setStyleSheet("border: none; font: 14pt 宋体;");
        info->show();
        connect(this, &ProjectTable::awardClicked, [ = ]() {
            info->hide();
            pixmapLabel.hide();
        });
        return;
    }//论文为空
    QFont font("宋体", 12); // 设置字体为宋体，大小为12号
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setVerticalHeaderLabels({"论文名称", "作者", "发表期刊", "发表时间", "是否加分"});
    // 设置表格部件的大小策略，使其填充整个表格框
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 设置单元格的伸展策略，使其填充整个表格框
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = new QTableWidgetItem("");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable); // 禁止编辑
        item->setFont(font);
        ui->tableWidget->setItem(row, 0, item);
    }
    //论文名称
    ui->tableWidget->item(0, 0)->setText(QString::fromUtf8(stu->stu_paper[page - 1].paper_name));
    //所有作者
    QString allWriter;
    for (int i = 0; i < stu->stu_paper[page - 1].writer_num; i++) {
        if (i == 0 && stu->stu_paper[page - 1].paper_allwriter[i][0] == '@') {
            allWriter += "通讯作者：";
            char writer[100];
            for (int i = 0; i + 1 < strlen(stu->stu_paper[page - 1].paper_allwriter[0]); i++) {
                writer[i] = stu->stu_paper[page - 1].paper_allwriter[0][i + 1];
            }
            allWriter += QString::fromUtf8(writer) + ";  ";
        } else {
            allWriter += QString::fromUtf8(stu->stu_paper[page - 1].paper_allwriter[i]);
            allWriter += "  ";
        }
    }
    ui->tableWidget->item(1, 0)->setText(allWriter);
    //期刊名称
    ui->tableWidget->item(2, 0)->setText(QString::fromUtf8(stu->stu_paper[page - 1].paper_periodicalname));
    //发表时间
    ui->tableWidget->item(3, 0)->setText(QString::fromUtf8(stu->stu_paper[page - 1].paper_time));
    //论文加分
    ui->tableWidget->item(4, 0)->setText(QString::number(stu->stu_paper[page - 1].paper_extra_credit));
}
void ProjectTable::printAward(Student *stu, int page) {
    if (stu->stu_award_num == 0) {
        this->ui->tableWidget->setRowCount(0);
        this->ui->tableWidget->setColumnCount(0);
        pixmapLabel.setParent(this->ui->tableWidget);
        pixmapLabel.setPixmap(this->NonePixmap);
        pixmapLabel.resize(this->NonePixmap.size());
        pixmapLabel.move((this->width() - NonePixmap.width()) / 2, 100);
        pixmapLabel.setStyleSheet("border: none;");
        pixmapLabel.show();
        QLabel *info = new QLabel("这里还什么都没有哦", this->ui->tableWidget);
        info->move((this->width() - info->width()) / 2 - 33, 300);
        info->setStyleSheet("border: none; font: 14pt 宋体;");
        info->show();
        connect(this, &ProjectTable::paperClicked, [ = ]() {
            info->hide();
            pixmapLabel.hide();
        });
        return;
    }//奖项为空
    QFont font("宋体", 12); // 设置字体为宋体，大小为12号
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setVerticalHeaderLabels({"奖项名称", "获奖者", "主办方名称", "获奖时间", "奖项加分"});
    // 设置表格部件的大小策略，使其填充整个表格框
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 设置单元格的伸展策略，使其填充整个表格框
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = new QTableWidgetItem("");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable); // 禁止编辑
        item->setFont(font);
        ui->tableWidget->setItem(row, 0, item);
        //ui->tableWidget->item(row, 0)->setFlags(ui->tableWidget->item(row, 0)->flags() & ~Qt::ItemIsSelectable); // 禁止选中
    }

    //奖项名称
    ui->tableWidget->item(0, 0)->setText(QString::fromUtf8(stu->stu_award[page - 1].award_name));
    //所有获奖者
    QString allWinner;
    for (int i = 0; i < stu->stu_award[page - 1].award_winner_num; i++) {
        allWinner += QString::fromUtf8(stu->stu_award[page - 1].award_allwinner[i]);
        allWinner += "  ";
    }
    ui->tableWidget->item(1, 0)->setText(allWinner);
    //主办方名称
    ui->tableWidget->item(2, 0)->setText(QString::fromUtf8(stu->stu_award[page - 1].award_hosted_by));
    //获奖时间
    ui->tableWidget->item(3, 0)->setText(QString::fromUtf8(stu->stu_award[page - 1].award_time));
    //奖项加分
    ui->tableWidget->item(4, 0)->setText(QString::number(stu->stu_award[page - 1].is_extra_credit));
}

ProjectTable::~ProjectTable() {
    delete ui;
}

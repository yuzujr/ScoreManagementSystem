#include "allscoretable.h"
#include "ui_allscoretable.h"

AllScoreTable::AllScoreTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllScoreTable)
{
    ui->setupUi(this);
}

AllScoreTable::~AllScoreTable()
{
    delete ui;
}

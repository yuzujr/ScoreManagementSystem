#include "addpaperdialog.h"
#include "ui_addpaperdialog.h"

AddPaperDialog::AddPaperDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPaperDialog)
{
    ui->setupUi(this);
}

AddPaperDialog::~AddPaperDialog()
{
    delete ui;
}

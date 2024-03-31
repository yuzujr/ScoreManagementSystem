#ifndef ADDPAPERDIALOG_H
#define ADDPAPERDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QDoubleValidator>
#include <QDate>
#include "student.h"

namespace Ui {
    class AddPaperDialog;
}

class AddPaperDialog : public QDialog {
    Q_OBJECT

  public:
    explicit AddPaperDialog(QWidget *parent = nullptr);
    Paper newPaper;
    void setPaper(Paper oldPaper);//设置待修改论文
    ~AddPaperDialog();

  private:
    Ui::AddPaperDialog *ui;
  signals:
    addPaperSuccessful();
    addPaperCanceled();
};

#endif // ADDPAPERDIALOG_H

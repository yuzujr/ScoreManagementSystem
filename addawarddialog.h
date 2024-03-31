#ifndef ADDAWARDDIALOG_H
#define ADDAWARDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QDoubleValidator>
#include <QDate>
#include "student.h"

namespace Ui {
    class AddAwardDialog;
}

class AddAwardDialog : public QDialog {
    Q_OBJECT

  public:
    explicit AddAwardDialog(QWidget *parent = nullptr);
    Award newAward;
    void setAward(Award oldAward);//设置待修改奖项
    ~AddAwardDialog();

  private:
    Ui::AddAwardDialog *ui;
  signals:
    addAwardSuccessful();
    addAwardCanceled();
};

#endif // ADDAWARDDIALOG_H

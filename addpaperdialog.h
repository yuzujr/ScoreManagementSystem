#ifndef ADDPAPERDIALOG_H
#define ADDPAPERDIALOG_H

#include <QDialog>
#include "student.h"

namespace Ui {
    class AddPaperDialog;
}

class AddPaperDialog : public QDialog {
    Q_OBJECT

  public:
    explicit AddPaperDialog(QWidget *parent = nullptr);
    Paper newPaper;
    ~AddPaperDialog();

  private:
    Ui::AddPaperDialog *ui;
  signals:
    addPaperSuccessful();
    addPaperCanceled();
};

#endif // ADDPAPERDIALOG_H

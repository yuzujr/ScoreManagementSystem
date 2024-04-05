#ifndef VERIFYPASSWDDIALOG_H
#define VERIFYPASSWDDIALOG_H

#include <QDialog>

namespace Ui {
    class VerifyPasswdDialog;
}

class VerifyPasswdDialog : public QDialog {
    Q_OBJECT

  public:
    explicit VerifyPasswdDialog(QWidget *parent = nullptr);
    ~VerifyPasswdDialog();

  private:
    Ui::VerifyPasswdDialog *ui;

  signals:
    verifySuccessful();

};

#endif // VERIFYPASSWDDIALOG_H

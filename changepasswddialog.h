#ifndef CHANGEPASSWDDIALOG_H
#define CHANGEPASSWDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QMessageBox>

namespace Ui {
    class changePasswdDialog;
}

class changePasswdDialog : public QDialog {
    Q_OBJECT

public:
    explicit changePasswdDialog(QWidget *parent = nullptr);
    ~changePasswdDialog();

private:
    Ui::changePasswdDialog *ui;
};

#endif // CHANGEPASSWDDIALOG_H

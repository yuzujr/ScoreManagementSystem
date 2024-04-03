#ifndef ALLSCORETABLE_H
#define ALLSCORETABLE_H

#include <QWidget>

namespace Ui {
class AllScoreTable;
}

class AllScoreTable : public QWidget
{
    Q_OBJECT

public:
    explicit AllScoreTable(QWidget *parent = nullptr);
    ~AllScoreTable();

private:
    Ui::AllScoreTable *ui;
};

#endif // ALLSCORETABLE_H

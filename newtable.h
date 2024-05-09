#ifndef NEWTABLE_H
#define NEWTABLE_H

#include <QDialog>

namespace Ui {
class NewTable;
}

class NewTable : public QDialog
{
    Q_OBJECT

public:
    explicit NewTable(QWidget *parent = nullptr);
    ~NewTable();

private:
    Ui::NewTable *ui;
};

#endif // NEWTABLE_H

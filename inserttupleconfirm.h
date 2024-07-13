#ifndef INSERTTUPLECONFIRM_H
#define INSERTTUPLECONFIRM_H

#include <QDialog>

namespace Ui {
class insertTupleConfirm;
}

class insertTupleConfirm : public QDialog
{
    Q_OBJECT

public:
    explicit insertTupleConfirm(QWidget *parent = nullptr);
    ~insertTupleConfirm();

private:
    Ui::insertTupleConfirm *ui;
};

#endif // INSERTTUPLECONFIRM_H

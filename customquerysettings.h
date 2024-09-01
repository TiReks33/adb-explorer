#ifndef CUSTOMQUERYSETTINGS_H
#define CUSTOMQUERYSETTINGS_H

#include <QDialog>

namespace Ui {
class CustomQuerySettings;
}

class CustomQuerySettings : public QDialog
{
    Q_OBJECT

public:
    explicit CustomQuerySettings(QWidget *parent = nullptr);
    ~CustomQuerySettings();



    friend class Tables;



private:
    Ui::CustomQuerySettings *ui;
};

#endif // CUSTOMQUERYSETTINGS_H

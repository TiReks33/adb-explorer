#ifndef CUSTOMQUERYSETTINGS_H
#define CUSTOMQUERYSETTINGS_H

#include <QDialog>
#include <QMap>
#include <QPushButton>

#include "adb-exp-utility.h"
#include "auth.h"


namespace Ui {
class CustomQuerySettings;
}

class CustomQuerySettings : public QDialog
{
    Q_OBJECT

public:
    explicit CustomQuerySettings(QWidget/*Tables*/ *parent = nullptr);
    ~CustomQuerySettings();



    //friend class Tables;

private slots:

    // write settings to file
    void write2query_opts_file();

    // read settings from file
    bool read4rom_query_opts_file();

signals:

    void settings_changed(QMap<QString,int>);

private:
    Ui::CustomQuerySettings *ui;

    QMap<QString,int> __settings_map;

//    Tables* parent_;

    void init_signals();

    void fileOps();

    void formStyle();

    //[
    QString const query_settings_f_name = adb_utility::filepath_+"/query.cfg";

//    bool t_content_wnd = true;

//    bool t_query_wnd = true;

//    bool MSG_SHOW_IF_BLANK_RESULT = false;

    //]


};

#endif // CUSTOMQUERYSETTINGS_H

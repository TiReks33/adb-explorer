#ifndef CUSTOMQUERYSETTINGS_H
#define CUSTOMQUERYSETTINGS_H

#include <QDialog>
#include <QMap>
#include <QPushButton>

#include "adb-exp-utility.h"
#include "auth.h"

enum tblQuerSet{
    separate_content_window,
    separate_describe_window,
    separate_query_window,
    BLANK_RESULT,
    MSG_SHOW_IF_BLANK_RESULT,
    MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL
};

namespace Ui {
class CustomQuerySettings;
}

class CustomQuerySettings : public QDialog
{
    Q_OBJECT

public:
    explicit CustomQuerySettings(QWidget/*Tables*/ *parent = nullptr);
    ~CustomQuerySettings();

    void setForeignSettingsOnForm(QMap<QString,int>); ////

    static QMap<int,QString> paramEnumToStr;


    //friend class Tables;


signals:

    void settingsExportSig(QMap<QString,int>,bool); ////

private:
    Ui::CustomQuerySettings *ui;


//    Tables* parent_;

    void init_signals();

    void formStyle();


    bool changes_ = false;

    void settings2Export(QMap<QString,int>&);

};

#endif // CUSTOMQUERYSETTINGS_H



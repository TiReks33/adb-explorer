#ifndef CUSTOM_QUERY_H
#define CUSTOM_QUERY_H

#include <QDialog>
#include <QDebug>
#include <QFontDialog>

#include "helping_stuff.h"
#include "clickablelabel.h"
#include "auth.h"
#include "fontembeddedwidget.h"


namespace Ui {
class Custom_Query;
}

class Custom_Query : public QDialog
{
    Q_OBJECT

public:
    explicit Custom_Query(QWidget *parent = nullptr, bool closeMessage = false/*, QFont=QFont()*/);
    virtual ~Custom_Query();

    // returns current entered form text
    QString const get_text() const;

    inline bool getCheckCloseMessageFlag(){ return checkCloseMessageFlag_;}

    void setCheckCloseMessageFlag(bool state__);

signals:

    void send_custom_query(QString const&);

    void send_custom_query(/*QString,*/Custom_Query *);

    void dont_show_note();

    void font_changes();

    void closeNowSig();

public slots:
    void close_window();

    // set text to form
    void set_text(QString const&) const;

    // add notification on window
    void add_note(QString const&);

private:
    Ui::Custom_Query *ui;

    void fileOps();

    void connections_init();

    void form_init();

    QPointer<fontEmbeddedWidget> defaultFontWidget_;

    QString const settings_f_name_=adb_utility::filepath_+"/writeQuery.cfg";


    static QString font_;
    static bool settingsFileReady_;

    bool read4rom_settings_file();

    void write2_settings_file();

    void reject();

    static bool askBeforeClose_;
    bool checkCloseMessageFlag_ = false;

};

#endif // CUSTOM_QUERY_H

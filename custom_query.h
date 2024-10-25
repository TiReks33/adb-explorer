#ifndef CUSTOM_QUERY_H
#define CUSTOM_QUERY_H

#include <QDialog>
#include <QDebug>

#include "helping_stuff.h"
#include "clickablelabel.h"

#include "auth.h"

namespace Ui {
class Custom_Query;
}

class Custom_Query : public QDialog
{
    Q_OBJECT

public:
    explicit Custom_Query(QWidget *parent = nullptr, QFont=QFont());
    ~Custom_Query();

    // returns current entered form text
    QString const get_text() const;

signals:

    void send_custom_query(QString const&);

    void send_custom_query(/*QString,*/Custom_Query *);

    void dont_show_note();

    void font_changes();

public slots:
    void close_window();

    // set text to form
    void set_text(QString const&) const;

    // add notification on window
    void add_note(QString const&);

private:
    Ui::Custom_Query *ui;

    QString const settings_f_name=adbexplorer::filepath_+"/font.cfg";

//    QString font_family = "Noto Sans";

//    int font_pointSize = 10;

//    int font_weight = -1;

//    bool font_italic = false;

    QString font_;

    bool read4rom_settings_file();

    void write2_settings_file();


    void connections_init();
};

#endif // CUSTOM_QUERY_H

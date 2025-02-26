#ifndef CUSTOM_QUERY_H
#define CUSTOM_QUERY_H

#include <QDialog>
#include <QDebug>
#include <QFontDialog>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QScrollBar>

#include <QMenuBar>

#include "adb-exp-utility.h"
#include "clickablelabel.h"
#include "auth.h"
#include "fontembeddedwidget.h"

//#include "adbcrypt.h"
#include "iqtplugins.h"

#include "hidemenu.h"
#include "noteframe.h"

#include "dynamicbool.h"

#include "loginwindow.h"

namespace Ui {
class Custom_Query;
}

enum QUERYSTATUS{
    FAILED,
    SUCCESS,
    NOTSENDED
};

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

    static bool removeUserQueriesHistory();

signals:

    void send_custom_query(QString const&);

    void send_custom_query(/*QString,*/Custom_Query *);

    ////void dont_show_note();

    void font_changes();

    void closeNowSig();

public slots:

    // special method to close user query window when query is successful;
    //
    // saveSuccessfulQuery = true -- query text (not result) will be saved in 'query history' form;
    //
    // customTextToAdd -- if not specified (or NULL), full queries text will be saved; if specified,
    // this text (from text variable) will be saved (usefull 4ex. when more than 1 query in form
    // presented at once, and queries list of separated strings is parsed in 'for' cycle)
//    void close_window(bool saveSuccessfulQuery = false, QString const & customTextToAdd = NULL, QUERYSTATUS = QUERYSTATUS::NOTSENDED);

    void save_query(QUERYSTATUS /*= QUERYSTATUS::NOTSENDED*/, QString const & customTextToAdd = NULL);

    // set text to form
    void set_text(QString const&) const;

    // add notification on window
    /*void*/noteFrame* add_note(QString const&);

private slots:

    void showQueriesHistory();

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

    static /*bool*/dynamicbool askBeforeClose_;
    bool checkCloseMessageFlag_ = false;

    QPointer<QPushButton> userQueriesHistoryBut_;
    static int userQueriesHistoryLengthLines_;

    static QString const userQueriesHistoryBinFileName_;
//    friend QString const & loginWindow::getQueriesHistoryFFilePath();
//    friend bool loginWindow::removeQueriesHistoryFile();

    QMenuBar* menuBar_ = nullptr;
    hideMenu* menuFile_ = nullptr;
    QAction* saveQueryEntrie_ = nullptr;
    QAction* exitEntrie_ = nullptr;

    // keyboard keys logic overload
    void keyPressEvent(QKeyEvent *);
};

#endif // CUSTOM_QUERY_H



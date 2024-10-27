#ifndef CUSTOMQUERYRESULT_H
#define CUSTOMQUERYRESULT_H

#include <QDialog>
#include <QCloseEvent>
#include <QClipboard>
#include <QMenu>
#include <QAction>
#include <QScreen>
#include <QCheckBox>
#include <QMap>
#include <QBitmap>

#include "auth.h"
#include "db_connection.h"
#include "signaltableview.h"
#include "fontembeddedwidget.h"
#include "reloadbutton.h"


namespace Ui {
class CustomQueryResult;
}

class CustomQueryResult : public QDialog
{
    Q_OBJECT

public:
    explicit CustomQueryResult(auth& auth__,QWidget *parent = nullptr,bool closeMessage = false);

    virtual ~CustomQueryResult();

    inline bool getCheckCloseMessageFlag(){ return checkCloseMessageFlag_;}

    void setCheckCloseMessageFlag(bool state__);

public slots:

    friend class Tables;
    friend class Databases;

    // general func to display result of query to separate window
    void custom_query_slot(QString const & query__, QString const & conName__ = auth::con_name_/*, QHeaderView::ResizeMode = QHeaderView::Stretch*/);

signals:

    //void closed_();

    void saveSettings();

    void closeNowSig();

private slots:

    void close_window();

    void copySelectionToClipboard();

    void slotCustomMenuRequested(QPoint pos);

    void loadQuery();

private:
    Ui::CustomQueryResult *ui;

    void init_form();

    void init_signals();

    void fileOps();

    void defaultSettings();

    QString const settings_f_name_ = adb_utility::filepath_+"/customquery.cfg";

    bool read4rom_settings_file();

    void write2_settings_file();

    /*signalTableView**/QPointer<signalTableView> tableView/* = nullptr*/;

    /*QPushButton*/reloadButton* reload_button_=nullptr;

    QPushButton* export_button_ = nullptr;
    QPushButton* copy_button_ = nullptr;
    /*QCheckBox**/QPointer<QCheckBox> copyButtonCheckBox_ /*= nullptr*/;
    static bool headerCopy_;


    // rescale stuff
    /*QWidget**/QPointer <QWidget>rescaleBoxWidget /*= nullptr*/;

    QPointer<QCheckBox>rescaleDefaultCheckBox;
    static int defaultScaleIndex_;
    static bool settingsFileReady_;

//public:
    auth& auth_;

    QSqlQueryModel model_;

    QString ClipboardCopyHeadersEntrieText_="Headers off[✓]";

    QString ClipboardCopySeparator_="\t";

    void keyPressEvent(QKeyEvent *) override;

    QString last_query_="";

    QString last_con_name_="";

    QPointer<fontEmbeddedWidget> fontWidget_;
    static QString defaultFont_;

    void reject() override;

    static bool askBeforeClose_;
    bool checkCloseMessageFlag_ = false;

    bool eventFilter(QObject *obj, QEvent *event) override;

    void repaintCopyButtonText(QColor);
};

#endif // CUSTOMQUERYRESULT_H

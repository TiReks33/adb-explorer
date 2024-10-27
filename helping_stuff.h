#ifndef HELPING_STUFF_H
#define HELPING_STUFF_H

#include <QString>
#include <QStringList>
#include <QPlainTextEdit>
#include <QDateTime>
#include <QTextStream>
#include <QMap>
#include <QDebug>
#include <QMessageBox>
#include <QPointer>
#include <QDialog>
#include <QTimer>
#include <QComboBox>
#include <QTableView>
#include <QLayout>
#include <QHeaderView>
#include <QLabel>
#include <QFileDialog>
#include <QCoreApplication>
#include <QCheckBox>
#include <QPushButton>
#include <QFileInfo>


#include "auth.h"
#include "signaltableview.h"
#include "notifycombobox.h"

////int const _CUSTOM_MESSAGE_BOX_TIMER_=15000;

// custom QMessageBox with closing timer
class adbMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit adbMessageBox(QWidget *parent = nullptr);

    explicit adbMessageBox(QMessageBox::Icon icon, const QString &title,
                           const QString &text, QMessageBox::StandardButtons buttons = NoButton,
                           QWidget *parent = nullptr,
                           Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    explicit adbMessageBox(QMessageBox::Icon icon, const QString &title,
                           const QString &text,

                           int closeTimer__,

                           QMessageBox::StandardButtons buttons = NoButton,
                           QWidget *parent = nullptr,
                           Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    virtual ~adbMessageBox();
};

// add new functions to this workspace
namespace adb_utility
{
    extern int _CUSTOM_MESSAGE_BOX_TIMER_;
    extern QString const filepath_;
    extern QString const currentLogFileName;
    extern QString const settings_f_name_;
//    extern bool _INTERACTIVE_RESIZE_;
    extern QString const en_lit;
    extern QString const digits_lit;
    extern QString const spec_chars_lit;



    // get custom QMessageBox
    void get_information_window(enum QMessageBox::Icon messageBoxType__, QString const&,QString const&, QWidget* = nullptr, bool stayOnTop__ = false);

    QWidget* getRescaleBox(/*QTableView*/QPointer<signalTableView>);

    void exportTableViewToCSV(QTableView *table);


    // r/w funcs for internal configuration file of the "adb_utility::" namespace
    // with separate sub-namespace.
    namespace config {
        bool read4rom_settings_file();

        void write2_settings_file();
    }


    // set textCursor to the end of text form
    void set_cursor_to_end_(QPlainTextEdit*);

    // move window to the center of a previuous window
    void window_center_from_another_(QWidget*,QWidget*);

    // QStringList->QString with separator
    QString pack_(QStringList const& list, QString const& separator=", ");

    // QString->QStringList by separator
    QStringList unpack_(QString const& string, QString const& separator=", ");

    // replaces all entries of pattern in string with some text (std::string)
    void replace_all( std::string& s, std::string const& toReplace, std::string const& replaceWith);

    // replaces all entries of pattern in string with some text (QString)
    void replace_all(QString& s,QString const& toReplace,QString const& replaceWith);

    // replaces all entries of pattern in string with some text (QString const)
    QString replace_all(QString const& s,QString const& toReplace /*= "`"*/,QString const& replaceWith /*= "``"*/);

    // escaping special symbols for MySql && MariaDB (.bindValue(..) method doesn't work in driver's level)
    QString escape_sql_backticks(QString const&); // escaping SQL 'Grave Accent' apostrophes
    QStringList escape_sql_backticks(QStringList const); // same for QList


    // redirect messages to file
    void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    // P.S.
    //Redirect QDebug in our custom class
    //If we want to redirect the output of our custom class to QDebug, it is necessary to declare the operator << of QDebug type as a friend method:
    //friend QDebug operator << (QDebug d, const Protocol &p);


    // insert numeric parameters from file to QMap
    bool get_settings_4rom_file(QString const&,QMap<QString,int>&);

    // insert string parameters (key="..") from file to QMap
    bool get_settings_4rom_file(QString const&,QMap<QString,QString>&);

    bool showExitAppDialog(QWidget* parent = nullptr, bool khakiHighlightStyle = true);

    inline bool fileExists_(QString const& path__){
        QFileInfo check_file(path__);
        // check if file exists and if yes: Is it really a file and not a directory?
        if (!(check_file.exists() && check_file.isFile())) {
            return false;
        }
        return true;
    };
}


namespace adb_style {

QColor rgb_reverse(QColor);

//extern QString const buttonKhakiHiglightSS_;
extern QString const buttonKhakiHighlightTextHoverColor_;
extern QString const buttonKhakiHighlightTextFocusColor_;
QString const getbuttonKhakiHiglightSS(QString const & hoverTextColor = adb_style::buttonKhakiHighlightTextHoverColor_,
                                       QString const & focusTextColor = adb_style::buttonKhakiHighlightTextFocusColor_,
                                       int borderWidth = 1);
extern QString const whiteUndGrayColor;
QString const getComboBoxKhakiHighlightSS(QString const & bgColour = "",
                                          QString const & fontColour = "",
                                          QString const & arrowImageSS = "url(:/pic/adbarrowdwn3.png)"
                                          );
extern QString const adbRadioStyleSheet;
extern QString const adbCheckBoxStyleSheet;

}



#endif // HELPING_STUFF_H



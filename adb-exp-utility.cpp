#include "adb-exp-utility.h"

int adb_utility::_CUSTOM_MESSAGE_BOX_TIMER_=15000;
QString const adb_utility:: filepath_ = QDir::homePath()+"/.adb-explorer";
QString const adb_utility::currentLogFileName = adb_utility::filepath_+"/LogFile.log";
QString const adb_utility::settings_f_name_=/*adbexplorer::filepath_*/adb_utility::filepath_+'/'+"adb_utility.cfg";
//bool adb_utility::_INTERACTIVE_RESIZE_=false;
QString const adb_utility::en_lit = "a-zA-Z";
QString const adb_utility::digits_lit = "0-9";
QString const adb_utility::spec_chars_lit = "_$";


QString const adb_style::buttonKhakiHighlightTextHoverColor_ = "black";
QString const adb_style::buttonKhakiHighlightTextFocusColor_ = "darkslategray";

QString const adb_style::getbuttonKhakiHiglightSS(QString const & hoverTextColor__, QString const & focusTextColor__, int borderWidth__){
    QString const __buttonKhakiHiglightStyleSheet = QStringLiteral("QPushButton:hover:!pressed  {"
    " background-color: khaki;"
    " color: %1; "
    " border-width: %2px; "
    " border-style: solid; "
    " border-top-color: goldenrod; "
    " border-left-color: goldenrod; "
    " border-right-color: goldenrod;"
    " border-bottom-color: goldenrod; "
    " border-top-left-radius: 3px; border-top-right-radius: 3px; "
    " border-bottom-left-radius: 3px; border-bottom-right-radius: 3px; "
    " }"
    " QPushButton:focus { "
    " outline: none; "
    " background:  lightgoldenrodyellow;" //ivory, #ffffdc
    " color: %3; "
    " border-width: %4px; "
    " border-style: solid; "
    " border-top-color: darkgoldenrod; "
    " border-left-color: darkgoldenrod; "
    " border-right-color: darkgoldenrod;"
    " border-bottom-color: darkgoldenrod; "
    " border-top-left-radius: 3px; border-top-right-radius: 3px; "
    " border-bottom-left-radius: 3px; border-bottom-right-radius: 3px; "
    " }").arg(hoverTextColor__)
            .arg(borderWidth__)
            .arg(focusTextColor__)
            .arg(borderWidth__);

    return __buttonKhakiHiglightStyleSheet;
}

QString const adb_style::whiteUndGrayColor = "#f1f1f1";

const QString adb_style::getComboBoxKhakiHighlightSS(QString const & bgColour__, QString const & fontColour__, const QString &arrowImageSS__  )
{
    QString const __newComboBoxStyleSheet =
    QStringLiteral(
    "QComboBox "
    " { "
    " color: %1; "
    " background: %2; "
    " border: 1px solid ; "
    " font-weight:bold; "
    " border-radius: 4px; "
    " padding-left: 10px; "
    " padding-top: 3px; "
    " padding-bottom: 3px; "
    " padding-right: 3px; "
    " } "
    " QComboBox:disabled "
    " { "
    " color: gray; "
    " background: lightgray; "
    " border: 1px solid ; "
    " font-weight:bold; "
    " border-radius: 4px; "
    " padding-left: 10px; "
    " padding-top: 3px; "
    " padding-bottom: 3px; "
    " padding-right: 3px; "
    " } "
    ""
    "QComboBox::drop-down "
    " { "
    " border: 0px; "
    " } "
    ""
    "QComboBox::down-arrow "
    " { "
    " image: %3 ; "
    " width: 12px; "
    " height: 12px; "
    " margin-right: 15px; "
    " } "
    ""
    "QComboBox QListView "
    " { "
    " font-size: 12px; "
    " border: 1px solid rgba(0,0,0, 10%); "
    " padding: 5px; "
    " color:darkslategray;"
    " background-color: lightgoldenrodyellow; " //#fffff0
    " outline: 0px; "
    " } "
    ""
    "QComboBox::hover:!pressed "
    " { "
    " background-color: khaki; "
    " color: black; "
    " border-width: 1px; "
    " border-style: solid; "
    " border-top-color: goldenrod; "
    " border-left-color: goldenrod; "
    " border-right-color: goldenrod; "
    " border-bottom-color: goldenrod; "
    " border-top-left-radius: 3px; border-top-right-radius: 3px; "
    " border-bottom-left-radius: 3px; border-bottom-right-radius: 3px; "
    " } "
    "QComboBox:focus { "
    " background:  lightgoldenrodyellow; "
    " color: darkslategray; "
    " border-width: 1px; "
    " border-style: solid; "
    " border-top-color: darkgoldenrod; "
    " border-left-color: darkgoldenrod; "
    " border-right-color: darkgoldenrod;"
    " border-bottom-color: darkgoldenrod; "
    " } "
    )
    .arg(fontColour__)
    .arg(bgColour__)
    .arg(arrowImageSS__)
    ;
    return __newComboBoxStyleSheet;
}

QString const adb_style::adbRadioStyleSheet = "QRadioButton:!disabled {/*background-color: gray;*/ color: black; font-weight:bold; } "
    " QRadioButton::indicator {width: 10px; height: 10px; border-radius: 7px;} "
    " QRadioButton::indicator:checked {background-color: darkslategray; border: 2px solid white; } "
    " QRadioButton::indicator:unchecked {background-color: white; border: 2px solid white; } ";

QString const adb_style::adbCheckBoxStyleSheet = "QCheckBox:!disabled {/*background-color: gray;*/ color: black; font-weight:bold; } "
    " QCheckBox::indicator:checked:focus { border: 2px solid darkgoldenrod; image: url(:/pic/check-mark-true_darkgoldenrod.png) 2 2 2 2 stretch stretch; background-repeat: no-repeat;  background-color: white; }" //!!
    " QCheckBox::indicator:unchecked:focus { background-color: white; border: 2px solid darkgoldenrod; }" //!!

    " QCheckBox:checked:focus { color: darkgoldenrod; font-weight:bold; } " //!!
    " QCheckBox:unchecked:focus {color: darkgoldenrod; font-weight:bold;}" //!!

    " QCheckBox::indicator {width: 10px; height: 10px; border-radius: 4px;} "
    " QCheckBox::indicator:checked { border: 2px solid darkslategray; image: url(:/pic/check-mark-true_darkslategray.png) 2 2 2 2 stretch stretch; background-repeat: no-repeat;  background-color: white;   } "
    " QCheckBox::indicator:unchecked {background-color: white; border: 2px solid darkslategray; }"
    " QCheckBox::indicator:disabled:checked { border: 2px solid lightgray; image: url(:/pic/check-mark-true_disable.png) 2 2 2 2 stretch stretch; background-repeat: no-repeat;  background-color: white;   } "
    " QCheckBox::indicator:disabled:unchecked {background-color: white; border: 2px solid lightgray; }"
;



void adb_utility::get_information_window(enum QMessageBox::Icon messageBoxType__, const QString & header_text__, const QString & main_text__,  QWidget *parent__, bool stayOnTop__)
{
//    // if .cfg file read fail, write default timer digit to it
//    if(!read4rom_settings_file())
//        write2_settings_file();

    static QPointer </*QMessageBox*/adbMessageBox> messageBox /*= nullptr*/;

    if(messageBox)messageBox->close();

    messageBox = new /*QMessageBox*/adbMessageBox(messageBoxType__,header_text__,
                                                      main_text__,
                                                      _CUSTOM_MESSAGE_BOX_TIMER_,
                                                      QMessageBox::Ok,parent__);

    //connect(messageBox,&QMessageBox::destroyed,[&](){ qDebug() << "~messageBox activated (destroyed).";});
    messageBox->setAttribute( Qt::WA_DeleteOnClose, true );

    QFlag flags = 0; bool IsModal = false;
    if(!stayOnTop__){
        flags = QFlag(Qt::Window & ~Qt::WindowStaysOnTopHint);
    }else{
        flags = QFlag(Qt::Dialog);
        IsModal = true;
    }
    messageBox->setWindowFlags(flags);
    messageBox->setModal(IsModal);
    messageBox->show();
}

QWidget*
adb_utility::getRescaleBox(/*QTableView*/QPointer<signalTableView>sourceTableView)
{
    QWidget* main_widget = new (std::nothrow) QWidget;

    if(main_widget){

        QGridLayout* main_lay = new QGridLayout;
        main_widget->setLayout(main_lay);
            main_lay->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        main_lay->setSpacing(0);

        main_lay->setContentsMargins(0,0,0,0);

        QLabel* lbl = new QLabel{"Scale mode: "};
        main_lay->addWidget(lbl,0,0);
        lbl->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

        QPointer</*QComboBox*/notifyComboBox> comboBox = new /*QComboBox*/notifyComboBox;
        main_lay->addWidget(comboBox,0,1);
        comboBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);


//        QObject::connect(main_widget,&QWidget::destroyed,[=]{ /*delete comboBox;*/ qDebug() << "~getRescaleBox()::main_widget";});
//        QObject::connect(comboBox,&/*QComboBox*/notifyComboBox::destroyed,[=]{ qDebug() << "~getRescaleBox()::comboBox";});

        comboBox->insertItem(0,"Padding to fit page");
        comboBox->insertItem(1,"Interactive resize");
        comboBox->insertItem(2, "Wrap text to fit content");

        if(sourceTableView){

            QObject::connect(sourceTableView,&signalTableView::model_changed,[=](){

                if(sourceTableView){
                    int curInd = comboBox->currentIndex();
                    switch(curInd)
                    {
                        case 0: sourceTableView->setRescale(QHeaderView::Stretch,QHeaderView::ResizeToContents); break;
                        case 1: sourceTableView->setRescale(QHeaderView::Interactive,QHeaderView::Interactive); break;
                        case 2: sourceTableView->setRescale(QHeaderView::ResizeToContents,QHeaderView::ResizeToContents); break;
                    }
                }

            });

            QObject::connect(comboBox,static_cast<void (/*QComboBox*/notifyComboBox::*)(int)>(&/*QComboBox*/notifyComboBox::currentIndexChanged),[=](int index__){

                if(sourceTableView){
                    switch(index__)
                    {
                        case 0: sourceTableView->setRescale(QHeaderView::Stretch,QHeaderView::ResizeToContents); break;
                        case 1: sourceTableView->setRescale(QHeaderView::Interactive,QHeaderView::Interactive); break;
                        case 2: sourceTableView->setRescale(QHeaderView::ResizeToContents,QHeaderView::ResizeToContents); break;
                    }

                }

            });

        }

//        comboBox->setCurrentIndex(_INTERACTIVE_RESIZE_);

    }

    return main_widget;
}

void adb_utility::exportTableViewToCSV(QTableView *table)
{
     QString filters("CSV files (*.csv);;All files (*.*)");
     QString defaultFilter("CSV files (*.csv)");

     QString fileName = QFileDialog::getSaveFileName(0, "Save file", /*QCoreApplication::applicationDirPath()*/adb_utility::filepath_+"/adb_exp_output.csv",
                        filters, &defaultFilter);
     QFile file(fileName);

     QAbstractItemModel *model =  table->model();
     if (file.open(QFile::WriteOnly | QFile::Truncate)) {
         QTextStream data(&file);
         QStringList strList;
         for (int i = 0; i < model->columnCount(); i++) {
             if (model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().length() > 0)
                 strList.append("\"" + model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + "\"");
             else
                 strList.append("");
         }
         data << strList.join(";") << "\n";
         for (int i = 0; i < model->rowCount(); i++) {
             strList.clear();
             for (int j = 0; j < model->columnCount(); j++) {

                 if (model->data(model->index(i, j)).toString().length() > 0)
                     strList.append("\"" + model->data(model->index(i, j)).toString() + "\"");
                 else
                     strList.append("");
             }
             data << strList.join(";") + "\n";
         }
         file.close();
     }

 }



QString adb_utility::pack_(const QStringList &list, QString const& separator)
{
    return list.join(separator);
}


QStringList adb_utility::unpack_(const QString &string, QString const& separator)
{
    return string.split(separator);
}


void adb_utility::set_cursor_to_end_(QPlainTextEdit *plainTextEdit)
{
    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    plainTextEdit->setTextCursor(cursor);
}



void adb_utility::window_center_from_another_(QWidget *old_window_,QWidget*new_window_)
{
    QPoint centerPoint = old_window_->geometry().center();

    //new_window_->adjustSize();
    new_window_->move(centerPoint.x() - new_window_->width()/2, centerPoint.y() - new_window_->height()/2);
}


void adb_utility::replace_all(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
) {
    std::string buf;
    std::size_t pos = 0;
    std::size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos)
            break;
        buf.append(s, prevPos, pos - prevPos);
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s, prevPos, s.size() - prevPos);
    s.swap(buf);
}

void adb_utility::replace_all(QString& s,QString const& toReplace,QString const& replaceWith) {
    QString buf;
    size_t pos = 0;
    size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.indexOf(toReplace, pos);
        if (pos == size_t(-1))
            break;

        buf.append(s.mid(prevPos, pos - prevPos));
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s.mid(prevPos, s.size() - prevPos));
    s.swap(buf);
}


QString adb_utility::replace_all(const QString &str, const QString &toReplace, const QString &replaceWith)
{
    QString final_string = str;
    QString buf;
    size_t pos = 0;
    size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(final_string.size());

    while (true) {
        prevPos = pos;
        pos = final_string.indexOf(toReplace, pos);
        if (pos == size_t(-1))
            break;

        buf.append(final_string.mid(prevPos, pos - prevPos));
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(final_string.mid(prevPos, final_string.size() - prevPos));
    final_string.swap(buf);
    return final_string;
}

QString adb_utility::escape_sql_backticks(const QString &str__)
{
    QString copy_str{str__};
    adb_utility::replace_all(copy_str,"`","``");
    return copy_str;
}

QStringList adb_utility::escape_sql_backticks(const QStringList list__)
{
    QStringList new_list_with_esc;

    for( const auto& i : list__)
    {
        new_list_with_esc.append(escape_sql_backticks(i));
    }

    return new_list_with_esc;
}



void adb_utility::customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   Q_UNUSED(context);

   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("[%1] ").arg(dt);

   switch (type)
   {
      case QtInfoMsg:
         txt += QString("{Info} \t\t %1").arg(msg);
         break;
      case QtDebugMsg:
         txt += QString("{Debug} \t\t %1").arg(msg);
         break;
      case QtWarningMsg:
         txt += QString("{Warning} \t %1").arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("{Critical} \t %1").arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("{Fatal} \t\t %1").arg(msg);
         abort();
         break;
   }

//   QString const currentLogFileName = adb_utility::filepath_+"/LogFile.log";

   QFile outFile(adb_utility:: currentLogFileName);

// if log file is too fat, backup it and place logs to new file -->
   QByteArray bytes = txt.toUtf8();
   int length = bytes.size(); //Number of bytes

   if(outFile.size()+length>=5242880/*2000*/){ //5mb file size limit
       QString const backup_file_name = adb_utility:: currentLogFileName+".old";

       if (QFile::exists(backup_file_name))
       {
           QFile::remove(backup_file_name);
       }

       QFile::copy(adb_utility:: currentLogFileName,backup_file_name);
       outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
   } else {
          outFile.open(QIODevice::WriteOnly | QIODevice::Append);
   }
//<<-
   QTextStream textStream(&outFile);
   textStream << txt << Qt::endl;
}

// parsing of file with different key-values pairs
bool adb_utility::get_settings_4rom_file(QString const& file_name__,QMap<QString, int> &settings_map__)
{

    QFile cFile(file_name__);

    if(cFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        QString line,name,value;

        int delimiterPos,value_to_int;
        bool ok;
            QTextStream in(&cFile);
               while (!in.atEnd())
               {
                  line = in.readLine();
                  line.remove(' ');
                  if(/*(line.size()!=0&&line.at(0)=='#')*/ line.isEmpty() || line[0]=='#')
                      continue;
                  delimiterPos = line.indexOf("=");
                  name = line.mid(0,delimiterPos);

                  value = line.mid(delimiterPos+1);

                  //qDebug().noquote().nospace() << name << ' ' << value;

                  value_to_int = value.toInt(&ok);

                  if(ok)
                      settings_map__.insert(name,value_to_int);
                  else
                      settings_map__.insert(name,-1);
               }
               cFile.close();

               //qDebug() << settings_map__;

               return true;
    }

    qWarning() << QString("Couldn't open config file `%1` for reading. Check file name correctness or read permissions.").arg(file_name__);

    return false;
}

bool adb_utility::get_settings_4rom_file(const QString &file_name__, QMap<QString, QString> &settings_map__)
{
    QFile cFile(file_name__);

    if(cFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        QString line,name,value;

        int delimiterPos;

            QTextStream in(&cFile);
               while (!in.atEnd())
               {

                   line = in.readLine();

                   auto size = line.size();

                   delimiterPos = line.indexOf("=");

                   if(line.isEmpty() || line[0]=='#' || delimiterPos==-1)
                       continue;


                   name = QStringRef(&line,0,delimiterPos).toString().remove(' ');


                   auto firstbracket = line.indexOf("\"",delimiterPos);

                   auto lastbracket = line.indexOf("\"",firstbracket+1);

                   if(firstbracket!=-1&&lastbracket!=-1){


                       value = line.remove(lastbracket+1,size-1-lastbracket)
                               .remove(delimiterPos+1,firstbracket-1-delimiterPos)
                               .mid(delimiterPos+1)
                               .remove("\"");
//qDebug() << "VALUE"<<value;

                       settings_map__.insert(name,value);

                  }

               }

               cFile.close();


               return true;
    }

    qWarning() << QString("Couldn't open config file `%1` for reading. Check file name correctness or read permissions.").arg(file_name__);

    return false;
}

adbMessageBox::adbMessageBox(QWidget *parent)
    : QMessageBox(parent)
{
}

adbMessageBox::adbMessageBox(QMessageBox::Icon icon, const QString &title, const QString &text, QMessageBox::StandardButtons buttons, QWidget *parent, Qt::WindowFlags f)
    : QMessageBox(icon,title,text,buttons,parent,f)
{
}

adbMessageBox::adbMessageBox(QMessageBox::Icon icon, const QString &title, const QString &text, int closeTimer__, QMessageBox::StandardButtons buttons, QWidget *parent, Qt::WindowFlags f)
    : QMessageBox(icon,title,text,buttons,parent,f)
{
    QTimer::singleShot(closeTimer__,this,&adbMessageBox::close);
}

adbMessageBox::~adbMessageBox()
{
    //qDebug() << "~adbMessageBox";
}

bool adb_utility::config::read4rom_settings_file()
{
    QMap<QString,int> __settings_map;

    if(adb_utility::get_settings_4rom_file(adb_utility::settings_f_name_,__settings_map)){
        int temp;

        if((temp = __settings_map.value("_CUSTOM_MESSAGE_BOX_TIMER_"))!=-1)
            adb_utility::_CUSTOM_MESSAGE_BOX_TIMER_ = temp;

//        if((temp = __settings_map.value("_INTERACTIVE_RESIZE_"))!=-1)
//            _INTERACTIVE_RESIZE_ = temp;

        return true;
    }

    qWarning() << "Error while read from"<<adb_utility::settings_f_name_;

    return false;
}

void adb_utility::config::write2_settings_file()
{
    QFile outFile(adb_utility::settings_f_name_);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);
    textStream << "_CUSTOM_MESSAGE_BOX_TIMER_" << '=' << QString::number(adb_utility::_CUSTOM_MESSAGE_BOX_TIMER_) << Qt::endl;
//    textStream << "_INTERACTIVE_RESIZE_" << '=' << QString::number(_INTERACTIVE_RESIZE_) << Qt::endl;
}



QColor adb_style::rgb_reverse(QColor colour__)
{
    return QColor::fromRgb(255 - colour__.red(),255 - colour__.green(),255 - colour__.blue());
//    auto color = std::stol("76BED0",nullptr,16);
////            int color = (int)Long.parseLong("#76BED0", 16); #java
//        auto r = (color >> 16) & 0xFF;
//        auto g = (color >> 8) & 0xFF;
//        auto b = (color >> 0) & 0xFF;
//        auto invertedRed = 255 - r;
//        auto invertedGreen = 255 - g;
//        auto invertedBlue = 255 - b;

//        qDebug() << QColor::fromRgb(invertedRed,invertedGreen,invertedBlue).name();

}



bool adb_utility::showExitAppDialog(QWidget *parent__, bool khakiHighlightStyle__)
{
    QPointer <adbMessageBox> quitMessageBox = new adbMessageBox(QMessageBox::Warning,"Quit", "Do you really want to exit application?",
                                                  QMessageBox::Yes | QMessageBox::No,parent__);

    quitMessageBox->setAttribute( Qt::WA_DeleteOnClose, true);
    //connect(quitMessageBox,&adbMessageBox::destroyed,[]{qDebug() << "~adbMessageBox";});

    if(khakiHighlightStyle__){
        QList<QPushButton*> ButtonsInFormlist = quitMessageBox->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
//            if(obj==quitMessageBox->button(QMessageBox::Yes)||obj==quitMessageBox->button(QMessageBox::No)){
                obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
//            }
        }
    }

    quitMessageBox->setModal(true);
    quitMessageBox->show();
    int dialogAnswer = quitMessageBox->exec();

    if(dialogAnswer!=QMessageBox::Yes){

        return false;
    }

    return true;
}



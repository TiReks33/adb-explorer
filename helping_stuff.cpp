#include "helping_stuff.h"

int adb_utility::_CUSTOM_MESSAGE_BOX_TIMER_=15000;
QString const adb_utility::settings_f_name_=adbexplorer::filepath_+'/'+"adb_utility.cfg";

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


QString pack_(const QStringList &list, QString const& separator)
{
    return list.join(separator);
}


QStringList unpack_(const QString &string, QString const& separator)
{
    return string.split(separator);
}


void set_cursor_to_end_(QPlainTextEdit *plainTextEdit)
{
    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    plainTextEdit->setTextCursor(cursor);
}


void window_center_from_another_(QWidget *old_window_,QWidget*new_window_)
{
    QPoint centerPoint = old_window_->geometry().center();

    //new_window_->adjustSize();
    new_window_->move(centerPoint.x() - new_window_->width()/2, centerPoint.y() - new_window_->height()/2);
}


void replace_all(
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

void replace_all(QString& s,QString const& toReplace,QString const& replaceWith) {
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


QString replace_all(const QString &str, const QString &toReplace, const QString &replaceWith)
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

QString escape_sql_backticks(const QString &str__)
{
    QString copy_str{str__};
    replace_all(copy_str,"`","``");
    return copy_str;
}

QStringList escape_sql_backticks(const QStringList list__)
{
    QStringList new_list_with_esc;

    for( const auto& i : list__)
    {
        new_list_with_esc.append(escape_sql_backticks(i));
    }

    return new_list_with_esc;
}



void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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

   QString const currentFileName = adbexplorer::filepath_+"/LogFile.log";

   QFile outFile(currentFileName);

// if log file is too fat, backup it and place logs to new file -->
   QByteArray bytes = txt.toUtf8();
   int length = bytes.size(); //Number of bytes

   if(outFile.size()+length>=5242880/*200*/){ //5mb file size limit
       QString const backup_file_name = currentFileName+".old";

       if (QFile::exists(backup_file_name))
       {
           QFile::remove(backup_file_name);
       }

       QFile::copy(currentFileName,backup_file_name);
       outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
   } else {
          outFile.open(QIODevice::WriteOnly | QIODevice::Append);
   }
//<<-
   QTextStream textStream(&outFile);
   textStream << txt << Qt::endl;
}

// parsing of file with different key-values pairs
bool get_settings_4rom_file(QString const& file_name__,QMap<QString, int> &settings_map__)
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

bool get_settings_4rom_file(const QString &file_name__, QMap<QString, QString> &settings_map__)
{
    QFile cFile(file_name__);

    if(cFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        QString line,name,value;

        int delimiterPos;

            QTextStream in(&cFile);
               while (!in.atEnd())
               {
                  line = in.readLine();
                  line.remove(' ');
                  if(line.isEmpty() || line[0]=='#')
                      continue;
                  delimiterPos = line.indexOf("=");

                  ////qDebug() << "Size-1::" << line.size()-1 << "delimeterPos+1" << delimiterPos+1;

                  if((line.size()-1!=delimiterPos+1)&&(line.at(delimiterPos+1)=='\"')/*&&(line.right(1)=='\"')*/&&(line.indexOf("\"",delimiterPos+2)!=-1)){

                  name = line.mid(0,delimiterPos);

                  value = line.mid(delimiterPos+1).remove('\"');

                  //qDebug().noquote().nospace() << name << ' ' << value;

//                  if( empty_value__ || (!empty_value__&&!value.isEmpty()) )
                    settings_map__.insert(name,value);

                  }

               }

               cFile.close();

               //qDebug() << settings_map__;

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

bool adb_utility::read4rom_settings_file()
{
    QMap<QString,int> __settings_map;

    if(get_settings_4rom_file(settings_f_name_,__settings_map)){
        int temp;

        if((temp = __settings_map.value("_CUSTOM_MESSAGE_BOX_TIMER_"))!=-1)
            _CUSTOM_MESSAGE_BOX_TIMER_ = temp;

        return true;
    }

    qWarning() << "Error while read from"<<settings_f_name_;

    return false;
}

void adb_utility::write2_settings_file()
{
    QFile outFile(settings_f_name_);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);
    textStream << "_CUSTOM_MESSAGE_BOX_TIMER_" << '=' << QString::number(_CUSTOM_MESSAGE_BOX_TIMER_) << Qt::endl;
}

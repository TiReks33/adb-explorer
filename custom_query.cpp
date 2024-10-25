#include "custom_query.h"
#include "ui_custom_query.h"

#include <QFontDialog>

Custom_Query::Custom_Query(QWidget *parent, QFont font) :
    QDialog(parent),
    ui(new Ui::Custom_Query)
  , font_(/*ui->plainTextEdit->font().toString()*/font.toString())
{
    ui->setupUi(this);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    //this->ui->buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet("background:green; color:white;");
    this->ui->Ok_button->setStyleSheet("background:green; color:white;");

    //SIGNALS

    connections_init();

    // if read file unsuccessful -- write default settings to file
    if(!read4rom_settings_file())
        write2_settings_file();

    //ui->plainTextEdit->setFont(QFont(font_family,font_pointSize,font_weight,font_italic));
    QFont __font;
    __font.fromString(font_);
    ui->plainTextEdit->setFont(__font);
}

Custom_Query::~Custom_Query()
{
        //qDebug()<<"~CustomQuery activated";
    delete ui;
}


void Custom_Query::connections_init()
{
    // syncronize settings changes with file
    connect(this,&Custom_Query::font_changes,[=]{ write2_settings_file(); });

    // get font choose window
    connect(ui->setFont_Button,&QPushButton::clicked,[=]{
        bool ok;

        QFont oldFont;
        oldFont.fromString(font_);

        QFont font = QFontDialog::getFont(
                        &ok, oldFont, this);
        if (ok) {
            // the user clicked OK and font is set to the font the user selected
            ui->plainTextEdit->setFont(font);

            font_ = font.toString();

            emit font_changes();

            ////qDebug() << font.family() << font.pointSize() << font.weight() << font.italic();
        } else {
            // the user canceled the dialog; font is set to the initial value
        }
    });

    // send result of query exec. to form
    connect(ui->Ok_button,&QPushButton::clicked,[=]{
        emit send_custom_query(/*ui->plainTextEdit->toPlainText(),*/this);
    });

    connect(ui->Cancel_button,&QPushButton::clicked,[=]{
        this->close();
    });
}


QString const Custom_Query::get_text() const
{
    return this->ui->plainTextEdit->toPlainText();
}



void Custom_Query::close_window()
{
    //qDebug()<<"Close custom query form signal handled";
    this->close();
}

void Custom_Query::set_text(QString const& text__) const
{
    this->ui->plainTextEdit->setPlainText(text__);
}

void Custom_Query::add_note(const QString & message__)
{

    QFrame* style_frame = new QFrame{this};

    QHBoxLayout* dynamic_note_lay = new QHBoxLayout;

    style_frame->setLayout(dynamic_note_lay);

    style_frame->setFrameShape(QFrame::StyledPanel);
    style_frame->setFrameShadow(QFrame::Raised);


    ClickableLabel* dyn_note_lbl = new ClickableLabel(message__);
    dyn_note_lbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    dyn_note_lbl->setWordWrap(true);
    QPushButton* dyn_note_hide_button = new QPushButton("hide");
    dyn_note_hide_button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    QPushButton* dyn_note_disable_button = new QPushButton("Don't show again");
    dyn_note_disable_button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    dynamic_note_lay->addWidget(dyn_note_lbl);
    dynamic_note_lay->addWidget(dyn_note_hide_button);
    dynamic_note_lay->addWidget(dyn_note_disable_button);

    ui->verticalLayout->insertWidget(0,style_frame);

    connect(dyn_note_hide_button,&QPushButton::clicked,[=]{
        delete style_frame;
    });

    connect(dyn_note_disable_button,&QPushButton::clicked,[=]{
        delete style_frame;
        emit dont_show_note();
    });
}



bool Custom_Query::read4rom_settings_file()
{

    QMap<QString,QString> __settings_map;

    if(get_settings_4rom_file(settings_f_name,__settings_map)){
        QString temp_s;

        if((temp_s = __settings_map.value("font_"))!="")
                        font_ = temp_s;

        return true;
    }

    qWarning() << "Error while read from"<<settings_f_name<<"(Strings)";

    return false;
}

void Custom_Query::write2_settings_file()
{
    QFile outFile(settings_f_name);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);

     textStream << "font_" << '=' << '\"'+font_+'\"' << Qt::endl;
}

#include "custom_query.h"
#include "ui_custom_query.h"

#include <QFontDialog>

Custom_Query::Custom_Query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Custom_Query)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    ui->statusLine->setReadOnly(true);

    //this->ui->buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet("background:green; color:white;");
    this->ui->Ok_button->setStyleSheet("background:green; color:white;");

    //SIGNALS

    connections_init();




}

Custom_Query::~Custom_Query()
{
        qDebug()<<"~CustomQuery activated";
    delete ui;
}


void Custom_Query::connections_init()
{
    connect(ui->setFont_Button,&QPushButton::clicked,[=]{
        bool ok;
        QFont font = QFontDialog::getFont(
                        &ok, QFont("Helvetica [Cronyx]", 10), this);
        if (ok) {
            // the user clicked OK and font is set to the font the user selected
            ui->plainTextEdit->setFont(font);
        } else {
            // the user canceled the dialog; font is set to the initial
            // value, in this case Helvetica [Cronyx], 10
        }
    });

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
    qDebug()<<"Close custom query form signal handled";
    this->close();
}

void Custom_Query::set_text(QString const& text__) const
{
    this->ui->plainTextEdit->setPlainText(text__);
}

void Custom_Query::add_note(const QString & message__)
{
    ui->verticalLayout->insertWidget(0,new QLabel(message__));
}

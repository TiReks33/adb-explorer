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


}

Custom_Query::~Custom_Query()
{
    delete ui;
}

void Custom_Query::on_setFont_Button_clicked()
{
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

}

void Custom_Query::on_Ok_button_clicked()
{
    emit send_custom_query(ui->plainTextEdit->toPlainText());
}

void Custom_Query::on_Cancel_button_clicked()
{
    this->close();
}

void Custom_Query::close_window()
{
    qDebug()<<"Close custom query form signal handled";
    this->close();
}

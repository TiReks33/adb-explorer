#include "custom_query.h"
#include "ui_custom_query.h"

#include <QFontDialog>

Custom_Query::Custom_Query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Custom_Query)
{
    ui->setupUi(this);
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

void Custom_Query::on_buttonBox_accepted()
{
    emit set_custom_query(ui->plainTextEdit->toPlainText());
}

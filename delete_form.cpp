#include "delete_form.h"

delete_form::delete_form(QWidget *parent,QString const &entity_name__) :
    QDialog(parent)
  , ui(new Ui::delete_form)
  , entity_name_(entity_name__)
  , reloadEntitiesListButton_{new reloadButton{}}
{

    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(-1);

    connect(ui->comboBox,&QComboBox::textActivated,[=](const QString & cur_text__){
        qDebug() <<QString("%0 to delete:: %1").arg(entity_name_).arg(cur_text__);
    });

    ui->label->setText("Choose "+entity_name_+ " to <font style=\"color: black;background: red; font-weight:bold;\">DELETE</font>");
    setWindowTitle("Remove "+entity_name_+" dialog");

    reloadEntitiesListButton_->setParent(this);
    reloadEntitiesListButton_->setFontColour("snow");
    reloadEntitiesListButton_->setBackgroundColour("darkslategray");
    reloadEntitiesListButton_->setBold(true);

    ui->comboBox->setStyleSheet(adb_style::getComboBoxKhakiHighlightSS("#f1f1f1","black"));

    ui->comboBoxHorizontalLayout->addWidget(reloadEntitiesListButton_);

    reloadEntitiesListButton_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

    connect(reloadEntitiesListButton_,&QPushButton::clicked,this,&delete_form::reloadSig);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->comboBox,&QComboBox::currentTextChanged,[this](QString const& newText__){
        auto btn = ui->buttonBox->button(QDialogButtonBox::Ok);
        if(newText__.isEmpty()){
            btn->setEnabled(false);
        } else{
            btn->setEnabled(true);
        }
    });

    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            if(obj==ui->buttonBox->button(QDialogButtonBox::Ok)||obj==ui->buttonBox->button(QDialogButtonBox::Cancel)){
                obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            } else if(obj->objectName()=="reloadButtonObj"){
                // if button is inherited class object -> cast pointer to inherit type
                auto reloadButtonObj = qobject_cast<reloadButton*>(obj);
                reloadButtonObj->setKhakiHighlight();
            } else {
                obj->setStyleSheet(QStringLiteral("QPushButton{%1} %2").arg(obj->styleSheet()).arg(adb_style::getbuttonKhakiHiglightSS()));
            }
        }


}

delete_form::~delete_form()
{
    delete ui;
}

void delete_form::reload()
{
    emit reloadEntitiesListButton_->clicked();
}

void delete_form::delete_form_request_slot(){
    emit delete_form_send(ui->comboBox);
    //qDebug() << "Delete form signal was send.";
}

void delete_form::on_buttonBox_accepted()
{

    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", QString("Do you really want to delete chosen %0 \"%1\"?").arg(entity_name_).arg(ui->comboBox->currentText()),
                                                             QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          emit delete_entity(ui->comboBox);
        //qDebug() << QString("Yes was clicked in delete %0 form").arg(entity_name_);
      } else {
        //qDebug() << "cancel deletion";
      }

}




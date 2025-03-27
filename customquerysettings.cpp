#include "customquerysettings.h"
#include "ui_customquerysettings.h"

//#include "tables.h"

QMap<int,QString> CustomQuerySettings::paramEnumToStr{
    {tblQuerSet::separate_content_window, "separate_content_window"},
    {tblQuerSet::separate_describe_window, "separate_describe_window"},
    {tblQuerSet::separate_query_window, "separate_query_window"},
    {tblQuerSet::BLANK_RESULT, "BLANK_RESULT"},
    {tblQuerSet::MSG_SHOW_IF_BLANK_RESULT, "MSG_SHOW_IF_BLANK_RESULT"},
    {tblQuerSet::MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL, "MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL"}
};





CustomQuerySettings::CustomQuerySettings(/*Tables*/QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomQuerySettings)

//  , parent_{parent}

{
    ui->setupUi(this);

    formStyle();

    init_signals();

}

CustomQuerySettings::~CustomQuerySettings()
{
    delete ui;
}



void CustomQuerySettings::setForeignSettingsOnForm(QMap<QString, int> settings_map__)
{
    bool changes_before = changes_;
    int temp = 0;

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_content_window]))!=-1)
        ui->select_checkbox->setChecked(temp);

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_describe_window]))!=-1)
        ui->describeCheckBox->setChecked(temp);

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_query_window]))!=-1)
        ui->custom_checkbox->setChecked(temp);

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::BLANK_RESULT]))!=-1)
        ui->emptySetCheckBox->setChecked(temp);

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::MSG_SHOW_IF_BLANK_RESULT]))!=-1)
        ui->emptySetNotifyCheckBox->setChecked(temp);

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL]))!=-1)
        ui->terminateCheckBox->setChecked(temp);


    if(!changes_before)
        changes_=false;
}

void CustomQuerySettings::settings2Export(QMap<QString, int> & __settings_map)
{
    __settings_map[CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_content_window]] = ui->select_checkbox->isChecked();

    __settings_map[CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_describe_window]] = ui->describeCheckBox->isChecked();

    __settings_map[CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_query_window]] = ui->custom_checkbox->isChecked();

    __settings_map[CustomQuerySettings::paramEnumToStr[tblQuerSet::BLANK_RESULT]] = ui->emptySetCheckBox->isChecked();

    __settings_map[CustomQuerySettings::paramEnumToStr[tblQuerSet::MSG_SHOW_IF_BLANK_RESULT]] = ui->emptySetNotifyCheckBox->isChecked();

    __settings_map[CustomQuerySettings::paramEnumToStr[tblQuerSet::MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL]] = ui->terminateCheckBox->isChecked();

}


void CustomQuerySettings::init_signals()
{

    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,&QDialog::reject);

    connect(ui->buttonBox,&QDialogButtonBox::accepted,[=]{

        QMap<QString,int> __settings_map;

        settings2Export(__settings_map);

        emit settingsExportSig(__settings_map, changes_);

        this->accept();

    });

    connect(ui->emptySetCheckBox,&QCheckBox::stateChanged,[=](int state__){
        if(state__){
            ui->emptySetNotifyCheckBox->setChecked(false);
            ui->emptySetNotifyCheckBox->setEnabled(false);
        } else{
            ui->emptySetNotifyCheckBox->setEnabled(true);
        }
    });

    QList<QCheckBox*> comboBoxesInForm = this->findChildren<QCheckBox*>();
        foreach (auto obj, comboBoxesInForm) {
            connect(obj, &QCheckBox::stateChanged,[this]{
                changes_ = true;
            });
        }


}


void CustomQuerySettings::formStyle()
{
    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            if(obj==ui->buttonBox->button(QDialogButtonBox::Ok)||obj==ui->buttonBox->button(QDialogButtonBox::Cancel)){
                obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            } else {

                obj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }

        }

    QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
        foreach (auto obj, checkBoxInFormlist) {

                obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
        }
}



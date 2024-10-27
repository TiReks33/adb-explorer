#include "customquerysettings.h"
#include "ui_customquerysettings.h"

//#include "tables.h"

CustomQuerySettings::CustomQuerySettings(/*Tables*/QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomQuerySettings)

//  , parent_{parent}

{
    ui->setupUi(this);

    formStyle();

    init_signals();

    fileOps();

}

CustomQuerySettings::~CustomQuerySettings()
{
    delete ui;
}

void CustomQuerySettings::init_signals()
{

    connect(ui->buttonBox,&QDialogButtonBox::rejected,this,&QDialog::reject);

    connect(ui->buttonBox,&QDialogButtonBox::accepted,[=]{

        write2query_opts_file();

        ////read4rom_query_opts_file();
        adb_utility::get_settings_4rom_file(query_settings_f_name,__settings_map);

        emit settings_changed(__settings_map);

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

}

void CustomQuerySettings::fileOps()
{
    // set default if read file fails
    if(!read4rom_query_opts_file())
        write2query_opts_file();
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


void CustomQuerySettings::write2query_opts_file()
{
    QFile outFile(query_settings_f_name);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);
    textStream << "t_content_wnd" << '=' << QString::number(ui->select_checkbox->isChecked()) << Qt::endl;
    textStream << "t_describe_wnd" << '=' << QString::number(ui->describeCheckBox->isChecked()) << Qt::endl;
    textStream << "t_query_wnd" << '=' << QString::number(ui->custom_checkbox->isChecked()) << Qt::endl;
    textStream << "BLANK_RESULT" << '=' << QString::number(ui->emptySetCheckBox->isChecked()) << Qt::endl;
    textStream << "MSG_SHOW_IF_BLANK_RESULT" << '=' << QString::number(ui->emptySetNotifyCheckBox->isChecked()) << Qt::endl;
}

bool CustomQuerySettings::read4rom_query_opts_file()
{
    ////QMap<QString,int> __settings_map;

    __settings_map.clear();

    if(adb_utility::get_settings_4rom_file(query_settings_f_name,__settings_map)){
        int temp;

        if((temp = __settings_map.value("t_content_wnd"))!=-1)
            ui->select_checkbox->setChecked(temp);

        if((temp = __settings_map.value("t_describe_wnd"))!=-1)
            ui->describeCheckBox->setChecked(temp);

        if((temp = __settings_map.value("t_query_wnd"))!=-1)
            ui->custom_checkbox->setChecked(temp);

        if((temp = __settings_map.value("BLANK_RESULT"))!=-1)
            ui->emptySetCheckBox->setChecked(temp);

        if((temp = __settings_map.value("MSG_SHOW_IF_BLANK_RESULT"))!=-1)
            ui->emptySetNotifyCheckBox->setChecked(temp);

        return true;
    }

    qWarning() << "Error while read from"<<query_settings_f_name;

    return false;
}

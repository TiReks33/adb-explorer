#include "delete_sqldb_user.h"

delete_sqldb_user::delete_sqldb_user(QWidget*parent__) :
    delete_form(parent__,"SQL database user/role")
{
    QFrame* recordTypeFrame = new QFrame{};
    recordTypeFrame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    recordTypeFrame->setFrameShape(QFrame::Panel);

    recordTypeFrame->setFrameShadow(QFrame::Plain);

    QVBoxLayout* recordTypeLay = new QVBoxLayout{recordTypeFrame};

    QLabel* recordTypeLbl = new QLabel{};
    recordTypeLbl->setText("Choose record type to delete:");
    recordTypeLbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    recordTypeLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    recordTypeLay->addWidget(recordTypeLbl);

    ui->verticalLayout->insertWidget(0,recordTypeFrame);


    QFrame* recordTypeRButtonsSubFrame = new QFrame{};
    recordTypeRButtonsSubFrame->setFrameShape(QFrame::Panel);
    recordTypeRButtonsSubFrame->setFrameShadow(QFrame::Sunken);
    QHBoxLayout* recordTypeRButtonsSubLay = new QHBoxLayout{recordTypeRButtonsSubFrame};
    recordTypeRButtonsSubLay->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    recordTypeRButtonsSubLay->setContentsMargins(0,0,0,0);
    recordTypeRButtonsSubLay->setSpacing(0);

    recordTypeLay->addWidget(recordTypeRButtonsSubFrame);

    QRadioButton* userRButton_ = new QRadioButton{"delete User",this};
    userRButton_->setObjectName("deleteUser");
    recordTypeRButtonsSubLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    recordTypeRButtonsSubLay->addWidget(userRButton_);
    QRadioButton* roleRButton_ = new QRadioButton{"delete Role",this};
    roleRButton_->setObjectName("deleteRole");
    //
    recordTypeRButtonsSubLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    QFrame* recordTypeLineFrame = new QFrame{};
    recordTypeLineFrame->setFrameShape(QFrame::VLine);
    recordTypeLineFrame->setFrameShadow(QFrame::Raised);
    recordTypeRButtonsSubLay->addWidget(recordTypeLineFrame);
    //
    recordTypeRButtonsSubLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    recordTypeRButtonsSubLay->addWidget(roleRButton_);

    recordTypeRButtonsSubLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));

    QList<QRadioButton*> RButtonsInFormlist = this->findChildren<QRadioButton*>();
        foreach (auto obj, RButtonsInFormlist) {

                obj->setStyleSheet(adb_style::adbRadioStyleSheet);
        }

}

void delete_sqldb_user::on_buttonBox_accepted()
{
    delete_form::on_buttonBox_accepted();

}

#include "noteframe.h"

noteFrame::noteFrame(QString const & noteMessage__, QWidget* parent__) :
    QFrame(parent__),
    dyn_note_lbl_{new ClickableLabel{noteMessage__}},
    dyn_note_hide_button_{new QPushButton{"hide"}},
    dyn_note_disable_button_{new QPushButton{"Don't show again"}}
{
    initForm();
    initSig();
}

void noteFrame::initForm()
{
    QHBoxLayout* dynamic_note_lay = new QHBoxLayout;

    this->setLayout(dynamic_note_lay);

    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);


    //dyn_note_lbl_->setObjectName("NOTELABEL");
    dyn_note_lbl_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    dyn_note_lbl_->setWordWrap(true);
    dyn_note_hide_button_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    dyn_note_disable_button_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    dynamic_note_lay->addWidget(dyn_note_lbl_);
    dynamic_note_lay->addWidget(dyn_note_hide_button_);
    dynamic_note_lay->addWidget(dyn_note_disable_button_);
}

void noteFrame::initSig()
{
    connect(dyn_note_hide_button_,&QPushButton::clicked,this,&noteFrame::close);
    connect(dyn_note_disable_button_,&QPushButton::clicked,[this]{
        emit dontShowNoteAgainSig();
        this->close();
    });
}



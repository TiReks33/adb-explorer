#include "twolistselection.h"

TwoListSelection::TwoListSelection(QWidget *parent) : QWidget{parent} { //CONSTRUCTOR
    init();
    connections();
}

void TwoListSelection::addAvailableItems(const QStringList &items) {
    for( QString s : items)
        list_before_changes_.append(s);
    mInput->addItems(items);
    qDebug() << "list::"<<list_before_changes_;
}

QStringList TwoListSelection::selectedItems() {
    QStringList selected;
    for (int i = 0; i < mOutput->count(); i++)
        selected << mOutput->item(i)->text();
    return selected;
}

//void TwoListSelection::reset_handler()
//{
//    qDebug() << "Reset handler activated";
//    mOutput->clear();
//    mInput->clear();
//    mInput->addItems(list_before_changes_);
//}

//void TwoListSelection::ok_handler()
//{
//    emit export_list(this->selectedItems());
//    this->close();
//}

void TwoListSelection::init() {
    QHBoxLayout *layout = new QHBoxLayout(this);
    mInput = new QListWidget;
    mOutput = new QListWidget;

    mButtonToSelected = new QPushButton(">>");
    mBtnMoveToAvailable = new QPushButton(">");
    mBtnMoveToSelected = new QPushButton("<");
    mButtonToAvailable = new QPushButton("<<");

    //
    ok_button_ = new QPushButton("OK");
    ok_button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    cancel_button_ = new QPushButton("Cancel");
    cancel_button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    reset_button_ = new QPushButton("Reset");

    QHBoxLayout *actions_layout = new QHBoxLayout;
    //    actions_layout->addItem(
    //        new QSpacerItem(320, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    //actions_layout->addWidget(cancel_button_);

    actions_layout->addItem(new QSpacerItem(180, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    actions_layout->addWidget(ok_button_);
    //layout->addLayout(actions_layout);
    //

    QHBoxLayout *input_layout = new QHBoxLayout;
    QVBoxLayout *input_sublayout = new QVBoxLayout;

    input_layout->addWidget(mInput);

    QVBoxLayout *layoutm = new QVBoxLayout;
    layoutm->addItem(
                new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layoutm->addWidget(mButtonToSelected);
    layoutm->addWidget(mBtnMoveToAvailable);
    layoutm->addWidget(mBtnMoveToSelected);
    layoutm->addWidget(mButtonToAvailable);
    layoutm->addItem(
                new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));


    QHBoxLayout *cancel_layout = new QHBoxLayout;
    cancel_layout->addWidget(cancel_button_);
    cancel_layout->addItem(new QSpacerItem(240, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    //cancel_layout->addWidget(reset_button_);

    //input_layout->addLayout(layoutm);
    //
    QVBoxLayout *reset_layout = new QVBoxLayout;
    reset_layout->addLayout(layoutm);
    reset_layout->addWidget(reset_button_);

    input_layout->addLayout(/*layoutm*/reset_layout);
    input_sublayout->addLayout(input_layout);
    input_sublayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));
    input_sublayout->addLayout(cancel_layout);/*addWidget(reset_button_);*/
    layout->addLayout(input_sublayout);
    //layout->addLayout(layoutm);
    //
    //    layout->addWidget(mOutput);
    QHBoxLayout* output_layout = new QHBoxLayout;
    output_layout->addWidget(mOutput);


    mBtnUp = new QPushButton("Up");
    mBtnDown = new QPushButton("Down");

    QVBoxLayout *layoutl = new QVBoxLayout;
    layoutl->addItem(
                new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layoutl->addWidget(mBtnUp);
    layoutl->addWidget(mBtnDown);
    layoutl->addItem(
                new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    //layoutl->addLayout(actions_layout);
    //
    QVBoxLayout *sublayout = new QVBoxLayout;
    output_layout->addLayout(layoutl);
    sublayout->addLayout(output_layout);
    sublayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));
    sublayout->addLayout(actions_layout);
    layout->addLayout(/*layoutl*/sublayout);
    //
    //    QGridLayout *main_layout = new QGridLayout(this);
    //    main_layout->addLayout(layout,0,0);
    //    main_layout->addLayout(actions_layout,1,1);
    //
    setStatusButton();
}

void TwoListSelection::connections() {
    connect(mOutput, &QListWidget::itemSelectionChanged, this,
            &TwoListSelection::setStatusButton);
    connect(mInput, &QListWidget::itemSelectionChanged, this,
            &TwoListSelection::setStatusButton);
    connect(mBtnMoveToAvailable, &QPushButton::clicked,
            [=]() { mOutput->addItem(mInput->takeItem(mInput->currentRow())); });

    connect(mBtnMoveToSelected, &QPushButton::clicked,
            [=]() { mInput->addItem(mOutput->takeItem(mOutput->currentRow())); });

    connect(mButtonToAvailable, &QPushButton::clicked, [=]() {
        while (mOutput->count() > 0) {
            mInput->addItem(mOutput->takeItem(0));
        }
    });

    connect(mButtonToSelected, &QPushButton::clicked, [=]() {
        while (mInput->count() > 0) {
            mOutput->addItem(mInput->takeItem(0));
        }
    });

    connect(mBtnUp, &QPushButton::clicked, [=]() {
        int row = mOutput->currentRow();
        QListWidgetItem *currentItem = mOutput->takeItem(row);
        mOutput->insertItem(row - 1, currentItem);
        mOutput->setCurrentRow(row - 1);
    });

    connect(mBtnDown, &QPushButton::clicked, [=]() {
        int row = mOutput->currentRow();
        QListWidgetItem *currentItem = mOutput->takeItem(row);
        mOutput->insertItem(row + 1, currentItem);
        mOutput->setCurrentRow(row + 1);
    });

    //ADDITIONAL_SIGNALS
//    connect(reset_button_,&QPushButton::clicked,this,&TwoListSelection::reset_handler);
    connect(reset_button_,&QPushButton::clicked, [=]() {
            qDebug() << "Reset handler activated";
            mOutput->clear();
            mInput->clear();
            mInput->addItems(list_before_changes_);
        });
//    connect(cancel_button_,&QPushButton::clicked,this,&TwoListSelection::cancel_handler);
    connect(cancel_button_,&QPushButton::clicked, [=]() {
            this->close();
});
//    connect(ok_button_,&QPushButton::clicked,this,&TwoListSelection::ok_handler);
    connect(ok_button_,&QPushButton::clicked, [=]() {
            emit export_list(this->selectedItems());
            this->close();
    });
}

void TwoListSelection::setStatusButton() {
    mBtnUp->setDisabled(mOutput->selectedItems().isEmpty() ||
                        mOutput->currentRow() == 0);
    mBtnDown->setDisabled(mOutput->selectedItems().isEmpty() ||
                          mOutput->currentRow() == mOutput->count() - 1);
    mBtnMoveToAvailable->setDisabled(mInput->selectedItems().isEmpty());
    mBtnMoveToSelected->setDisabled(mOutput->selectedItems().isEmpty());
}

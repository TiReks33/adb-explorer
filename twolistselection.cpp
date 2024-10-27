#include "twolistselection.h"
#include "db_connection.h"


TwoListSelection::TwoListSelection(auth& auth__,QWidget *parent) :
    QDialog{parent}
  , auth_(auth__)
{
    init();
    connections();
}

TwoListSelection::~TwoListSelection()
{
    //qDebug() << "~TwoListSelection";
}

void TwoListSelection::addAvailableItems(const QStringList &items) {
    for( QString s : items)
        list_before_changes_.append(s);
    mInput->addItems(items);
    //qDebug() << "list::"<<list_before_changes_;
}

QStringList TwoListSelection::selectedItems() {
    QStringList selected;
    for (int i = 0; i < mOutput->count(); i++)
        selected << mOutput->item(i)->text();
    return selected;
}

void TwoListSelection::update_doublelist(QString const& query__,QString const& connection_name__)
{
    if(!db_connection::try_to_reopen(auth_,connection_name__)){
        qDebug() << QString("(x)There is error while update tables (connection is not established).");
        return;
    }
//    db_connection::set_query(QString("SHOW COLUMNS FROM `%1`").arg(QString(escape_sql_backticks(auth_.table_name_))), this);
    if(db_connection::set_query(query__, this, connection_name__))
        select_cells(0,0,mInput);

}



void TwoListSelection::init() {
    QHBoxLayout *layout = new QHBoxLayout(this);
    mInput = new QListWidget;
    mOutput = new QListWidget;

    mButtonToSelected = new QPushButton(">>");
    mBtnMoveToAvailable = new QPushButton(">");
    mBtnMoveToSelected = new QPushButton("<");
    mButtonToAvailable = new QPushButton("<<");


    ok_button_ = new QPushButton("OK");
    ok_button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ok_button_->setEnabled(false);

    cancel_button_ = new QPushButton("Cancel");
    cancel_button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    reset_button_ = new QPushButton("Reset");

    QHBoxLayout *actions_layout = new QHBoxLayout;


    actions_layout->addItem(new QSpacerItem(180, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    actions_layout->addWidget(ok_button_);


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


    QVBoxLayout *reset_layout = new QVBoxLayout;
    reset_layout->addLayout(layoutm);
    reset_layout->addWidget(reset_button_);

    input_layout->addLayout(/*layoutm*/reset_layout);
    input_sublayout->addLayout(input_layout);
    input_sublayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));
    input_sublayout->addLayout(cancel_layout);/*addWidget(reset_button_);*/
    layout->addLayout(input_sublayout);


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


    QVBoxLayout *sublayout = new QVBoxLayout;
    output_layout->addLayout(layoutl);
    sublayout->addLayout(output_layout);
    sublayout->addItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed));
    sublayout->addLayout(actions_layout);
    layout->addLayout(/*layoutl*/sublayout);


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

    // reset to condition before changes
    connect(reset_button_,&QPushButton::clicked, [=]() {
            //qDebug() << "Reset handler activated";
            mOutput->clear();
            mInput->clear();
            mInput->addItems(list_before_changes_);
            select_cells(0,0,mInput);
        });

    connect(cancel_button_,&QPushButton::clicked, [=]() {

            this->close();
    });

    connect(ok_button_,&QPushButton::clicked, [=]() {

            emit export_list(this->selectedItems());
            this->close();
    });

    // disabled export if right (output) list is empty
    connect(mOutput->model(),&QAbstractItemModel::rowsRemoved, [=]() {
            if(mOutput->count()==0)
                 ok_button_->setEnabled(false);
             //else
                 //ok_button_->setEnabled(true);
    });

    // enable export if something being inserted
    connect(mOutput->model(),&QAbstractItemModel::rowsInserted, [=]() {
            //if(mOutput->count()!=0)
                 ok_button_->setEnabled(true);
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

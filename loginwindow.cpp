#include "loginwindow.h"
#include "ui_loginwindow.h"

#include "blinkinbutton.h"

loginWindow::loginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginWindow)
    , db_window_(new Databases(auth_))
{
    ui->setupUi(this);
    this->setFixedSize(QSize(600, 325));
    move(screen()->geometry().center() - frameGeometry().center());
    ui->checkBox->setText("Hide");
    ui->checkBox->setChecked(true);
    ui->Host_Form->setText("localhost");

    ui->cc_label->setToolTip("<pre style=\"white-space: pre-wrap;\"><img src=':/pic/9rnvobslce_cc.png' height='16' width='16'><b>"
                        "[Non-Commercial License<br>   Requires Attribution]</b><br>This logo's "
                        "original image<br>was "
                        "designed by <b>Wannapik</b.><br>"
                        "       [URL:]<br>"
                        "<a>https://www.wannapik.com<br>"
                        "/vectors/22949</a></pre>");
    ui->cc_label->setToolTipDuration(60000);



    connect(this,SIGNAL(message_to_database_window(QString const&)),db_window_,SLOT(message_to_status(QString const&)));

//    int r = 5;
//    auto tr = QSharedPointer<int>::create(r);
//    std::shared_ptr<int> irt = std::make_shared<int>(r);
//    qDebug() << (*irt);
//    qDebug() << (*tr);

//    QPointer<auth> ty = new auth{};
//    ty.data()->db_name_ = "123";
//    qDebug() << ty.data()->db_name_ << ty.data()->db_server_;

////    QStringList list; list << "aba``"<<"baba```re" << "abc";
////    qDebug() << escape_sql_backticks(list);

//    SqlDump_db_choose db_choose{auth_};

//    db_choose.show();
//    db_choose.exec();

//QDialog dialog;

//QLabel* label = new QLabel;

//QVBoxLayout*layout = new QVBoxLayout;
//dialog.setLayout(layout);

//QPushButton* button = new QPushButton;

//button->setText("ABCDEF");

//QHBoxLayout* sublay = new QHBoxLayout;

////sublay->setSizeConstraint(QLayout::SetMinimumSize);

//dialog.layout()->addWidget(label);
//layout->addLayout(sublay);

//QComboBox* comboBox = new QComboBox;

//sublay->addWidget(comboBox);

//QPushButton *button1 = new QPushButton;

//QVBoxLayout* but_lay = new QVBoxLayout;

//but_lay->setSizeConstraint(QLayout::SetMinimumSize);

//button1->setLayout(but_lay);

//QLabel* but_label = new QLabel("ABCDEF");

//but_label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

//but_lay->addWidget(but_label);

//sublay->addWidget(button1);

//dialog.show();
//dialog.exec();



//    QDialog dialog;

//    QVBoxLayout* layout = new QVBoxLayout;
//    dialog.setLayout(layout);

//    QPushButton* button1 = new QPushButton("Button 1");
//    BlinkinButton* button2 = new BlinkinButton("Button 2");

//    layout->addWidget(button2);

//    layout->insertWidget(0,button1);

//    connect(button1,&QPushButton::clicked,[=]{
//        (!button2->is_active()) ? button2->start_blinkin(890) : button2->stop_blinkin();
//    });
////    button2->start_blinkin();

//    dialog.show();



//    dialog.exec();
}

loginWindow::~loginWindow()
{
    delete ui;
    delete db_window_;
}



void loginWindow::on_pushButton_clicked()
{
    auth_.login_=this->ui->Login_Form->text();
    auth_.passw_=this->ui->Password_Form->text();
    auth_.host_=this->ui->Host_Form->text();



        if(!db_connection::open(auth_))
            ui->statusbar->showMessage("(x)Authorization wrong. Please check your login details.");
        else{

            ui->statusbar->showMessage("(✓)Successful authorization");

            if(db_window_->show_databases()){
                db_window_->setModal(true);
                db_window_->show();
                emit message_to_database_window("SQL database autorization for user ::"+auth_.login_+":: succesfull.");

                this->hide();
            } else {
                ui->statusbar->showMessage(ui->statusbar->currentMessage()
                                           .append(".. but something goes wrong, and query to SQL DB failed.:("));
            }

        }

}


void loginWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->Password_Form->setEchoMode(QLineEdit::Password);
    else
        ui->Password_Form->setEchoMode(QLineEdit::Normal);

}




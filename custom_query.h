#ifndef CUSTOM_QUERY_H
#define CUSTOM_QUERY_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class Custom_Query;
}

class Custom_Query : public QDialog
{
    Q_OBJECT

public:
    explicit Custom_Query(QWidget *parent = nullptr);
    ~Custom_Query();

    QString get_text();

signals:

    void send_custom_query(QString);

    void send_custom_query(/*QString,*/Custom_Query*);

private slots:
    void on_setFont_Button_clicked();

//    void on_buttonBox_accepted();
    void on_Ok_button_clicked();

    void on_Cancel_button_clicked();

public slots:
    void close_window();

    void set_text(QString);

private:
    Ui::Custom_Query *ui;
};

#endif // CUSTOM_QUERY_H

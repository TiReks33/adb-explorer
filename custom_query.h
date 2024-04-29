#ifndef CUSTOM_QUERY_H
#define CUSTOM_QUERY_H

#include <QDialog>

namespace Ui {
class Custom_Query;
}

class Custom_Query : public QDialog
{
    Q_OBJECT

public:
    explicit Custom_Query(QWidget *parent = nullptr);
    ~Custom_Query();

signals:

    void send_custom_query(QString);

private slots:
    void on_setFont_Button_clicked();

    void on_buttonBox_accepted();

private:
    Ui::Custom_Query *ui;
};

#endif // CUSTOM_QUERY_H

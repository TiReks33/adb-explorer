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

    QString const get_text() const;

signals:

    void send_custom_query(QString const&);

    void send_custom_query(/*QString,*/Custom_Query *);


public slots:
    void close_window();

    void set_text(QString const&) const;

    void add_note(QString const&);

private:
    Ui::Custom_Query *ui;
    void connections_init();
};

#endif // CUSTOM_QUERY_H

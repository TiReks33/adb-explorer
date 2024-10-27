#ifndef NOTIFYCOMBOBOX_H
#define NOTIFYCOMBOBOX_H

#include <QComboBox>
#include <QObject>

class notifyComboBox : public QComboBox
{
    Q_OBJECT
public:
    notifyComboBox();
signals:
    void sameIndexRepeated();
public slots:
    virtual void setCurrentIndex(int index__);
};

#endif // NOTIFYCOMBOBOX_H

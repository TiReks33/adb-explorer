#ifndef HIDEMENU_H
#define HIDEMENU_H

#include <QMenu>
#include <QObject>
#include <QKeyEvent>

class hideMenu : public QMenu
{
    Q_OBJECT
public:
    explicit hideMenu(Qt::Key, QWidget* parent = nullptr);
    virtual ~hideMenu();

    Qt::Key inline currentHideKey() { return hideKey_;}

    inline void setHideKey(Qt::Key newKey__) { hideKey_ = newKey__; }

signals:
    void menuHiden();
private:
    Qt::Key hideKey_;

    void keyPressEvent(QKeyEvent *e);

};

#endif // HIDEMENU_H

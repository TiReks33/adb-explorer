#include "hidemenu.h"

hideMenu::hideMenu(Qt::Key key__, QWidget* parent__)
    : QMenu(parent__)
    , hideKey_{key__}
{

}

hideMenu::~hideMenu()
{

}

void hideMenu::keyPressEvent(QKeyEvent *e)
{
    auto key = e->key();
    if(key == hideKey_){
        if(isVisible()){
            hide();
            emit menuHiden();
        }
    } else{
        QMenu::keyPressEvent(e);
    }
}



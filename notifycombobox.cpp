#include "notifycombobox.h"

notifyComboBox::notifyComboBox()
{

}

void notifyComboBox::setCurrentIndex(int newIndex__)
{
    if(newIndex__==currentIndex())
        emit sameIndexRepeated();
    QComboBox::setCurrentIndex(newIndex__);
}

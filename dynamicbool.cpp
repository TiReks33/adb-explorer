#include "dynamicbool.h"

dynamicbool::dynamicbool(bool const& booly__, QObject *parent__) :
    QObject{parent__},
    booly_{booly__}
{

}

dynamicbool::dynamicbool(const dynamicbool & dynbool__, QObject *parent__)
    : QObject{parent__},
      booly_{dynbool__.booly_}
{
    //emit boolChanged();
}

dynamicbool& dynamicbool::operator=(const dynamicbool & dynbool__)
{
    if(&dynbool__ != this)
    {
        booly_ = dynbool__.booly_;
        emit boolChanged();
    }
    return *this;
}

void dynamicbool::synchronizeCheckBox(QCheckBox * const CB__) const
{
    if(CB__){
        auto dynamicCheck = connect(this, &dynamicbool::boolChanged,[CB__,this]{
            CB__->setChecked(*this);
        });

        connect(CB__,&QCheckBox::destroyed,[dynamicCheck]{
            disconnect(dynamicCheck);
        });
    }
}

dynamicbool& dynamicbool::operator=(const bool & booly__)
{
    if(booly__ != this->booly_)
    {
        booly_ = booly__;
        emit boolChanged();
    }
    return *this;
}

//bool dynamicbool::operator ==(const dynamicbool &__dynbool)
//{
//    return (booly_ = __dynbool.booly_);
//}

//bool dynamicbool::operator !=(const dynamicbool &__dynbool)
//{
//    return !(*this==__dynbool);
//}



#ifndef DYNAMICBOOL_H
#define DYNAMICBOOL_H

#include <QObject>
#include <QCheckBox>

class dynamicbool : public QObject
{
    Q_OBJECT
public:
    /*explicit*/ dynamicbool(bool const& = false, QObject *parent = nullptr);

    explicit dynamicbool(dynamicbool const &, QObject *parent = nullptr);

    dynamicbool & operator=(dynamicbool const &);

    dynamicbool & operator=(bool const &);

    //bool operator ==(dynamicbool const&);

    //bool operator !=(dynamicbool const&);

    inline bool operator ==(const dynamicbool &__dynbool)
    {
        return (booly_ = __dynbool.booly_);
    }

    inline bool operator !=(const dynamicbool &__dynbool)
    {
        return !(*this==__dynbool);
    }

    inline bool toBool() const { return booly_; };

    inline operator bool() const  { return booly_; }

    void synchronizeCheckBox(QCheckBox * const = nullptr) const;

    virtual ~dynamicbool(){}

signals:
    void boolChanged();

private:
    bool booly_;
};

#endif // DYNAMICBOOL_H

#ifndef SIGNALTABLEVIEW_H
#define SIGNALTABLEVIEW_H

#include <QTableView>
#include <QObject>
#include <QDebug>
#include <QHeaderView>

class signalTableView : public QTableView
{
    Q_OBJECT

public:
    explicit signalTableView(QWidget* parent__ = nullptr);
    virtual ~signalTableView();

signals:
    void model_changed();

    void tableRescaled();

public slots:
    virtual void setModel(QAbstractItemModel *model) override;

    // 'silent' parameter defines that this slot execution not (true) or will (false) notification excorted
    void setRescale(QHeaderView::ResizeMode h,QHeaderView::ResizeMode v, bool silent=true);

    void setHorRescale(QHeaderView::ResizeMode);

    void setVerRescale(QHeaderView::ResizeMode);

    inline QHeaderView::ResizeMode getHorScaleMode(){
        return this->horScale_;
    }

    inline QHeaderView::ResizeMode getVerScaleMode(){
        return this->verScale_;
    }



private:
    QHeaderView::ResizeMode horScale_ = QHeaderView::Stretch;
    QHeaderView::ResizeMode verScale_ = QHeaderView::ResizeToContents;
};

#endif // SIGNALTABLEVIEW_H

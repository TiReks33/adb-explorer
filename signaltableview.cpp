#include "signaltableview.h"

signalTableView::signalTableView(QWidget* parent__)
    : QTableView(parent__)
{

    connect(this,&signalTableView::model_changed,[this]{

        auto tooltip = model()->headerData(0,Qt::Horizontal,Qt::DisplayRole).toString();

        for(auto i=0; i!=model()->columnCount();++i){
            this->model()->setHeaderData(i,Qt::Horizontal,(model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()),Qt::ToolTipRole);
        }

    });

}

signalTableView::~signalTableView()
{

}


void signalTableView::setModel(QAbstractItemModel *newModel)
{
    QTableView::setModel(newModel);

    emit model_changed();
}

void signalTableView::setRescale(QHeaderView::ResizeMode newHorScale__, QHeaderView::ResizeMode newVerScale__, bool silent)
{
    this->horizontalHeader()->setSectionResizeMode(newHorScale__);
    this->horScale_ = newHorScale__;
    this->verticalHeader()->setSectionResizeMode(newVerScale__);
    this->verScale_ = newVerScale__;

    if(!silent)
        emit tableRescaled();
}

void signalTableView::setHorRescale(QHeaderView::ResizeMode newHorScale__)
{
    this->horizontalHeader()->setSectionResizeMode(newHorScale__);
    this->horScale_ = newHorScale__;
}

void signalTableView::setVerRescale(QHeaderView::ResizeMode newVerScale__)
{
    this->verticalHeader()->setSectionResizeMode(newVerScale__);
    this->verScale_ = newVerScale__;
}



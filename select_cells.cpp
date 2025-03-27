#include "select_cells.hpp"

void select_cells(const QModelIndex &index, QAbstractItemView *itemView)
{
    QItemSelectionModel *selectionModel = itemView->selectionModel();

    selectionModel->select(index, QItemSelectionModel::ClearAndSelect);

    itemView->setCurrentIndex(itemView->model()->index(index.row(),index.column()));
}

void select_cells(int ai, int aj, QAbstractItemView *itemView)
{
    QItemSelectionModel *selectionModel = itemView->selectionModel();

    selectionModel->select(itemView->model()->index(ai, aj), QItemSelectionModel::ClearAndSelect);

    itemView->setCurrentIndex(itemView->model()->index(ai,aj));
}

void select_cells(const QModelIndex &index,const QModelIndex &index2, QAbstractItemView *itemView)
{

    QItemSelectionModel *selectionModel = itemView->selectionModel();

    itemView->setCurrentIndex(itemView->model()->index(index2.row(),index2.column()));

    QItemSelection selection(index, index2);

    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

}

void select_cells(int ai, int aj, int bi, int bj, QAbstractItemView *itemView)
{
    QItemSelectionModel *selectionModel = itemView->selectionModel();

    QModelIndex a = itemView->model()->index(ai, aj, QModelIndex());
    QModelIndex b = itemView->model()->index(bi, bj, QModelIndex());

    itemView->setCurrentIndex(itemView->model()->index(bi,bj));

    QItemSelection selection(a, b);

    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

}



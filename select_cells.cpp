#include "select_cells.hpp"

void select_cells(const QModelIndex &index, QAbstractItemView *itemView)
{
    QItemSelectionModel *selectionModel = itemView->selectionModel();

         selectionModel->select(index, QItemSelectionModel::ClearAndSelect);

//         QString val=ui->tableView->model()->data(index).toString();
         //         ui->statusLine->setText("Database current index after select_cells: "+val);

         ////SETUP CURRENT INDEX POSITION
         itemView->setCurrentIndex(itemView->model()->index(index.row(),index.column()));
}

void select_cells(int ai, int aj, QAbstractItemView *itemView)
{
    QItemSelectionModel *selectionModel = itemView->selectionModel();

         selectionModel->select(itemView->model()->index(ai, aj/*, QModelIndex()*/), QItemSelectionModel::ClearAndSelect);

         //SETUP CURRENT INDEX POSITION
         itemView->setCurrentIndex(itemView->model()->index(ai,aj));
}

void select_cells(const QModelIndex &index,const QModelIndex &index2, QAbstractItemView *itemView)
{
    QItemSelectionModel *selectionModel = itemView->selectionModel();

//    QModelIndex topLeft;
//    QModelIndex bottomRight;

//    topLeft = ui->tableView->model()->index(0, 0, QModelIndex());
//    bottomRight = ui->tableView->model()->index(2, 0, QModelIndex());

    //SETUP CURRENT INDEX POSITION (2nd index)
    itemView->setCurrentIndex(itemView->model()->index(index2.row(),index2.column()));

    QItemSelection selection(index, index2);
         selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

//         QString val=ui->tableView->model()->data(index).toString();
         //         ui->statusLine->setText("Database current index after select_cells: "+val);


}

void select_cells(int ai, int aj, int bi, int bj, QAbstractItemView *itemView)
{
    QItemSelectionModel *selectionModel = itemView->selectionModel();

        QModelIndex a = itemView->model()->index(ai, aj, QModelIndex());
        QModelIndex b = itemView->model()->index(bi, bj, QModelIndex());

        ////SETUP CURRENT INDEX POSITION (second 'b' index)
        itemView->setCurrentIndex(itemView->model()->index(bi,bj));

    QItemSelection selection(a, b);
         selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);


}

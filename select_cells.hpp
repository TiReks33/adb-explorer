#ifndef SELECT_CELLS_H
#define SELECT_CELLS_H
#include <QItemSelectionModel>
#include <QAbstractItemView>

void select_cells(const QModelIndex &index, QAbstractItemView *itemView);

void select_cells(int ai, int aj, QAbstractItemView *itemView);

void select_cells(const QModelIndex &index,const QModelIndex &index2, QAbstractItemView *itemView);

void select_cells(int ai, int aj, int bi, int bj, QAbstractItemView *itemView);

#endif // SELECT_CELLS_H

#ifndef TWOLISTSELECTION_H
#define TWOLISTSELECTION_H

#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QDialog>
#include <QCloseEvent>

#include "auth.h"

#include "select_cells.hpp"

//#include "multi_connection.h"
#include "helping_stuff.h"

// Custom form to iterate trough 2x lists
class TwoListSelection : public QDialog {
  Q_OBJECT
public:
  explicit TwoListSelection(auth& auth__,QDialog *parent = nullptr);
    virtual ~TwoListSelection();

  // save previous elements and adding new to the left (input) list
  void addAvailableItems(const QStringList &items);

  // returns right (output) selected list of items
  QStringList selectedItems();

  // clear all
  void clear(){ mOutput->clear();mInput->clear();list_before_changes_.clear();}

  // set query result as input list
  void update_doublelist(QString const&,QString const& = auth::con_name_);

signals:

    // transfer right (output) list to external object
    void export_list(QStringList);

private:
  void init();

  void connections();

  // change buttons active status if no output items selected
  void setStatusButton();

  QListWidget *mInput;
  QListWidget *mOutput;

  QStringList list_before_changes_;

  QPushButton *mButtonToAvailable;
  QPushButton *mButtonToSelected;

  QPushButton *mBtnMoveToAvailable;
  QPushButton *mBtnMoveToSelected;

  QPushButton *mBtnUp;
  QPushButton *mBtnDown;

  QPushButton *ok_button_;
  QPushButton *cancel_button_;
  QPushButton *reset_button_;

  auth& auth_;
  //multi_connection multi_con_;
};

#endif // TWOLISTSELECTION_H

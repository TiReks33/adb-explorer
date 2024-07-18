#ifndef TWOLISTSELECTION_H
#define TWOLISTSELECTION_H

#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QDialog>

#include "auth.h"

#include "multi_connection.h"

class TwoListSelection : public QDialog {
  Q_OBJECT
public:
  explicit TwoListSelection(auth& auth__,QDialog *parent = nullptr);

  void addAvailableItems(const QStringList &items);

  QStringList selectedItems();

  void clear(){ mOutput->clear();mInput->clear();list_before_changes_.clear();}

  void update_doublelist();

private slots:

//  void reset_handler();

//  void cancel_handler(){this->close(); }

//  void ok_handler();

public slots:

    void update_doublelist_handler();

signals:

    void export_list(QStringList);

private:
  void init();

  void connections();

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
  multi_connection multi_con_;
};

#endif // TWOLISTSELECTION_H

#ifndef TWOLISTSELECTION_H
#define TWOLISTSELECTION_H

#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>
#include <QDebug>

class TwoListSelection : public QWidget {
  Q_OBJECT
public:
  explicit TwoListSelection(QWidget *parent = nullptr);

  void addAvailableItems(const QStringList &items);

  QStringList selectedItems();

  void clear(){ mOutput->clear();mInput->clear();list_before_changes_.clear();}

private slots:

//  void reset_handler();

//  void cancel_handler(){this->close(); }

//  void ok_handler();

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
};

#endif // TWOLISTSELECTION_H

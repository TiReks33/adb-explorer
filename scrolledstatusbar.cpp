#include "scrolledstatusbar.h"


statusLineEdit::statusLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    ////set_default();
    setPlaceholderText(default_str_);
}

void statusLineEdit::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton){
        //setCursorPosition(text().size()-1);
        selectAll();
        selectAll();
        emit rightClicked();
    } else
    QLineEdit::mousePressEvent(e);
}


scrolledStatusBar::scrolledStatusBar(QWidget *parent)
    : QWidget(parent)
    , statusLine(new statusLineEdit)
    , hor_scrollbar(new QScrollBar(Qt::Horizontal))
{
    QVBoxLayout* main_lay = new QVBoxLayout;

    setLayout(main_lay);

    statusLine->setTextMargins(0,0,0,0);

    main_lay->addWidget(statusLine);
    main_lay->addWidget(hor_scrollbar);

    statusLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    hor_scrollbar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    main_lay->setSizeConstraint(QLayout::SetMinimumSize);

    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    connect(statusLine,&QLineEdit::textChanged,[=](QString const& text__){
        QFontMetrics fm(statusLine->font());
        width = fm.horizontalAdvance(text__);

        if(width>=/*hor_scrollbar->width()-20*/statusLine->size().width()-20)
            hor_scrollbar->show();
        else{
            hor_scrollbar->hide();
            ////this->adjustSize();
        }

//        if(text__.isEmpty())
//            get_line()->set_default();

    });

    // synchronize
    connect(hor_scrollbar, &QScrollBar::valueChanged, statusLine, [&](int v) {
      auto scrollbarAt = static_cast<double>(v) / hor_scrollbar->maximum();
      auto cursorTo = std::round(statusLine->text().length() * scrollbarAt);
      statusLine->setCursorPosition(cursorTo);
    });

    hor_scrollbar->setPageStep(1);
     hor_scrollbar->setMaximum(statusLine->text().length());
     connect(hor_scrollbar, &QScrollBar::valueChanged,
             statusLine, &QLineEdit::setCursorPosition);
     //this can also ben done on textChanged, however for the price
     //of more frequent execution...
     connect(statusLine, &QLineEdit::cursorPositionChanged,
             hor_scrollbar, [&](int, int n) {
               hor_scrollbar->setMaximum(statusLine->text().length());
               //...one gets the easy way to track the cursor
               //with the slider
               hor_scrollbar->setValue(n);
             });

     hor_scrollbar->hide();



     statusLine->setReadOnly(true);

}

scrolledStatusBar::~scrolledStatusBar()
{
    delete hor_scrollbar;
    delete statusLine;
}

void scrolledStatusBar::resizeEvent(QResizeEvent *event)
{
    //refresh();
//    qDebug() << hor_scrollbar->width() << hor_scrollbar->size().width() << hor_scrollbar->size().height();
    QWidget::resizeEvent(event);

    if(width>=/*hor_scrollbar->width()-20*/statusLine->size().width()-20)
        hor_scrollbar->show();
    else{
        hor_scrollbar->hide();
        ////this->adjustSize();
    }
}



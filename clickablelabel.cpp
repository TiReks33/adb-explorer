#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QString const& LabelText, QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
setWindowFlags(f);
setText(LabelText);
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
    event->accept();
}

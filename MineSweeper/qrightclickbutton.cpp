#include "qrightclickbutton.h"

QRightClickButton::QRightClickButton(QWidget *parent) :
    QPushButton(parent)
{
}

void QRightClickButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton)
        emit setRightClicked();
    else if(e->button() == Qt::MiddleButton)
        emit setMiddleClicked();
    else
    {
        QPushButton::mousePressEvent(e);
    }
}

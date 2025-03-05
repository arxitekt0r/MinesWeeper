#include "custombutton.h"


CustomButton::CustomButton(QWidget *parent)
    : QPushButton(parent)
{

}

void CustomButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        emit rightClicked();
    }
    else if (event->button() == Qt::LeftButton)
    {
        emit leftClicked();
    }
    else
    {
        QPushButton::mousePressEvent(event);
    }
}

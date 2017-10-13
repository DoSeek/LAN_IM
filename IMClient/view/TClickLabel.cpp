/*********************************************
File Name： TClickLabel.cpp
Author： jet.F.R
Date： 2014.3.10
Description： 具有单击相应的标签类
Changes：
********************************************/

#include "stable.h"
#include "TClickLabel.h"


TClickLabel::TClickLabel(QWidget *parent) :
    QLabel(parent)
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    setPalette(pa);
}

TClickLabel::TClickLabel(const QString &text, QWidget *parent) :
    QLabel(parent)
{
    setText(text);
}


void TClickLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
        emit clicked();//(this);
}

void TClickLabel::enterEvent(QEvent * )
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::blue);
    setPalette(pa);
}

void TClickLabel::leaveEvent(QEvent *)
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    setPalette(pa);
}

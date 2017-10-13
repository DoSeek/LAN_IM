/*********************************************
File Name： TClickLabel.h
Author： jet.F.R
Date： 2014.3.10
Description： 具有单击相应的标签类
Changes：
********************************************/

#ifndef IMCLICKLABEL_H
#define IMCLICKLABEL_H

#include <QLabel>


/*************************************************
Class Name： TClickLabel
Description: 具有单击相应的标签类
*************************************************/
class TClickLabel : public QLabel
{
    Q_OBJECT
public:
    explicit TClickLabel(QWidget *parent = 0);
    TClickLabel(const QString &text, QWidget *parent=0);
signals:
    // 鼠标单击信号
    void clicked();//TClickLabel* label);

protected:
    // 鼠标单击事件
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
public slots:
    
};

#endif // IMCLICKLABEL_H

/*********************************************
File Name： IMServerWindow.h
Author： jet.F.R
Date： 2014.3.10
Description： 服务器主界面类
Changes：
********************************************/

#ifndef IMSERVERWINDOW_H
#define IMSERVERWINDOW_H

// Coding: UTF-8(BOM)
#if ((_MSC_VER) && (_MSC_VER) >=1600)
    #pragma execution_character_set("utf-8")
#endif

#include <QWidget>

class IMServerMessageCtrl;

/*************************************************
Class Name： IMServerWindow
Description: 服务器主界面类
*************************************************/
class IMServerWindow : public QWidget
{
    Q_OBJECT

public:
    IMServerWindow(QWidget *parent = 0);
    ~IMServerWindow();

private:
        IMServerMessageCtrl *m_serverCtrl;
};

#endif // IMSERVERWINDOW_H

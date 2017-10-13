/*********************************************
File Name： TClientSocketCtrl.h
Author： jet.F.R
Date： 2014.3.10
Description： 用于控制客户端socket的类
Changes：
********************************************/

#ifndef TCLIENTSOCKETCTRL_H
#define TCLIENTSOCKETCTRL_H

#include <QTcpSocket>
#include "TDatabaseCtrl.h"
#include "TConstant.h"

class QDateTime;


/*************************************************
Class Name： TClientSocketCtrl
Description: 来自客户端socket，用于处理各种请求
*************************************************/
class TClientSocketCtrl : public QTcpSocket
{
    Q_OBJECT
public:
    TClientSocketCtrl(QObject *parent = 0);
    ~TClientSocketCtrl();

signals:
    // 请求server处理
    void sendSignal(const TSaveTmpInformation &temp);
    // 用户登录
    void userLoginSignal(const TUserInformation &user);
    //用户断开连接了
    void deleteSignal(const QString &acc);

public slots:
    // 接收信息
    void receiveMessage();
    // 发送信息
    void sendMessage(const TSaveTmpInformation &temp);
    // 删除socket
    void deleteScoket();

private:
    quint16 mnBlockSize;
    TSaveTmpInformation mSaveTmpInfo;

private:
    TDatabaseCtrl mDatabaseCtrl;

};

#endif // TCLIENTSOCKETCTRL_H

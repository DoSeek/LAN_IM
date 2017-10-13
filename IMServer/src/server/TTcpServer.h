/*********************************************
File Name： TTcpServer.h
Author： jet.F.R
Date： 2014.3.10
Description： 服务器类
Changes：
********************************************/

#ifndef IMTCPSERVER_H
#define IMTCPSERVER_H

#include <QTcpServer>
#include "TClientSocketCtrl.h"
#include "TConstant.h"
#include "TDatabaseCtrl.h"

/*************************************************
Class Name： TTcpServer
Description: 服务器
*************************************************/
class TTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TTcpServer(QObject *parent = 0);
    ~TTcpServer();

public slots:
    // 客户端断开连接
    void clientDisconnected(const QString &id);
    // 要求clientSocket发送信息
    void sendMessage(const TSaveTmpInformation &temp);

signals:
    void haveNewID(const QString & id);
    void haveNewAddress(const QString & address);
    void haveLeftID(const QString & id);
    void haveLeftAddress(const QString & address);

private:
    // 当有新的连接时，会调用此函数。用于创建新的线程去控制socket通信
    void incomingConnection(int socketDescriptor);

    // 处理好友请求
    void friendRequest(const TSaveTmpInformation & save);
    // 处理群请求
    void flockRequest(const TSaveTmpInformation & save);
    // 处理讨论组请求
    void discussionRequest(const TSaveTmpInformation & saves);
    // 处理对话请求
    void talkRequest(const TSaveTmpInformation & save);


    // 通知好友，改变用户状态
    void tellFriendsStatusChanged(const QString &id, int status);
    // 通知好友，改变用户信息
    void tellFriendsInformationChanged(const QString & id);//, const TFriendInformation &fri);
    // 通知群成员，群被解散
    void tellMembersFlockDroped(const QString & flockID);
    // 通知讨论组成员， 讨论组被解散了
    void tellMembersDiscussionDroped(const QString & discussionID);
    // 通知群成员，群信息被改变
    void tellMembersFlockChanged(const TFlockInformation &);
    // 通知讨论组成员，讨论组信息被修改了
    void tellMembersDiscussionChanged(const TDiscussionInformation &);

    // 通知群成员， 有新的成员
    void tellFlockMemberHaveNewMember(const QString &flockID);
    // 通知群成员， 有成员退出
    void tellFlockMemberHaveLeavedMember(const QString & flockID);

    // 通知群成员， 有新的成员
    void tellDiscussionMemberHaveNewMember(const QString & discussionID);
    // 通知讨论组成员， 有成员退出
    void tellDiscussionMemberHaveLeavedMember(const QString & discussionID);

private:
    // 临时信息
    TSaveTmpInformation mSaveTmpInfo;
    // 操作数据库
    TDatabaseCtrl mDatabaseCtrl;
    // 在线用户列表
    QMap<QString, TClientSocketCtrl*> mmapOnlineUser;
    // 在线用户状态
    QMap<QString, int> mmapOnlineUserStatus;

    // 邮箱在线的用户
    QMap<QString, TClientSocketCtrl*> mmapOnlineUserMail;
};

#endif // IMTCPSERVER_H


/*********************************************
File Name： IMMainCtrl.h
Author： jet.F.R
Date： 2014.3.26
Description： 用于控制主界面的类
Changes：
********************************************/
#ifndef IMMAINCTRL_H
#define IMMAINCTRL_H

#include <QObject>
#include <QMultiMap>
#include <QVector>
#include "TConstant.h"
#include "stable.h"

class IMTcpSocket;

class IMMainCtrl : public QObject
{
    Q_OBJECT
public:
    IMMainCtrl(const QString myID, QObject *parent = 0);
    
    // 关闭链接
    void closeConnect();

    // 获取所有好友列表信息
    void getFriendsInformation(const QString &, const int);
    //  获取群信息列表
    void getFlocksInformation(const QString &);
    // 获取讨论组信息列表
    void getDiscussionsInformation(const QString &);
    // 获取本地最近消息
    void getLatestMessage(const QString &);

    // 获取群所有成员
    void getFlockMembers(const QString &);
    // 获取讨论组所有成员
    void getDiscussionMembers(const QString &);

    // 发送好友聊天信息
    void sendTalkMessage(TTalkMessage &);

    // 获取好友详细信息
    void getFriendInformation(const QString &id);
    // 获取自己详细信息
    void getMynformation(const QString &id);
    // 修改自己的信息
    void changeMyInformation(const TUserInformation &me);
    // 修改自己的状态
    void changeMyStatus(const QString id, const int status);
    // 修改好友备注
    void changeFriendRemark(const QString & myID,
                            const QString & friendID,
                            const QString &remark);

    // 删除好友
    void deleteFriend(const QString & myID, const QString & friendID);
    // 添加好友
    void addFriend(const TTalkMessage & mes);//const TTempStrings &);
    // 加入群
    void addFlock(const TTalkMessage & mes);
    // 加入讨论组
    void addDiscussion(const TTalkMessage & mes);
    // 解散群
    void dropFlock(const QString &, const QString &);
    // 解散讨论组
    void dropDiscussion(const QString &, const QString &);
    // 退出群
    void leaveFlock(const QString &, const QString &);
    // 退出讨论组
    void leaveDiscussion(const QString &, const QString & discussionID);
    // 创建群
    void createFlock(const TTempStrings & tmpStr);
    // 创建讨论组
    void createDiscussion(const TTempStrings & tmpStr);

    // 修改群信息
    void changeFlockInformation(const TFlockInformation &);
    // 修改群成员名片
    void changeFlockMemberRemark(const TTempStrings & tmpStr);

    // 修改讨论组信息
    void changeDiscussionInformation(const TDiscussionInformation &);

    // 好友请求结果
    void resultOfFriendRequest(const TTalkMessage & mes);
    // 群请求结结果
    void resultOfFlockRequest(const TTalkMessage & mes);
    // 删除分组
    void removeBox(const QString &);
    // 重命名分组
    void renameBox(const QString &, const QString &);
    // 移动好友至其他分组
    void moveFriendToBox(const QString &, const QString &, const QString &);

    /**************** network ****************/

signals:
    void getFriendsSuccess(const QVector<TFriendInformation> &);
    void getFlocksSuccess(const QVector<TFlockInformation> &);
    void getDiscussionsSuccess(const QVector<TDiscussionInformation> &);
    void getFlockMembersSuccess(const QVector<TFlockMember> &);
    void getDiscussionMembersSuccess(const QVector<TDiscussionMember> &);

    void getFriendTalkMessage(const TTalkMessage &);
    void getFriendChangedStatus(const QString &, const int);
    void getFriendChangedInformation(const TFriendInformation &);
    void getFriendInformationSuccess(const TUserInformation &);
    void getMyInformationSuccess(const TUserInformation &);
    void changeMyInformationSuccess(const TUserInformation &);
    void changeFriendRemarkSuccess(const TTempStrings &);
    void deleteFriendSuccess(const QString &, const QString &, const QString &);
    void getFriendRequest(const TTalkMessage &, const TUserInformation &);

    void getFlockRequest(const TTalkMessage &, const TFlockMember &,
                         const TFlockInformation &);
    void dropFlockSuccess(const QString &);
    void leaveFlockSuccess(const QString &, const QString &, const QString &);
    void haveMemberleavedFlock(const QString &, const QString &, const QString &);
    void changeFlockSuccess(const TFlockInformation &);
    void changeFlockRemarkSuccess(const TTempStrings &);
    void createFlockSuccess(const TFlockInformation &);

    void getDiscussionRequest(const TTalkMessage &, const TDiscussionMember &,
                              const TDiscussionInformation &);
    void dropDiscussionSuccess(const QString &);
    void leaveDiscussionSuccess(const QString &, const QString &);//, const QString &);
    void changeDiscussionSuccess(const TDiscussionInformation &);
    void createDiscussionSuccess(const TDiscussionInformation &);
    void renameBoxSuccess(const QString &, const QString &);
    void moveFriendToBoxSuccess(const QString &, const QString &, const QString);

    void getNetworkHistoryMessagesSuccess(const QString & friendID, const QDate & date,
                                   const QVector<TTalkMessage> &);
    void closeWindowSignal();

public slots:
    // 发送获取所有好友信息的请求
    void requestGetFriendsInformation();
    // 发送获取所有群信息的请求
    void requestGetFlocksInformation(const QString &);
    // 发送获取所有讨论组信息的请求
    void requestGetDiscussionsInformation(const QString &);
    // 发送获取群所有成员的请求
    void requestGetFlockMembers(const QString &);
    // 发送获取讨论组所有成员的请求
    void requestGetDiscussionMembers(const QString &);

    // 发送获取好友详细信息的请求
    void requestGetFriendInformation(const QString &);
    // 发送获取自己详细信息的请求
    void requestGetMyInformation();
    // 发送修改自己信息的请求
    void requestChangeMyInformation(const TUserInformation &);
    // 发送修改自己状态的请求
    void requestChangeMyStatus(const QString id, const int status);
    // 发送修改好友备注的请求
    void requestChangeFriendRemark(const QString & myID,
                                   const QString & friendID,
                                   const QString &remark);
    // 发送删除好友的请求
    void requestDeleteFriend(const QString & myID, const QString & friendID);
    // 发送添加好友的请求
    void requestAddFriend(const TTalkMessage & mes);
    // 发送加入群的请求
    void requestAddFlock(const TTalkMessage & mes);
    // 请求加入讨论组的请求
    void requestAddDiscussion(const TTalkMessage & mes);
    // 发送解散群的请求
    void requestDropFlock(const QString &, const QString &);
    // 发送解散讨论组的请求
    void requestDropDiscussion(const QString &, const QString &);

    // 发送退出群的请求
    void requestLeaveFlock(const QString &, const QString &);

    // 发送退出讨论组的请求
    void requestLeaveDiscussion(const QString &, const QString &);

    // 发送创建群的请求
    void requestCreateFlock(const TTempStrings &);
    // 发送创建群的请求
    void requestCreateDiscussion(const TTempStrings &);

    // 发送处理好友请求结果的请求
    void requestSendResultOfFriendRequest(const TTalkMessage & mes);
    // 发送处理群请求结果的请求
    void requestSendResultOfFlockRequest(const TTalkMessage & mes);

    // 发送修改群信息的请求
    void requestChangeFlockInformation(const TFlockInformation &);
    // 发送修改群成员名片的请求
    void requestChangeFlockMemberRemark(const TTempStrings & tmpStr);

    // 发送修改讨论组信息的请求
    void requestChangeDiscussionInformation(const TDiscussionInformation &);

    // 发送好友对话信息请求
    void requestSendTalkMessage();

    // 发送删除分组的请求
    void requestRemoveBox(const QString &);
    // 发送重命名分组的请求
    void requestRenameBox(const QString &, const QString &);
    // 发送移动好友至其他分组的请求
    void requestMoveFriendToBox(const QString & friendID, const QString & oldTitle,
                                const QString & newTitle);

    // 获取某日某好友在服务器的消息
    void requestGetNetWorkHistoryMessageOneFriendOneDate(
            const QString &, const QDate &);

    // 发送修改密码的请求
    void requestChangePwd(const QString & oldPwd, const QString & newPwd);

    // 读取信息
    void readMessage();

    // 关闭程序
    void closeWindow();


private:
    QString m_myID;

    IMTcpSocket *m_tcpSocket;
    quint16 m_blockSize;
    QString m_id;
    int m_status;
    int m_peerStatus;

    QVector<TTalkMessage> m_messageVector;

    QVector<TTalkMessage> m_HistoryMessageVector;
    QVector<TTalkMessage> m_OfflineMessageVector;

    QString m_creatorID;
    QString m_flockID;
    QString m_discussionID;
    TFlockMember m_flockMember;
    TDiscussionMember m_discussionMember;
    TFlockInformation m_flockInf;
    TDiscussionInformation m_discussionInf;

    QVector<TFriendInformation> m_friendsVec;
    QVector<TFlockInformation> m_flocksVec;
    QVector<TDiscussionInformation> m_discussionsVec;
    QVector<TFlockMember> m_flockMembersVec;
    QVector<TDiscussionMember> m_discussionMembersVec;

    TTalkMessage m_message;
    QString m_peerID;
    QString m_groupName;
    QString m_friendID;
    TUserInformation m_myInf;
    TUserInformation m_friInf;
    TFriendInformation m_friSimInf;
    TTempStrings m_tempStr;

    QString m_title;
    QString m_newTitle;
    QDate m_date;

};

#endif // IMMAINCTRL_H

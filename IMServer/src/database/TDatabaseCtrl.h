/*********************************************
File Name： TDatabaseCtrl.h
Author： jet.F.R
Date： 2014.3.12
Description： 操作数据库
Changes：
********************************************/
#ifndef TDATABASECTRL_H
#define TDATABASECTRL_H

#include <QObject>
#include <QMap>
#include <QSqlError>
#include "TConstant.h"

class QSqlQuery;
class QSqlDatabase;
//class IMUser;
class QDateTime;


/*************************************************
Class Name： TDatabaseCtrl
Description: 数据库操作类
*************************************************/
class TDatabaseCtrl : public QObject
{
    Q_OBJECT
public:
    TDatabaseCtrl(QObject *parent = 0);

    ~TDatabaseCtrl();

    // 创建连接
    bool createConnection();
    // 初始化数据库 将所有用户的状态status设置为OFFLINE
    bool initDatabase();
    // 关闭数据库
    void close();
    // 打开数据库
    bool isOpen();

    // 修改密码
    int updatePassword(TTempStrings & tmp);
    // 查询密保问题以及答案
    int searchQuestionAndAnswer(const QString &id, TTempStrings & tmp);
    // 查询用户
    int searchUserInUserInformation(const TLoginInformation &logInf,
                                       TUserInformation &userInf);

    // 通过id查询好友简易信息
    int searchFriendSimpleInformationByID(const QString &id,
                                          TFriendInformation &friInf);

    // 通过id查询好友信息
    int searchFriendInformationByID(const QString &id, TUserInformation &userInf);
    // 通过id查询自己信息
    int searchMyselfInformationByID(const QString &id, TUserInformation &userInf);
    // 修改用户信息
    int updateMyselfInformation(const TUserInformation &user);
    // 添加用户
    int addUserInUserInformation(const TUserInformation &userInf);

    // 获取一段时间注册数量
    int countRegisteredInPeriod(const QDateTime &datePre,
                                 const QDateTime &dateLater);
    // 获取一段时间群添加数量
    int countFlockInPeriod(const QDateTime &datePre,
                                 const QDateTime &dateLater);
    // 获取所有好友信息
    int searchFriendsInformation(const QString &id,
                                  QVector<TFriendInformation> &friendsVec);
    // 获取所有好友ID
    bool searchFriendsID(const QString &id, QVector<QString> &IDVec);
    // 更新好友状态
    bool updateStatus(int status, const QString & id);
    // 更新好友备注
    int updateRemark(const TTempStrings & tempStr);
    // 删除好友
    int deleteFriend(const QString & myID, const QString & friendID,
                       QString & groupName, QString & groupNameF);
    // 添加好友
    bool addFriend(const TTalkMessage & mes);

    // 获取某用户加入的所有群
    int searchFlocksInformation(const QString &, QVector<TFlockInformation> &);
    // 获取某个群信息
    bool searchFlockInformation(const QString &, TFlockInformation &);
    // 获取某群中所有的成员
    int searchFlockMembers(const QString &, QVector<TFlockMember> &);
    // 获取某群的某个成员
    bool searchFlockMember(const QString &, const QString &, TFlockMember & flockMember);


    // 获取所有用户加入的讨论组
    int searchDiscussionsInformation(const QString &,
                                       QVector<TDiscussionInformation> &);
    // 获取某个讨论组信息
    bool searchDiscussionInformation(const QString &, TDiscussionInformation &);
    // 获取某讨论组中所有的成员
    int searchDiscussionMembers(const QString &, QVector<TDiscussionMember> &);
    // 获取某讨论组中的某个成员
    bool searchDiscussionMember(const QString & discussionID,
                               const QString & userID,
                               TDiscussionMember & discussionMember);


    // 获取群主帐号
    bool searchFlockHost(const QString & flockID, QString & hostID);
    // 添加群成员
    bool addFlockMember(const QString & flockID, const QString & memberID,
                        const int status = EFS_FLOCK_MEMBER);
    // 获取组长帐号
    bool searchDiscussionHost(const QString & discussionID, QString & hostID);
    // 添加讨论组成员
    bool addDiscussionMember(const QString & discussionID, const QString & memberID,
                            const int status = EDS_DISCUSSION_MEMBER);
    // 添加群
    int addFlock(const TTempStrings &, QString & flockID);
    // 添加讨论组
    int addDiscussion(const TTempStrings &, QString & discussionID);
    // 删除群
    int deleteFlock(const QString userID, const QString &flockID);
    // 删除讨论组
    int deleteDiscussion(const QString userID, const QString &discussionID);
    // 删除群成员
    int deleteFlockMember(const QString & flockID, const QString & memberID);
    // 删除讨论组成员
    int deleteDiscussionMember(const QString & discussionID, const QString & memberID);

    // 更新群信息
    int updateFlockInformation(const TFlockInformation & flock);
    // 更新讨论组信息
    int updateDiscussionInformation(const TDiscussionInformation & discussion);

    // 更新群成员名片
    int updateFlockMemberRemark(const TTempStrings & tmpStr);

    // 更新讨论组成员名片---暂时没用
    // int updateDiscussionMemberRemark(const TTempStrings & tmpStr);

    // 检测用户---暂时没用
    // int checkUser(const TLoginInformation &logInf);

    //插入DateCounterb表某日的数据项
    bool insertValuesInDateCounter(const QDate &);
    // 获取某日用户注册数量+1
    int getUserTimes(const QDate &);
    // 获取某日新建群数量+1
    int getFlockTimes(const QDate &);
    // 获取某日新建讨论组数量+1
    int getDiscussionTimes(const QDate &);

    // 增加某日用户注册数量
    bool increaseUserTimes(const QDate &);
    // 增加某日新建群数量
    bool increaseFlockTimes(const QDate &);
    // 增加某日新建讨论组数量
    bool increaseDiscussionTimes(const QDate &);

    /***********邮件模块*********************/
    // 获取收件箱
    int searchInMails(const QString & id, QVector<TMailInformation> & mailsVec);
    // 获取发件箱
    int searchOutMails(const QString & id, QVector<TMailInformation> & mailsVec);
    // 删除邮件
    bool deleteMail(const QString & receiverID, const int mailID);
    // 删除收件箱邮件
    int deleteInMail(const QString & receiverID, const int mailID);
    // 删除收件箱邮件
    int deleteOutMail(const QString & senderID, const int mailID);
    // 插入新邮件
    int insertMail(TMailInformation &);
    // 更新邮件，设置为已读
    bool setMailRead(const QString & receiverID, const int mailID);

    /*************好友分组********************/
//    // 删除分组
//    int deleteGroup(const QString & id, const QString & title);
//    // 创建分组
//    int createGroup(const QString & id, const QString & title);

    // 移动好友至其他分组
    int moveFriendToGroup(const QString & myID, const QString & friendID,
                          const QString & title, const QString & newTitle);
    // 重命名分组
    int renameGroup(const QString & id, const QString & title,
                    const QString & newTitle);

    /************消息管理*********************/
    // 存储历史记录
    bool addHistoryMessage(const TTalkMessage &);
    // 存储离线记录
    bool addOfflineMessage(const TTalkMessage &);
    // 获取用户的某日与某位好友的聊天记录
    int searchHistoryMessageOneFriendOneDate(const QString & myID, const QString &friendID, const QDate & date, QVector<TTalkMessage> & mesVec);

private:
    //检查SQL语句的正确性
    void errorSQLOrder(QSqlQuery query, QString mark);
    //创建数据库表格
    void createTable();

private:
    QSqlDatabase *mpSqlDatabase;
};

#endif // TDATABASECTRL_H

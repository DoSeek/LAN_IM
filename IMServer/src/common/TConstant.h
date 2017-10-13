/*********************************************
File Name： IMConstant.h
Author： jet.F.R
Date： 2014.3.10
Description： 定义了一个常用的结构体与常量
Changes：
********************************************/

#ifndef IMCONSTANT_H
#define IMCONSTANT_H

#include "stable.h"

class TClientSocketCtrl;

static const QString GROUP_NAME_DEFAULT = QString(QObject::tr("New Friends"));

struct ServerNode
{
    QString mstrIPAddress;			//服务器地址
    int mnPort;                     //服务器窗口
    ServerNode()
    {	mnPort = 0;	}
};

// 登录状态
enum ELoginStatus
{
    ELS_ONLINE = 0,    // 在线
    ELS_INVISIBLE = 1, // 隐身
    ELS_BUSY = 2,      // 忙碌
    ELS_LEAVE = 3,     // 离开
    ELS_NOTDISTURB = 4,// 请勿打扰
    ELS_OFFLINE = 5
};

//群成员身份
enum EFlockStatus
{
    EFS_FLOCK_HOST = 0,    // 群主
    EFS_FLOCK_ADMAIN = 1,  // 群管理员
    EFS_FLOCK_MEMBER = 2   // 群成员
};

//讨论组成员身份
enum EDiscussionStatus
{
    EDS_DISCUSSION_HOST = 0,    // 组长
    EDS_DISCUSSION_MEMBER = 1   // 组员
};

enum EMailStatus
{
    EMS_MAIL_UNREAD = 0,
    EMS_MAIL_READ = 1
};


// 信息(命令)类型
enum EMessageType
{
    /***********************client to server************************/
    EMT_CHECK_CONNECTION,           // 检查是否能与服务器连接
    EMT_LOGIN,                      // 登录
    EMT_REGISTER,                   // 注册
    EMT_GET_QUESTION_ANSWER,    // 获取密保问题以及答案
    EMT_GET_QUESTION_ANSWER_SUCCESS,
    EMT_GET_QUESTION_ANSWER_FAIL,

    EMT_CHECK_MESSAGE,              // 检查有无给自己的信息（登录时检查）

    EMT_GET_ALL_FRIENDS,            // 获取所有好友信息
    EMT_GET_ALL_FLOCKS,             // 获取所有群信息
    EMT_GET_ALL_DISCUSSIONS,        // 获取所有讨论组信息
    EMT_GET_FLOCK_MEMBERS,          // 获取群成员
    EMT_GET_DISCUSSIONS_MEMBERS,    // 获取讨论组成员

    EMT_TALK,				// 发送对话
    EMT_ADD_FRIEND,                 // 添加好友



    EMT_ADD_FLOCK,                   // 用户加群
    EMT_ADD_DISCUSSION,

    EMT_FLOCK_ADD_MEMBER,            // 群加用户

    EMT_GET_FRIEND_INFORMATION,		// 获取好友信息
    EMT_DELETE_FRIEND,				// 删除好友
    EMT_GET_MYSELF_INFORMATION,		// 获取自己信息
    EMT_CHANGE_INFORMATION,			// 改变自己的信息
    EMT_CHANGE_REMARK,				// 改变（好友）备注
    EMT_CHANGE_PASSWORD,			    // 修改密码
    EMT_CHANGE_STATUE,				// 改变自身状态
    // 还有分组好友管理
    EMT_QUIT,						// 退出

    /***********************server to client************************/

    EMT_LOGIN_SUCCESS ,				// 登录成功
    EMT_LOGIN_FAIL,			        // 登录失败
    EMT_HAVE_LOGINED,				// 已经登录
    EMT_REGISTER_SUCCESS,	        // 注册成功
    EMT_REGISTER_FAIL,			    // 注册失败

    EMT_REMOVE_BOX,               // 删除分组
    EMT_REMOVE_BOX_SUCCESS,
    EMT_REMOVE_BOX_FAIL,
    EMT_RENAME_BOX,               // 重命名分组
    EMT_RENAME_BOX_SUCCESS,
    EMT_RENAME_BOX_FAIL,
    EMT_CREATE_BOX,               // 创建分组
    EMT_CREATE_BOX_SUCEESS,
    EMT_CREATE_BOX_FAIL,
    EMT_MOVE_FRIEND_BOX,          // 移动好友至其他分组  36
    EMT_MOVE_FRIEND_BOX_SUCCESS,
    EMT_MOVE_FRIEND_BOX_FAIL,


    EMT_GET_ALL_FRIENDS_SUCCESS,    // 获取所有好友信息成功
    EMT_GET_ALL_FRIENDS_FAIL,       // 获取所有好友信息失败
    EMT_NO_FRIEND,                  // 没有好友
    EMT_GET_ALL_FLOCKS_SUCCESS,     // 获取所有群信息成功
    EMT_GET_ALL_FLOCKS_FAIL,        // 获取所有群信息失败
    EMT_NO_FLOCK,
    EMT_GET_ALL_DISCUSSIONS_SUCCESS,// 获取所有讨论组信息成功
    EMT_GET_ALL_DISCUSSIONS_FAIL,   // 获取所有讨论组信息失败
    EMT_NO_DISCUSSION,

    EMT_GET_FLOCK_MEMBERS_FAIL,           // 获取群成员失败
    EMT_GET_FLOCK_MEMBERS_SUCCESS,        // 获取群成员成功
    EMT_NO_FLOCK_MEMBER,                // 没有群成员
    EMT_GET_DISCUSSION_MEMBERS_FAIL,     // 获取讨论组成员失败
    EMT_GET_DISCUSSION_MEMBERS_SUCCESS,  // 获取讨论组成员成功
    EMT_NO_DISCUSSION_MEMBER,          // 没有讨论组成员

    EMT_NO_MESSAGE,				    // 没有信息
    EMT_HAVE_MESSAGE,				// 有信息
    EMT_HAVE_TALK_MESSAGE,          // 有聊天信息
    //？？？？？？？？？？？？？？？？
    EMT_FRIEND_REQUESTED,			// 好友请求
    EMT_FRIEDN_NO_ACCOUNT,			// 没有这个用户名
    EMT_ALREAD_FRIENDS,			    // 回应好友请求

    EMT_GET_FRIEND_FAIL,
    EMT_GET_FRIEND_SUCCESS,			// 获得好友资料成功
    EMT_DELETE_FRIEND_FAIL,
    EMT_DELETE_FRIEND_SUCCESS,		// 删除好友成功
    EMT_GET_MYSELF_FAIL,
    EMT_GET_MYSELF_SUCCESS,		    // 获得自己详细资料成功
    EMT_CHANGE_INFORMATION_FAIL,
    EMT_CHANGE_INFORMATION_SUCCESS,	// 改变自身资料成功

    EMT_CHANGE_REMARK_FAIL,             // 修改备注失败
    EMT_CHANGE_REMARK_SUCCESS,		// 修改备注成功

    EMT_OLD_PWD_IS_WRONG,			// 旧密码是错的
    EMT_CHANGE_PWD_FAIL,              // 修改密码失败
    EMT_CHANGE_PWD_SUCCESS,			// 修改密码成功

    /***********************client to client************************/

    EMT_REQUEST_FRIEND,	     // 请求添加好友
    EMT_AGREE_FRIEND,	     // 同意好友请求
    EMT_REFUSE_FRIEND,		 // 不同意好友请求
    EMT_ADD_FRIEND_FAIL,      // 添加好友失败
    EMT_ADD_FRIEND_SUCCESS,   // 添加好友成功
    EMT_END_ADD_FRIEND,	     // 好友添加结束
    EMT_BE_DELETED,		     // 自己被好友删除了

    EMT_MESSAGE,				 // 普通信息
    EMT_TALK_MESSAGE,         // 聊天信息

    EMT_REQUEST_FLOCK,   // 用户请求加入群
    EMT_FLOCK_AGREE,    // 群主同意用户加入
    EMT_FLOCK_REFUSE,   // 群主拒绝用户加入
    EMT_ADD_FLOCK_FAIL,             // 加入群失败
    EMT_ADD_FLOCK_SUCCESS,          // 加入群成功
    EMT_NEW_FLOCK_MEMBER,           // 新成员入群
    EMT_LEAVE_FLOCK_MEMBER,


    EMT_FLOCK_REQUEST,  // 群主请求用户加入群
    EMT_AGREE_FLOCK,    // 用户同意加入群
    EMT_REFUSE_FLOCK,   // 用户拒绝加入群


    EMT_CREATE_FLOCK,               // 创建群
    EMT_CREATE_FLOCK_FAIL,
    EMT_CREATE_FLOCK_SUCCESS,

    EMT_CREATE_DISCUSSION,               // 创建讨论组
    EMT_CREATE_DISCUSSION_FAIL,
    EMT_CREATE_DISCUSSION_SUCCESS,

    EMT_DROP_FLOCK,                 // 解散群
    EMT_DROP_FLOCK_FAIL,            // 解散群失败
    EMT_DROP_FLOCK_SUCCESS,         // 解散群成功

    EMT_DROP_DISCUSSION,                 // 解散讨论组
    EMT_DROP_DISCUSSION_FAIL,            // 解散讨论组失败
    EMT_DROP_DISCUSSION_SUCCESS,         // 解散讨论组成功

    EMT_LEAVE_FLOCK,              // 退出群
    EMT_LEAVE_FLOCK_SUCCESS,      // 退出群成功
    EMT_LEAVE_FLCCK_FAIL,         // 退出群失败
    EMT_LEAVE_DISCUSSION,           // 退出讨论组
    EMT_LEAVE_DISCUSSION_FAIL,       // 退出讨论组失败
    EMT_LEAVE_DISCUSSION_SUCCESS,   // 退出讨论组成功


    EMT_ADD_DISCUSSION_FAIL,
    EMT_ADD_DISCUSSION_SUCCESS,
    EMT_NEW_DISCUSSION_MEMBER,      // 新成员入讨论组
    EMT_LEAVE_DISCUSSION_MEMBER,

    EMT_CHANGE_FLOCK,               // 修改群信息
    EMT_CHANGE_FLOCK_SUCCESS,
    EMT_CHANGE_FLOCK_FAIL,

    EMT_CHANGE_FLOCK_REMARK,             // 修改群成员备注
    EMT_CHANGE_FLOCK_REMARK_FAIL,
    EMT_CHANGE_FLOCK_REMARK_SUCCESS,

    EMT_CHANGE_DISCUSSION,               // 修改讨论组信息
    EMT_CHANGE_DISCUSSION_SUCCESS,
    EMT_CHANGE_DISCUSSION_FAIL,

    EMT_TALK_FLOCK,               // 群对话
    EMT_TALK_DISCUSSION,          // 讨论组对话

    EMT_REQUEST_VIDEO,
    EMT_REFUSE_VIDEO,
    EMT_END_VIDEO,
    EMT_REQUEST_VOICE,
    EMT_AGREE_VOICE,
    EMT_REFUSE_VOICE,
    EMT_END_VOICE,
    EMT_REQUEST_FILE,
    EMT_AGREE_FILE,
    EMT_REFUSE_FILE,
    EMT_END_FILE,

    /****** 邮件模块 ******/
    EMT_GET_IN_MAILS,
    EMT_GET_IN_MAILS_SUCCESS,
    EMT_GET_IN_MAILS_FAIL,

    EMT_GET_OUT_MAILS,
    EMT_GET_OUT_MAILS_SUCCESS,
    EMT_GET_OUT_MAILS_FAIL,

    EMT_SEND_MAIL,
    EMT_SEND_MAIL_SUCCESS,
    EMT_SEND_MAIL_FAIL,

    EMT_RECEIVE_MAIL,
    EMT_RECEIVE_MAIL_SUCCESS,
    EMT_RECEIVE_MAIL_FAIL,

    EMT_DELETE_IN_MAIL,
    EMT_DELETE_IN_MAIL_SUCCESS,
    EMT_DELETE_IN_MAIL_FAIL,
    EMT_DELETE_OUT_MAIL,
    EMT_DELETE_OUT_MAIL_SUCCESS,
    EMT_DELETE_OUT_MAIL_FAIL,

    EMT_SET_MAIL_READ,

    EMT_GET_HISTORY_MESSAGE,
    EMT_GET_HISTORY_MESSAGE_SUCCESS,
    EMT_GET_HISTORY_MESSAGE_FAIL
};


// 存储聊天的字体和颜色
struct TSaveFile
{
    QFont mFont;
    QColor mColor;
    friend QDataStream & operator<< (QDataStream &out, const TSaveFile &sf)
    {
       out << sf.mFont << sf.mColor;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, TSaveFile &sf)
    {
       in >> sf.mFont >> sf.mColor;
       return in;
    }
};

struct TMailInformation
{
    int mnMailID;
    QString mstrSenderAddress;
    QString mstrReceiverAddress;
    QString mstrTheme;
    QString mstrContent;
    QDateTime mdateTime;
    int mnStatus;

    TMailInformation()
    {
        mnStatus = EMS_MAIL_READ;
    }

    // 重载输入输出操作符
    friend QDataStream &operator<<(QDataStream &out, const TMailInformation &mail)
    {
        out << mail.mnMailID << mail.mstrSenderAddress << mail.mstrReceiverAddress
            << mail.mstrTheme << mail.mstrContent << mail.mdateTime << mail.mnStatus;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, TMailInformation &mail)
    {
        in >> mail.mnMailID >> mail.mstrSenderAddress >> mail.mstrReceiverAddress
           >> mail.mstrTheme >> mail.mstrContent >> mail.mdateTime >> mail.mnStatus;
        return in;
    }
};

// 用户详细信息
struct TUserInformation
{
    //QString mstrPicturePath;
    int mnHeadPortrait;
    QString mstrUserID;
    QString mstrNickName;
    QString mstrPassword;
    QString mstrSex;
    QDate mstrBirthday;
    QString mstrQuestion;
    QString mstrAnswer;
    QString mstrName;
    QString mstrPhone;
    QString mstrAddress;
    QDateTime mRegDateTime;
    //QString mstrMark;
    int mnStatus;

    TUserInformation()
    {
       mnStatus = ELS_OFFLINE;
       mnHeadPortrait = 1;
    }
    // 重载输入输出操作符
    friend QDataStream &operator<<(QDataStream &out, const TUserInformation &user)
    {
       out << user.mnHeadPortrait << user.mstrUserID << user.mstrNickName
           << user.mstrPassword << user.mstrSex << user.mstrBirthday
           << user.mstrQuestion << user.mstrAnswer << user.mstrName
           << user.mstrPhone << user.mstrAddress << user.mRegDateTime
           << user.mnStatus;
       return out;
    }

    friend QDataStream &operator>>(QDataStream &in, TUserInformation &user)
    {
       in >> user.mnHeadPortrait >> user.mstrUserID >> user.mstrNickName
          >> user.mstrPassword >> user.mstrSex >> user.mstrBirthday
          >> user.mstrQuestion >> user.mstrAnswer >> user.mstrName
          >>  user.mstrPhone >> user.mstrAddress >> user.mRegDateTime
          >> user.mnStatus;
       return in;
    }
};

// 好友信息
struct TFriendInformation
{
//    QString m_picture;
    int mnHeadPortrait;
    QString mstrUserID;
    QString mstrNickName;
    QString mstrGroupName;
    QString mstrRemarkName;
    int mnStatus;

    TFriendInformation()
    {
       mnStatus = ELS_OFFLINE;
       mnHeadPortrait = 1;
       mstrGroupName = GROUP_NAME_DEFAULT;
    }

    //重载输入输出操作符
    friend QDataStream & operator<< (QDataStream &out,
                                    const TFriendInformation &fri)
    {
       out << fri.mnHeadPortrait << fri.mstrUserID << fri.mstrNickName
           << fri.mstrGroupName << fri.mstrRemarkName << fri.mnStatus;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, TFriendInformation &fri)
    {
       in >> fri.mnHeadPortrait >> fri.mstrUserID >> fri.mstrNickName
          >> fri.mstrGroupName >> fri.mstrRemarkName >> fri.mnStatus;
       return in;
    }
};

// 储存在各种小窗口中填写的信息
struct TTempStrings
{
    QString mstrOne;
    QString mstrTwo;
    QString mstrThree;
    friend QDataStream & operator<< (QDataStream &out, const TTempStrings &str)
    {
       out << str.mstrOne << str.mstrTwo << str.mstrThree;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, TTempStrings &str)
    {
       in >> str.mstrOne >> str.mstrTwo >> str.mstrThree;
       return in;
    }
};

// 聊天框信息
struct TChatInformation
{
    QString mstrMyID;
//     QString mstrMyNickname;
   int mnHeadPortrait;
   QString mstrFriendID;          // 好友id
   QString mstrFriendNickname;    // 好友昵称
   QString mstrFriendRemark;      // 好友备注
   QString mstrFriendMark;        // 好友签名
   QString mstrFriendIP;          // 好友ip地址
   int mnFriendStatus;

    TChatInformation()
    {
       mnFriendStatus = ELS_OFFLINE;
       mnHeadPortrait= 1;
       mstrFriendMark = QString(QObject::tr("好友签名"));
    }

//    //重载输入输出操作符
//    friend QDataStream & operator<< (QDataStream &out, const TChatInformation &chat)
//    {
//       out << chat.m_headPortrait << chat.m_friendID << chat.m_friendNickname
//           << chat.m_friendRemark << chat.m_friendMark << chat.m_friendIP;
//       return out;
//    }
//    friend QDataStream & operator>> (QDataStream &in, TChatInformation &chat)
//    {
//       in >> chat.m_headPortrait >> chat.m_friendID >> chat.m_friendNickname
//          >> chat.m_friendRemark >> chat.m_friendMark >> chat.m_friendIP;
//       return in;
//    }
};

// 对话信息
struct TTalkMessage
{
    int mnType;               // 消息类型
    QString mstrSenderID;       // 发送者ID
    QString mstrSenderIP;       // 发送者IP
    QString mstrReceiverID;     // 接收者ID
    QString mstrReceiverIP;     // 接收者IP
    QString mstrText;           // 消息内容
    QDateTime mdateTime;

    TTalkMessage() {	mnType = 0; }

    friend QDataStream & operator<< (QDataStream &out, const TTalkMessage &mes)
    {
       out << mes.mnType << mes.mstrSenderID << mes.mstrSenderIP << mes.mstrReceiverID
           << mes.mstrReceiverIP << mes.mstrText << mes.mdateTime;
       return out;
    }

    friend QDataStream & operator>> (QDataStream &in, TTalkMessage &mes)
    {
       in >> mes.mnType >> mes.mstrSenderID >> mes.mstrSenderIP >> mes.mstrReceiverID
          >> mes.mstrReceiverIP >> mes.mstrText >>  mes.mdateTime;
       return in;
    }
};

// 用户登录信息
struct TLoginInformation
{
    QString mstrUserID;
    QString mstrPassword;
    int mnStatus;

    TLoginInformation()
    {	mnStatus = ELS_OFFLINE;	}
    friend QDataStream & operator<< (QDataStream &out, const TLoginInformation &li)
    {
       out << li.mstrUserID << li.mstrPassword << li.mnStatus;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, TLoginInformation &li)
    {
       in >> li.mstrUserID >> li.mstrPassword >> li.mnStatus;
       return in;
    }
};

// 群成员信息
struct TFlockMember
{
    QString mstrFlockID;     // 群ID
    QString mstrUserID;      // 用户ID
    int mnHeadPortrait;    // 用户头像编号
    QString mstrNickname;    // 用户昵称
    int mnLoginStatus;     // 用户登录状态
    QString mstrRemark;      // 用户群名片
    int mmFlockStatus;     // 用户身份
    QDateTime mdateTimeJoin;  // 加入时间

    friend QDataStream & operator<< (QDataStream &out, const TFlockMember &flockMember)
    {
       out << flockMember.mstrFlockID << flockMember.mstrUserID
           << flockMember.mnHeadPortrait << flockMember.mstrNickname
           << flockMember.mnLoginStatus << flockMember.mstrRemark
           << flockMember.mmFlockStatus << flockMember.mdateTimeJoin;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, TFlockMember &flockMember)
    {
       in >> flockMember.mstrFlockID >> flockMember.mstrUserID
          >> flockMember.mnHeadPortrait >> flockMember.mstrNickname
          >> flockMember.mnLoginStatus >> flockMember.mstrRemark
          >> flockMember.mmFlockStatus >> flockMember.mdateTimeJoin;
       return in;
    }
};

// 讨论组成员信息
struct TDiscussionMember
{
    QString mstrDiscussionID;// 讨论组ID
    QString mstrUserID;      // 用户ID
    int mnHeadPortrait;    // 用户头像编号
    QString mstrNickName;    // 用户昵称
    int mnLoginStatus;     // 用户登录状态
    int mnDiscussionStatus;// 用户身份
    QDateTime mdateTimejoin;  // 加入时间

    friend QDataStream & operator<< (QDataStream &out, const TDiscussionMember &disMember)
    {
       out << disMember.mstrDiscussionID << disMember.mstrUserID
           << disMember.mnHeadPortrait << disMember.mstrNickName
           << disMember.mnLoginStatus << disMember.mnDiscussionStatus
           << disMember.mdateTimejoin;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, TDiscussionMember &disMember)
    {
       in >> disMember.mstrDiscussionID >> disMember.mstrUserID
          >> disMember.mnHeadPortrait >> disMember.mstrNickName
          >> disMember.mnLoginStatus  >> disMember.mnDiscussionStatus
          >> disMember.mdateTimejoin;
       return in;
    }
};

// 群信息
struct TFlockInformation
{
    QString mstrFlockID;
    QString mstrCreatorID;
    QDate mstrCreateDate;
    QString mstrFlockName;
    QString mstrTheme;

    friend QDataStream & operator<< (QDataStream &out, const TFlockInformation &flock)
    {
       out << flock.mstrFlockID << flock.mstrCreatorID << flock.mstrCreateDate
           << flock.mstrFlockName << flock.mstrTheme;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, TFlockInformation &flock)
    {
       in >> flock.mstrFlockID >> flock.mstrCreatorID >> flock.mstrCreateDate
          >> flock.mstrFlockName >> flock.mstrTheme;
       return in;
   }
};

// 讨论组信息
struct TDiscussionInformation
{
    QString mstrDiscussionID;
    QString mstrCreatorID;
    QDate mstrCreateDate;
    QString mstrDiscussionName;
    QString mstrTheme;

    friend QDataStream & operator<< (QDataStream &out, const TDiscussionInformation &dis)
    {
       out << dis.mstrDiscussionID << dis.mstrCreatorID << dis.mstrCreateDate
           << dis.mstrDiscussionName << dis.mstrTheme;
       return out;
    }
    friend QDataStream & operator>> (QDataStream &in, TDiscussionInformation &dis)
    {
       in >> dis.mstrDiscussionID >> dis.mstrCreatorID >> dis.mstrCreateDate
          >> dis.mstrDiscussionName >> dis.mstrTheme;
       return in;
    }
};

// 存储临时信息的结构体，用于信号/糟
struct TSaveTmpInformation
{
    int mnRequestKind;
    int mnReplyKind;
    TClientSocketCtrl *mpClientSocket;

    QString mstrMyID;
    QString mstrPeerID;
    QDate mDate;
    QString mstrGroupName;
    QString mstrGroupNameF;
    TUserInformation mUserInf;
    TLoginInformation mLoginInf;
    TFriendInformation mFriendInf;
    TTalkMessage mMessage;
    int mnStatus;
    TTempStrings mTempStr;
    QVector<TTalkMessage> mMessageVec;
    QVector<TFriendInformation> mFriendsVec;
    QVector<QString> mIDVec;

    QString mstrFlockID;
    TFlockMember mFlockMember;
    TFlockInformation mFlockInf;
    QVector<TFlockInformation> mFlocksVec;
    QVector<TFlockMember> mFlockMembersVec;
    QString mstrDiscussionID;
    TDiscussionMember mDiscussionMember;
    TDiscussionInformation mDiscussionInf;
    QVector<TDiscussionInformation> mDiscussionsVec;
    QVector<TDiscussionMember> mDiscussionMembersVec;

    int mnMailID;
    TMailInformation mMail;
    QVector<TMailInformation>  mInMailsVec;
    QVector<TMailInformation>  mOutMailsVec;

    QString mstrTitle;
    QString mstrNewTitle;

    QString mstrSocketID;

    QVector<TTalkMessage> mHistoryMessageVec;
};


//头像尺寸.
const int HEAD_BIG_SIZE = 40;
const int HEAD_SMALL_SIZE = 35;
const int HEAD_MINI_SIZE = 20;

//聊天室 工具栏尺寸
const int CHAT_WIDGET_TOOL_SIZE = 20;
const int CHAT_WIDGET_TOOL_SMALL_SIZE = 15;

const int FONT_SIZE = 2;

static const QString MAIL_ADDRESS_FORMAT = "@im.com";

static const QString LOCAL_HISTORY_MESSAGE_PATH = "resource/history/local/";
static const QString NETWORK_HISTORY_MESSAGE_PATH = "resource/history/network/";

#endif // IMCONSTANT_H

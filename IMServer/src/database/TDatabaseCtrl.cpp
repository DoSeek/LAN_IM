/*********************************************
File Name： TDatabaseCtrl.cpp
Author： jet.F.R
Date： 2014.3.12
Description： 操作数据库
Changes：
********************************************/

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <qdebug.h>
#include <QSqlError>
#include <QTextDocument>

#include "TDatabaseCtrl.h"

static const QString UPDATE_STATUS_IN_USERINFORMATION_SQL =
        "update TUserInformation set status = 5";
static const QString UPDATE_PASSWORD_IN_USERINFORMATION_SQL =
        "update TUserInformation set password = ? "
        "where userID = ? and password = ?";
static const QString SEARCH_QUESTION_ANSWER_IN_USERINFORMATION_SQL =
        "select password, question, answer from TUserInformation where userID = ?;";
static const QString SEARCH_USER_IN_USERINFORMATION_SQL =
        "select * from TUserInformation where (userID = ? and password = ?)";
static const QString SEARCH_USER_IN_USERINFORMATION_ID_SQL =
        "select * from TUserInformation where userID = ?";
static const QString UPDATE_USERINFORMATION_SQL =
        "update TUserInformation set "
        "head = ?, nickname = ?, sex = ?, birthday = ?, "
        "name = ?, phone = ?, address = ?  where userID = ?;";

//update TUserInformation set head =33, nickname = "33nick", sex = "女",
//birthday= 1991,name= 333,phone= 334,address= "33a"  where userID=201404081;

static const QString  UPDATE_STATUS_IN_USERINFORMATION_BY_ID_SQL=
        "update TUserInformation set status = ? where userID = ?;";

static const QString  UPDATE_REMARKNAME_IN_USERFRIENDS_SQL=
        "update UserFriends set remarkName = ? where userID = ? and friendID = ?;";

static const QString ADD_USER_IN_USERINFORMATION_SQL =
        "insert into TUserInformation values(?,?,?,?,?,?,?,?,?,?,?,?,?)";

static const QString COUNT_REGISTERED_USERS_IN_USERINFORMATION_SQL =
        "select count(*) from TUserInformation where regDate >= ? and regDate < ?";

static const QString COUNT_FLOCKS_IN_FLOCKINFORMATION_SQL =
        "select count(*) from TFlockInformation where createDate >= ? and createDate < ?";

static const QString SEARCH_FRIENDS_IN_USERFRIENDS_USERINFORMATION_SQL =
        "select UserFriends.groupName, UserFriends.friendID, "
        "TUserInformation.nickName, UserFriends.remarkName,"
        "TUserInformation.status, TUserInformation.head "
        "from UserFriends,TUserInformation "
        "where UserFriends.userID = ? "
        "and TUserInformation.userID = UserFriends.friendID;";

static const QString SEARCH_ID_IN_USERFRIENDS_SQL =
        "select friendID from UserFriends where userID = ?";
static const QString SEARCH_GROUPNAME_IN_USERFRIENDS_SQL =
        "select groupName from UserFriends where userID = ? and friendID = ?";
static const QString DELETE_FRIEND_IN_USERFRIENDS_SQL =
        "delete from UserFriends where "
        "(userID = ? and friendID = ?) or "
        "(userID = ? and friendID = ?)";
static const QString ADD_FRIEND_IN_USERFRIENDS_SQL =
        "insert into UserFriends(userID, friendID, groupName) values (?, ?, ?);";

static const QString SEARCH_FLOCKS_IN_FLOCKINFORMAIONT_USERFLOCKS_SQL =
        "select * from TFlockInformation "
        "where TFlockInformation.flockID IN ("
        "select UserFlocks.flockID from UserFlocks "
        "where UserFlocks.userID = ?);";
static const QString SEARCH_FLOCK_IN_FLOCKINFORMAIONT_SQL =
        "select * from TFlockInformation where flockID = ?";

static const QString SEARCH_FLOCKMEMBERS_IN_USERINFORMATION_USERFLOCKS_SQL =
        "select UserFlocks.flockID, UserFlocks.userID, TUserInformation.head, "
        "TUserInformation.nickname, TUserInformation.status, UserFlocks.remark, "
        "UserFlocks.status, UserFlocks.joinDate "
        "from TUserInformation, UserFlocks "
        "where UserFlocks.flockID = ? and "
        "TUserInformation.userID = UserFlocks.userID;";

static const QString SEARCH_FLOCKMEMBER_IN_USERINFORMATION_USERFLOCKS_SQL =
        "select UserFlocks.flockID, UserFlocks.userID, TUserInformation.head, "
        "TUserInformation.nickname, TUserInformation.status, UserFlocks.remark, "
        "UserFlocks.status, UserFlocks.joinDate "
        "from TUserInformation, UserFlocks "
        "where UserFlocks.flockID = ? and UserFlocks.userID= ? and "
        "TUserInformation.userID = UserFlocks.userID;";

static const QString SEARCH_DISCUSSIONS_IN_DISCUSSIONINFORMAIONT_USERDISCUSSIONS_SQL =
        "select * from TDiscussionInformation "
        "where TDiscussionInformation.discussionID IN ("
        "select UserDiscussions.discussionID from UserDiscussions "
        "where UserDiscussions.userID = ?);";

static const QString SEARCH_DISCUSSION_IN_DISCUSSIONINFORMAIONT_SQL =
        "select * from TDiscussionInformation where discussionID = ?";

static const QString SEARCH_DISCUSSIONMEMBERS_IN_USERINFORMATION_USERDISCUSSIONS_SQL =
        "select UserDiscussions.discussionID, UserDiscussions.userID, "
        "TUserInformation.head, TUserInformation.nickname, "
        "TUserInformation.status,UserDiscussions.status, "
        "UserDiscussions.joinDate "
        "from TUserInformation, UserDiscussions "
        "where UserDiscussions.discussionID = ? and "
        "TUserInformation.userID = UserDiscussions.userID;";

static const QString SEARCH_DISCUSSIONMEMBER_IN_USERINFORMATION_USERDISCUSSIONS_SQL =
        "select UserDiscussions.discussionID, UserDiscussions.userID, "
        "TUserInformation.head, TUserInformation.nickname, "
        "TUserInformation.status,UserDiscussions.status, "
        "UserDiscussions.joinDate "
        "from TUserInformation, UserDiscussions "
        "where UserDiscussions.discussionID = ? and UserDiscussions.userID= ? and "
        "TUserInformation.userID = UserDiscussions.userID;";

////////////////////////////////////////////////////////////////////////////////////

static const QString SEARCH_FLOCK_CREATORID_IN_FLOCKINFORMATION_SQL =
        "select creatorID from TFlockInformation where flockID = ?";

static const QString ADD_MEMBER_IN_USERFLOCKS_SQL =
        "insert into UserFlocks(flockID, userID, status, joinDate) values (?, ?, ?, ?);";

static const QString DELETE_FLOCK_IN_FLOCKINFORMATION_SQL =
        "delete from TFlockInformation where flockID = ? and creatorID = ?";

static const QString DELETE_FLOCKMEMEBER_IN_USERFLOCKS_SQL =
        "delete from UserFlocks where flockID= ? and userID = ?";

static const QString UPDATE_FLOCKINFORMATION_SQL =
        "update TFlockInformation set flockName = ?, theme = ? where flockID = ?;";

static const QString UPDATE_FLOCKMEMBER_REMAKR_IN_USERFLOCKS_SQL =
        "update UserFlocks set remark = ? where flockID = ? and userID = ?";

static const QString ADD_FLOCK_IN_FLOCKINFORMATOIN_SQL =
        "insert into TFlockInformation values( ?, ?, ?, ?, ?);";
////////////////////////////////////////////////////////////////////////////////////

static const QString SEARCH_DISCUSSION_CREATORID_IN_DISCUSSIONINFORMATION_SQL =
        "select creatorID from TDiscussionInformation where discussionID = ?";

static const QString ADD_MEMBER_IN_USERDISCUSSIONS_SQL =
        "insert into UserDiscussions(discussionID, userID, status, joinDate) values (?, ?, ?, ?);";

static const QString DELETE_DISCUSSION_IN_DISCUSSIONINFORMATION_SQL =
        "delete from TDiscussionInformation where discussionID = ? and creatorID = ?";

static const QString DELETE_DISCUSSIONMEMEBER_IN_USERDISCUSSIONS_SQL =
        "delete from UserDiscussions where discussionID= ? and userID = ?";

static const QString UPDATE_DISCUSSIONINFORMATION_SQL =
        "update TDiscussionInformation set discussionName = ?, theme = ? where discussionID = ?;";

static const QString UPDATE_DISCUSSIONMEMBER_REMAKR_IN_USERDISCUSSIONS_SQL =
        "update UserDiscussions set remark = ? where discussionID = ? and userID = ?";

static const QString ADD_DISCUSSION_IN_DISCUSSIONINFORMATOIN_SQL =
        "insert into TDiscussionInformation values( ?, ?, ?, ?, ?);";


////////////////////////////////////////////////////////////////////////////////////

static const QString COUNT_DATARECORD_IN_DATECOUNTER_SQL =
        "select count(*) from DateCounter where date = ?;";
static const QString INSERT_VALUES_IN_DATECOUNTER_SQL =
        "insert into DateCounter values(?, 0, 0, 0);";

static const QString SELECT_USERTIMES_IN_DATECOUNTER_SQL =
        "select userTimes from DateCounter where date = ?;";
static const QString SELECT_FLOCKTIMES_IN_DATECOUNTER_SQL =
        "select flockTimes from DateCounter where date = ?;";
static const QString SELECT_DISCUSSIONTIMES_IN_DATECOUNTER_SQL =
        "select discussionTimes from DateCounter where date = ?;";

static const QString INCREASE_USERTIMES_IN_DATECOUNTER_SQL =
        "update DateCounter set userTimes = userTimes + 1 where date = ?;";
static const QString INCREASE_FLOCKTIMES_IN_DATECOUNTER_SQL =
        "update DateCounter set flockTimes = flockTimes + 1 where date = ?;";
static const QString INCREASE_DISCUSSIONTIMES_IN_DATECOUNTER_SQL =
        "update DateCounter set discussionTimes = discussionTimes + 1 where date = ?;";

static const QString COUNT_FLOCKMEMBER_IN_USERFLOCKS_SQL =
        "select count(*) from UserFlocks where flockID = ? and userID = ?;";

static const QString COUNT_DISCUSSIONMEMBER_IN_USERDISCUSSIONS_SQL =
        "select count(*) from UserDiscussions where discussionID = ? and userID = ?;";

/************************mail*********************/

static const QString SEARCH_IN_MAILS_IN_MAIL =
        "select * from Mail where receiverStatus = 0 and "
        "receiverID = ? and receiverAddress = ? order by time desc;";

static const QString SEARCH_OUT_MAILS_IN_MAIL =
        "select * from Mail where senderStatus = 0 and "
        "senderID = ? and senderAddress = ? order by time desc;";

static const QString INSERT_MAIL_IN_MAIL =
        "insert into Mail (senderID, senderAddress, receiverID, receiverAddress,"
        "theme, content, time) values (?, ?, ?, ?, ?, ?, ?);";
static const QString DELETE_MAIL_IN_MAIL =
        "delete from Mail where receiverID = ? and mailID = ?";
static const QString UPDATE_MAIL_IN_MAIL =
        "update Mail set receiverStatus = 1 where receiverID = ? and mailID = ?";
static const QString UPDATE_MAIL_OUT_MAIL =
        "update Mail set senderStatus = 1 where senderID = ? and mailID = ?";
static const QString SET_MAIL_READ_IN_MAIL =
        "update Mail set status = 1 where receiverID = ? and mailID = ?";

/************************group***************************/
static const QString RENAME_GROUPNAME_IN_USERFREINS =
        "update UserFriends set groupName = ? where userID = ? and groupName = ?;";
static const QString MOVE_FRIEND_IN_USERFREINS =
        "update UserFriends set groupName = ? where userID = ? "
        "and friendID = ?;";   // and groupName = ?;";

/************************message************************/
static const QString INSERT_MESSAGE_IN_HISTORYMESSAGE =
        "insert into HistoryMessage (senderID, receiverID, content, dateTime) "
        "values(?, ?, ?, ?); ";
static const QString INSERT_MESSAGE_IN_OFFLINEMESSAGE =
        "insert into OfflineMessage (kind, senderID, receiverID, content) "
        "values(?, ?, ?, ?);";
static const QString SEARCH_FRIEND_DATE_MESSAGE_IN_HISTORYMESSAGE =
        "select senderID, receiverID, content, dateTime from HistoryMessage "
        "where ( (senderID = ? and receiverID = ?) "
        "or (senderID = ? and receiverID = ?) ) "
        "and dateTime >= ? and dateTime < ?;";

/********************create tables********************/
static const QString CREATE_TABLE_USERINFORMATION_SQL = "";
static const QString CREATE_TABLE_USERFRIENDS_SQL = "";
static const QString CREATE_TABLE_TMPMESSAGE_SQL = "";


//将html格式的qstring转换
QString simplifyRichText( QString richText )
{
    QTextDocument doc;
    doc.setHtml(richText);
    qDebug() << "say:"<< doc.toPlainText();
    return doc.toPlainText();

//    QRegExp rxBody("<body[^>]*>(.*)</body>");
//    if(rxBody.indexIn(richText))
//    {
//        richText = rxBody.cap(1); // throw away anything not in the body, and the body tags as well
//        richText.replace(QRegExp("^\\s+"),""); // throw away any leading whitespace (be it new lines or spaces)
//    }
//    QRegExp rxBody1("<span[^>]*>(.*)</span>");
//    if(richText.indexOf(rxBody1) != -1){
//        if(rxBody1.indexIn(richText))
//        {
//            richText = rxBody1.cap(1); // throw away anything not in the body, and the body tags as well
//            richText.replace(QRegExp("^\\s+"),""); // throw away any leading whitespace (be it new lines or spaces)
//        }
//    }
//    richText.replace(QRegExp("<p style=\"[^\"]*\">"),"<p>"); // throw away paragraph styles


//    if(richText == "<p></p>") // thats what we get for an empty QTextEdit
//        return QString();
//    richText.replace("<p>","");
//    richText.replace("</p>","");

//    return richText;
}

TDatabaseCtrl::TDatabaseCtrl(QObject *parent) :
    QObject(parent)
{
    mpSqlDatabase = new QSqlDatabase;
}

TDatabaseCtrl::~TDatabaseCtrl()
{
    if (mpSqlDatabase != nullptr)
    {
        if (mpSqlDatabase->isOpen())
            mpSqlDatabase->close();
        delete mpSqlDatabase;
        mpSqlDatabase = nullptr;
    }
}

/*************************************************
Function Name： createConnection()
Description: 创建数据库连接
Input： nullptr
Output：bool 表示创建结果
Changes： nullptr
*************************************************/
bool TDatabaseCtrl::createConnection()
{
    if (mpSqlDatabase == nullptr)
        mpSqlDatabase = new QSqlDatabase;
    if (mpSqlDatabase->isOpen())
    {
        return true;
    }
    // 如果已经打开
    if (QSqlDatabase::contains("serverIMDB"))
    {
        *mpSqlDatabase = QSqlDatabase::database("serverIMDB");
    }
    else
    {// 如果没有，就添加
        //*mpSqlDatabase = QSqlDatabase::addDatabase("QMYSQL", "serverIMDB");
        *mpSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
        mpSqlDatabase->setDatabaseName("serverIMDB");
    }

    mpSqlDatabase->setUserName("root");
    mpSqlDatabase->setPassword("");
    mpSqlDatabase->setHostName(QLatin1String("localhost"));
    if (!mpSqlDatabase->open())
    {
        QSqlError sqlError = mpSqlDatabase->lastError();
        QString errorString = sqlError.text();
        return false;
    }
    return true;
}

/*************************************************
Function Name： initDatabase()
Description: 初始化数据库 将所有用户的状态status设置为OFFLINE
*************************************************/
bool TDatabaseCtrl::initDatabase()
{
    if (!createConnection())
    {
        return false;
    }

    QSqlQuery query(*mpSqlDatabase);
    query.exec(UPDATE_STATUS_IN_USERINFORMATION_SQL);
    errorSQLOrder(query, "initiDatabase");
    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return false;
    }
    mpSqlDatabase->close();
    return true;
}

/*************************************************
Function Name： close()
Description: 关闭数据库
Input： nullptr
Output：nullptr
Changes： nullptr
*************************************************/
void TDatabaseCtrl::close()
{
    if (mpSqlDatabase != nullptr && mpSqlDatabase->isOpen())
        mpSqlDatabase->close();
}


/*************************************************
Function Name： isOpen()
Description: 是否连接着
Input： nullptr
Output： bool: 是否连接着
Changes： NULLUserFriends.userID
*************************************************/
bool TDatabaseCtrl::isOpen()
{
    return (mpSqlDatabase->isOpen());

}


/*************************************************
Function Name： updatePassword()
Description: 修改密码
*************************************************/
int TDatabaseCtrl::updatePassword(TTempStrings & tmp)
{
    if (!createConnection())
    {
        return EMT_CHANGE_PWD_FAIL;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(UPDATE_PASSWORD_IN_USERINFORMATION_SQL);

    query.addBindValue(tmp.mstrThree);
    query.addBindValue(tmp.mstrOne);
    query.addBindValue(tmp.mstrTwo);

    query.exec();
    errorSQLOrder(query, "update-pwd");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_CHANGE_PWD_FAIL;
    }
    qDebug() << "size:" << query.numRowsAffected();
    if (query.numRowsAffected() <= 0)
    {
        mpSqlDatabase->close();
        return EMT_CHANGE_PWD_FAIL;
    }

    qDebug() << "size:" << query.numRowsAffected();
//    mpSqlDatabase->close();
    return EMT_CHANGE_PWD_SUCCESS;
}

/*************************************************
Function Name： searchQuestionAndAnswer()
Description: 查询密保问题以及答案
*************************************************/
int TDatabaseCtrl::searchQuestionAndAnswer(const QString &id, TTempStrings &tmp)
{
    if (!createConnection())
    {
        return EMT_GET_QUESTION_ANSWER_FAIL;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_QUESTION_ANSWER_IN_USERINFORMATION_SQL);
    query.addBindValue(id);
//    query.addBindValue(IMEncryption::getInstace().
//                       getXorEncryptDecrypt(logInf.m_password, 10));
    query.exec();
    errorSQLOrder(query, "forget-pwd");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_QUESTION_ANSWER_FAIL;
    }
    if (query.size() <= 0)
    {
        mpSqlDatabase->close();
        return EMT_GET_QUESTION_ANSWER_FAIL;
    }
    if (query.next())
    {
        tmp.mstrOne = query.value(0).toString();
        tmp.mstrTwo = query.value(1).toString();
        tmp.mstrThree = query.value(2).toString();


        return EMT_GET_QUESTION_ANSWER_SUCCESS;
    }
//    mpSqlDatabase->close();
    return EMT_GET_QUESTION_ANSWER_FAIL;
}

/*************************************************
Function Name： searchUserInUserInformation()
Description: 查询用户
*************************************************/
int TDatabaseCtrl::searchUserInUserInformation(const TLoginInformation &logInf,
                                                  TUserInformation &userInf)
{
    if (!createConnection())
    {
        return EMT_LOGIN_FAIL;
    }

    //sql 事务
    QSqlDatabase::database().transaction();
    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_USER_IN_USERINFORMATION_SQL);
    query.addBindValue(logInf.mstrUserID);
    query.addBindValue(logInf.mstrPassword);
    query.exec();
    errorSQLOrder(query, "login-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_LOGIN_FAIL;
    }
    if (query.size() <= 0)
    {
        mpSqlDatabase->close();
        return EMT_LOGIN_FAIL;
    }
    if (query.next())
    {
        userInf.mstrUserID = query.value(0).toString();
        userInf.mnHeadPortrait = query.value(1).toInt();
        userInf.mstrNickName = query.value(2).toString();
        userInf.mstrSex = query.value(4).toString();
        userInf.mstrBirthday = query.value(5).toDate();
        userInf.mstrName = query.value(8).toString();
        userInf.mstrPhone = query.value(9).toString();
        userInf.mstrAddress = query.value(10).toString();
        userInf.mRegDateTime = query.value(11).toDateTime();
        userInf.mnStatus  = logInf.mnStatus;

        qDebug() << "searchUserInUserInformation:" << userInf.mnStatus;
        if (updateStatus(logInf.mnStatus, logInf.mstrUserID))
            return EMT_LOGIN_SUCCESS;
    }
//    mpSqlDatabase->close();
    return EMT_LOGIN_FAIL;
}

/*************************************************
Function Name： searchFriendSimpleInformationByID()
Description: 通过id查询好友简易信息
*************************************************/
int TDatabaseCtrl::searchFriendSimpleInformationByID(const QString &id,
                                                        TFriendInformation &friInf)
{
    // Mark: 作者未实现，并且项目中没有调用此函数
    return 0;
}

/*************************************************
Function Name： searchFriendInformationByID()
Description: 通过id查询好友信息
*************************************************/
int TDatabaseCtrl::searchFriendInformationByID(const QString &id,
                                            TUserInformation &userInf)
{
    if (!createConnection())
    {
        return EMT_GET_FRIEND_FAIL;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_USER_IN_USERINFORMATION_ID_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "friend-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_FRIEND_FAIL;
    }
    if (query.size() <= 0)
    {
        mpSqlDatabase->close();
        return EMT_GET_FRIEND_FAIL;
    }
    if (query.next())
    {
        userInf.mstrUserID = query.value(0).toString();
        userInf.mnHeadPortrait = query.value(1).toInt();
        userInf.mstrNickName = query.value(2).toString();
        userInf.mstrSex = query.value(4).toString();
        userInf.mstrBirthday = query.value(5).toDate();
        userInf.mstrName = query.value(8).toString();
        userInf.mstrPhone = query.value(9).toString();
        userInf.mstrAddress = query.value(10).toString();
        userInf.mRegDateTime = query.value(11).toDateTime();
        userInf.mnStatus = query.value(12).toInt();
    }
//    mpSqlDatabase->close();
    return EMT_GET_FRIEND_SUCCESS;
}

/*************************************************
Function Name： searchMyselfInformationByID()
Description: 通过id查询自己信息
*************************************************/
int TDatabaseCtrl::searchMyselfInformationByID(const QString &id,
                                            TUserInformation &userInf)
{
    if (!createConnection())
    {
        return EMT_GET_MYSELF_FAIL;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_USER_IN_USERINFORMATION_ID_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "myself-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_MYSELF_FAIL;
    }
    if (query.size() <= 0)
    {
        mpSqlDatabase->close();
        return EMT_GET_MYSELF_FAIL;
    }
    if (query.next())
    {
        userInf.mstrUserID = query.value(0).toString();
        userInf.mnHeadPortrait = query.value(1).toInt();
        userInf.mstrNickName = query.value(2).toString();
        userInf.mstrSex = query.value(4).toString();
        userInf.mstrBirthday = query.value(5).toDate();
        userInf.mstrName = query.value(8).toString();
        userInf.mstrPhone = query.value(9).toString();
        userInf.mstrAddress = query.value(10).toString();
        userInf.mRegDateTime = query.value(11).toDateTime();
        userInf.mnStatus = query.value(12).toInt();
    }
//    mpSqlDatabase->close();
    return EMT_GET_MYSELF_SUCCESS;
}

/*************************************************
Function Name： updateMyselfInformation()
Description: 修改用户信息
*************************************************/
int TDatabaseCtrl::updateMyselfInformation(const TUserInformation &user)
{
    if (!createConnection())
    {
        return EMT_CHANGE_INFORMATION_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();
    QSqlQuery query(*mpSqlDatabase);
    query.prepare(UPDATE_USERINFORMATION_SQL);
    query.addBindValue(user.mnHeadPortrait);
    query.addBindValue(user.mstrNickName);
    query.addBindValue(user.mstrSex);
    query.addBindValue(user.mstrBirthday);
    query.addBindValue(user.mstrName);
    query.addBindValue(user.mstrPhone);
    query.addBindValue(user.mstrAddress);
    query.addBindValue(user.mstrUserID);

    query.exec();
    errorSQLOrder(query, "change-update");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_CHANGE_INFORMATION_FAIL;
    }
    return EMT_CHANGE_INFORMATION_SUCCESS;
}

/*************************************************
Function Name： addUserInUserInformation()
Description: 添加用户
*************************************************/
int TDatabaseCtrl::addUserInUserInformation(const TUserInformation &userInf)
{
    if (!createConnection())
    {
        return EMT_REGISTER_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(ADD_USER_IN_USERINFORMATION_SQL);
    query.addBindValue(userInf.mstrUserID);
    query.addBindValue(userInf.mnHeadPortrait);
    query.addBindValue(userInf.mstrNickName);
    query.addBindValue(userInf.mstrPassword);
    query.addBindValue(userInf.mstrSex);
    query.addBindValue(userInf.mstrBirthday.toString("yyyy-MM-dd"));
    query.addBindValue(userInf.mstrQuestion);
    query.addBindValue(userInf.mstrAnswer);
    query.addBindValue(userInf.mstrName);
    query.addBindValue(userInf.mstrPhone);
    query.addBindValue(userInf.mstrAddress);
    query.addBindValue(userInf.mRegDateTime);//.toString("yyyy-MM-dd hh:mm:ss"));
    query.addBindValue(5);
    query.exec();
    errorSQLOrder(query, "register-add");
    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_REGISTER_FAIL;
    }

    if (!increaseUserTimes(QDate::currentDate()))
            return EMT_REGISTER_FAIL;

//    mpSqlDatabase->close();
    return EMT_REGISTER_SUCCESS;
}


/*************************************************
Function Name： countRegisteredInPeriod()
Description: 获取一段时间注册数量
Input： IMUser
Output： int
Changes： nullptr
*************************************************/
int TDatabaseCtrl::countRegisteredInPeriod(const QDateTime &datePre,
                                            const QDateTime &dateLater)
{
    qDebug() << "among of date:" << datePre.toString("yyyy-MM-dd hh:mm:ss")
                << dateLater.toString("yyyy-MM-dd hh:mm:ss");

    if (!createConnection())
    {
        return 0;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(COUNT_REGISTERED_USERS_IN_USERINFORMATION_SQL);
    query.addBindValue(datePre);
    query.addBindValue(dateLater);
    query.exec();
    errorSQLOrder(query, "register-count");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return 0;
    }
    int sum = 0;
    if (query.next())
        sum = query.value(0).toInt() + 1;

    //mpSqlDatabase->close();
    return sum;

}


/*************************************************
Function Name： countFlockInPeriod()
Description: 获取一段时间群添加数量
*************************************************/
int TDatabaseCtrl::countFlockInPeriod(const QDateTime &datePre,
                                 const QDateTime &dateLater)
{
    qDebug() << "among of date:" << datePre.toString("yyyy-MM-dd hh:mm:ss")
                << dateLater.toString("yyyy-MM-dd hh:mm:ss");

    if (!createConnection())
    {
        return 0;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(COUNT_FLOCKS_IN_FLOCKINFORMATION_SQL);
    query.addBindValue(datePre);
    query.addBindValue(dateLater);
    query.exec();
    errorSQLOrder(query, "register-count");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return 0;
    }
    int sum = 0;
    if (query.next())
        sum = query.value(0).toInt() + 1;

    //mpSqlDatabase->close();
    return sum;
}


/*************************************************
Function Name： searchFriendsInformation()
Description: 获取所有好友信息
*************************************************/
int TDatabaseCtrl::searchFriendsInformation(const QString &id,
                                              QVector<TFriendInformation> &friendsVec)
{
    if (!createConnection())
    {
        return EMT_GET_ALL_FRIENDS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_FRIENDS_IN_USERFRIENDS_USERINFORMATION_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-friends-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_ALL_FRIENDS_FAIL;
    }
    friendsVec.clear();
    while (query.next())
    {
        TFriendInformation fri;

        fri.mstrGroupName = query.value(0).toString();
        fri.mstrUserID = query.value(1).toString();
        fri.mstrNickName = query.value(2).toString();
        fri.mstrRemarkName = query.value(3).toString();
        fri.mnStatus = query.value(4).toInt();
        fri.mnHeadPortrait = query.value(5).toInt();
        friendsVec.push_back(fri);
    }
//    mpSqlDatabase->close();
    if (friendsVec.isEmpty())
    {
        return EMT_NO_FRIEND;
    }
    return EMT_GET_ALL_FRIENDS_SUCCESS;
}


/*************************************************
Function Name： searchFriendsID()
Description: 获取所有好友ID
*************************************************/
bool TDatabaseCtrl::searchFriendsID(const QString &id,
                     QVector<QString> &friVec)
{
    friVec.clear();
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_ID_IN_USERFRIENDS_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-friends-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return false;
    }

    while (query.next())
    {
        friVec.push_back(query.value(0).toString());
    }

    return true;
}

// mark: private----------------------------------------------------

/*************************************************
Function Name： errorSQLOrder()
Description: 检查SQL语句的正确性
*************************************************/
void TDatabaseCtrl::errorSQLOrder(QSqlQuery query, QString mark)
{
    //如果SQL语句错误，就弹出警告信息
    if(!query.isActive())
    {
        QString str = query.lastError().text() + "\n" + mark;
        //QMessageBox::warning(nullptr, "ERROR", str);
        qDebug() << "Mysql qurey error: " << str;
        qDebug() << "query text:" << query.lastQuery();
    }
}

/*************************************************
Function Name： createTable()
Description: 创建数据库表格
*************************************************/
void TDatabaseCtrl::createTable()
{
}

/*************************************************
Function Name： updateStatus()
Description: 更新好友状态
*************************************************/
bool TDatabaseCtrl::updateStatus(int status, const QString &id)
{
    qDebug() << "status is what :" << status;
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();
    QSqlQuery query(*mpSqlDatabase);
    query.prepare(UPDATE_STATUS_IN_USERINFORMATION_BY_ID_SQL);
    query.addBindValue(status);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "quit-update");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

//    mpSqlDatabase->close();
    return true;
}

/*************************************************
Function Name： updateRemark()
Description: 更新好友备注
*************************************************/
int TDatabaseCtrl::updateRemark(const TTempStrings & tempStr)
{
    if (!createConnection())
    {
        return EMT_CHANGE_REMARK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();
    QSqlQuery query(*mpSqlDatabase);
    query.prepare(UPDATE_REMARKNAME_IN_USERFRIENDS_SQL);
    query.addBindValue(tempStr.mstrThree);
    query.addBindValue(tempStr.mstrOne);
    query.addBindValue(tempStr.mstrTwo);
    query.exec();
    errorSQLOrder(query, "update-remark");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_CHANGE_REMARK_FAIL;
    }

//    mpSqlDatabase->close();
    return EMT_CHANGE_REMARK_SUCCESS;
}


/*************************************************
Function Name： deleteFriend()
Description: 删除好友
*************************************************/
int TDatabaseCtrl::deleteFriend(const QString &myID, const QString &friendID,
                                   QString &groupName, QString &groupNameF)
{
    if (!createConnection())
    {
        return EMT_DELETE_FRIEND_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();
    QSqlQuery query(*mpSqlDatabase);

    /***********start 查询groupName***************/
    query.prepare(SEARCH_GROUPNAME_IN_USERFRIENDS_SQL);
    query.addBindValue(myID);
    query.addBindValue(friendID);
    query.exec();
    errorSQLOrder(query, "search-friend-groupName");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_DELETE_FRIEND_FAIL;
    }
    if (query.next())
    {
        groupName = query.value(0).toString();
    }
    query.clear();

    /***********start 查询groupNameF***************/
    query.prepare(SEARCH_GROUPNAME_IN_USERFRIENDS_SQL);
    query.addBindValue(friendID);
    query.addBindValue(myID);
    query.exec();
    errorSQLOrder(query, "search-friend-groupNameF");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_DELETE_FRIEND_FAIL;
    }
    if (query.next())
    {
        groupNameF = query.value(0).toString();
    }
    query.clear();

    /***********start 删除好友***************/
    query.prepare(DELETE_FRIEND_IN_USERFRIENDS_SQL);
    query.addBindValue(myID);
    query.addBindValue(friendID);
    query.addBindValue(friendID);
    query.addBindValue(myID);
    query.exec();
    errorSQLOrder(query, "delete-friend");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_DELETE_FRIEND_FAIL;
    }

//    mpSqlDatabase->close();
    return EMT_DELETE_FRIEND_SUCCESS;
}


/*************************************************
Function Name： addFriend()
Description: 添加好友
*************************************************/
bool TDatabaseCtrl::addFriend(const TTalkMessage & mes)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(ADD_FRIEND_IN_USERFRIENDS_SQL);
    query.addBindValue(mes.mstrSenderID);
    query.addBindValue(mes.mstrReceiverID);
    query.addBindValue(GROUP_NAME_DEFAULT);
    query.exec();
    errorSQLOrder(query, "add-friend1");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }
    query.clear();
    query.prepare(ADD_FRIEND_IN_USERFRIENDS_SQL);
    query.addBindValue(mes.mstrReceiverID);
    query.addBindValue(mes.mstrSenderID);
    query.addBindValue(GROUP_NAME_DEFAULT);
    query.exec();
    errorSQLOrder(query, "add-friend2");

    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return true;
}


/*************************************************
Function Name： searchFlocksInformation()
Description: 获取某用户加入的所有群
*************************************************/
int TDatabaseCtrl::searchFlocksInformation(
        const QString & id, QVector<TFlockInformation> & flocksVec)
{
    if (!createConnection())
    {
        return EMT_GET_ALL_FLOCKS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_FLOCKS_IN_FLOCKINFORMAIONT_USERFLOCKS_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-flocks-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_ALL_FLOCKS_FAIL;
    }

    flocksVec.clear();
    while (query.next())
    {
        TFlockInformation flock;

        flock.mstrFlockID = query.value(0).toString();
        flock.mstrCreatorID = query.value(1).toString();
        flock.mstrCreateDate = query.value(2).toDate();
        flock.mstrFlockName = query.value(3).toString();
        flock.mstrTheme = query.value(4).toString();
        flocksVec.push_back(flock);
    }
//    mpSqlDatabase->close();
    if (flocksVec.isEmpty())
    {
        return EMT_NO_FLOCK;
    }
    return EMT_GET_ALL_FLOCKS_SUCCESS;
}

/*************************************************
Function Name： searchFlockInformation()
Description: 获取某个群信息
*************************************************/
bool TDatabaseCtrl::searchFlockInformation(const QString & flockID,
                                            TFlockInformation & flock)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_FLOCK_IN_FLOCKINFORMAIONT_SQL);
    query.addBindValue(flockID);
    query.exec();
    errorSQLOrder(query, "get-flock-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return false;
    }

    if (query.next())
    {
        flock.mstrFlockID = query.value(0).toString();
        flock.mstrCreatorID = query.value(1).toString();
        flock.mstrCreateDate = query.value(2).toDate();
        flock.mstrFlockName = query.value(3).toString();
        flock.mstrTheme = query.value(4).toString();
        return true;
    }
//    mpSqlDatabase->close();

    return false;
}

/*************************************************
Function Name： searchFlockMembers()
Description: 获取某群中所有的成员
*************************************************/
int TDatabaseCtrl::searchFlockMembers(
        const QString & id, QVector<TFlockMember> &flockMembersVec)
{
    if (!createConnection())
    {
        return EMT_GET_FLOCK_MEMBERS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_FLOCKMEMBERS_IN_USERINFORMATION_USERFLOCKS_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-flock-members-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_FLOCK_MEMBERS_FAIL;
    }

    flockMembersVec.clear();
    while (query.next())
    {
        TFlockMember flockMember;

        flockMember.mstrFlockID = query.value(0).toString();
        flockMember.mstrUserID = query.value(1).toString();
        flockMember.mnHeadPortrait = query.value(2).toInt();
        flockMember.mstrNickname = query.value(3).toString();
        flockMember.mnLoginStatus = query.value(4).toInt();
        flockMember.mstrRemark = query.value(5).toString();
        flockMember.mmFlockStatus = query.value(6).toInt();
        flockMember.mdateTimeJoin = query.value(7).toDateTime();
        flockMembersVec.push_back(flockMember);
    }
//    mpSqlDatabase->close();
    if (flockMembersVec.isEmpty())
    {
        return EMT_NO_FLOCK_MEMBER;
    }
    return EMT_GET_FLOCK_MEMBERS_SUCCESS;
}

/*************************************************
Function Name： searchFlockMember()
Description: 获取某群的某个成员
*************************************************/
bool TDatabaseCtrl::searchFlockMember(const QString & flockID,
                                       const QString & userID,
                                       TFlockMember & flockMember)
{

    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_FLOCKMEMBER_IN_USERINFORMATION_USERFLOCKS_SQL);
    query.addBindValue(flockID);
    query.addBindValue(userID);
    query.exec();
    errorSQLOrder(query, "get-flock-member-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return false;
    }

    if (query.next())
    {
        flockMember.mstrFlockID = query.value(0).toString();
        flockMember.mstrUserID = query.value(1).toString();
        flockMember.mnHeadPortrait = query.value(2).toInt();
        flockMember.mstrNickname = query.value(3).toString();
        flockMember.mnLoginStatus = query.value(4).toInt();
        flockMember.mstrRemark = query.value(5).toString();
        flockMember.mmFlockStatus = query.value(6).toInt();
        flockMember.mdateTimeJoin = query.value(7).toDateTime();
        return true;
    }
//    mpSqlDatabase->close();
    return false;
}


/*************************************************
Function Name： searchDiscussionsInformation()
Description: 获取所有用户加入的讨论组
*************************************************/
int TDatabaseCtrl::searchDiscussionsInformation(
        const QString & id, QVector<TDiscussionInformation> & discussionsVec)
{
    if (!createConnection())
    {
        return EMT_GET_ALL_DISCUSSIONS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_DISCUSSIONS_IN_DISCUSSIONINFORMAIONT_USERDISCUSSIONS_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-discussions-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_ALL_DISCUSSIONS_FAIL;
    }

    discussionsVec.clear();
    while (query.next())
    {
        TDiscussionInformation discussion;

        discussion.mstrDiscussionID = query.value(0).toString();
        discussion.mstrCreatorID = query.value(1).toString();
        discussion.mstrCreateDate = query.value(2).toDate();
        discussion.mstrDiscussionName = query.value(3).toString();
        discussion.mstrTheme = query.value(4).toString();
        discussionsVec.push_back(discussion);
    }
//    mpSqlDatabase->close();
    if (discussionsVec.isEmpty())
    {
        return EMT_NO_DISCUSSION;
    }
    return EMT_GET_ALL_DISCUSSIONS_SUCCESS;
}

/*************************************************
Function Name： searchDiscussionInformation()
Description: 获取某个讨论组信息
*************************************************/
bool TDatabaseCtrl::searchDiscussionInformation(const QString & discussionID,
                                                 TDiscussionInformation & discussion)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_DISCUSSION_IN_DISCUSSIONINFORMAIONT_SQL);
    query.addBindValue(discussionID);
    query.exec();
    errorSQLOrder(query, "get-discussion-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return false;
    }

    if (query.next())
    {
        discussion.mstrDiscussionID = query.value(0).toString();
        discussion.mstrCreatorID = query.value(1).toString();
        discussion.mstrCreateDate = query.value(2).toDate();
        discussion.mstrDiscussionName = query.value(3).toString();
        discussion.mstrTheme = query.value(4).toString();
        return true;
    }
//    mpSqlDatabase->close();

    return false;
}

/*************************************************
Function Name： searchDiscussionMembers()
Description: 获取某讨论组中所有的成员
*************************************************/
int TDatabaseCtrl::searchDiscussionMembers(
        const QString & id, QVector<TDiscussionMember> & discussionMembersVec)
{
    if (!createConnection())
    {
        return EMT_GET_DISCUSSION_MEMBERS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_DISCUSSIONMEMBERS_IN_USERINFORMATION_USERDISCUSSIONS_SQL);
    query.addBindValue(id);
    query.exec();
    errorSQLOrder(query, "get-discussion-members-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_DISCUSSION_MEMBERS_FAIL;
    }

    discussionMembersVec.clear();
    while (query.next())
    {
        TDiscussionMember flockMember;

        flockMember.mstrDiscussionID = query.value(0).toString();
        flockMember.mstrUserID = query.value(1).toString();
        flockMember.mnHeadPortrait = query.value(2).toInt();
        flockMember.mstrNickName = query.value(3).toString();
        flockMember.mnLoginStatus = query.value(4).toInt();
        flockMember.mnDiscussionStatus = query.value(5).toInt();
        flockMember.mdateTimejoin = query.value(6).toDateTime();
        discussionMembersVec.push_back(flockMember);
    }
//    mpSqlDatabase->close();
    if (discussionMembersVec.isEmpty())
    {
        return EMT_NO_DISCUSSION_MEMBER;
    }
    return EMT_GET_DISCUSSION_MEMBERS_SUCCESS;
}


/*************************************************
Function Name： searchDiscussionMember()
Description: 获取某讨论组中的某个成员
*************************************************/
bool TDatabaseCtrl::searchDiscussionMember(const QString & discussionID,
                                           const QString & userID,
                                           TDiscussionMember & discussionMember)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_DISCUSSIONMEMBER_IN_USERINFORMATION_USERDISCUSSIONS_SQL);
    query.addBindValue(discussionID);
    query.addBindValue(userID);
    query.exec();
    errorSQLOrder(query, "get-discussion-member-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return false;
    }

    if (query.next())
    {
        discussionMember.mstrDiscussionID = query.value(0).toString();
        discussionMember.mstrUserID = query.value(1).toString();
        discussionMember.mnHeadPortrait = query.value(2).toInt();
        discussionMember.mstrNickName = query.value(3).toString();
        discussionMember.mnLoginStatus = query.value(4).toInt();
//        discussionMember.mstrRemark = query.value(5).toString();
        discussionMember.mnDiscussionStatus = query.value(5).toInt();
        discussionMember.mdateTimejoin = query.value(6).toDateTime();
        return true;
    }
//    mpSqlDatabase->close();
    return false;
}

/*************************************************
Function Name： searchFlockHost()
Description: 获取群主帐号
*************************************************/
bool TDatabaseCtrl::searchFlockHost(const QString & flockID, QString & hostID)
{
    if (!createConnection())
    {
        return false;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_FLOCK_CREATORID_IN_FLOCKINFORMATION_SQL);
    query.addBindValue(flockID);
    query.exec();
    errorSQLOrder(query, "flock-creatorID-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return false;
    }
    if (query.size() <= 0)
    {
        mpSqlDatabase->close();
        return false;
    }
    if (query.next())
    {
        hostID = query.value(0).toString();
        return true;
    }
//    mpSqlDatabase->close();
    return false;
}


/*************************************************
Function Name： searchDiscussionHost()
Description: 获取组长帐号
*************************************************/
bool TDatabaseCtrl::searchDiscussionHost(const QString & discussionID,
                                          QString & hostID)
{
    if (!createConnection())
    {
        return false;
    }

    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_DISCUSSION_CREATORID_IN_DISCUSSIONINFORMATION_SQL);
    query.addBindValue(discussionID);
    query.exec();
    errorSQLOrder(query, "discussion-creatorID-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return false;
    }
    if (query.size() <= 0)
    {
        mpSqlDatabase->close();
        return false;
    }
    if (query.next())
    {
        hostID = query.value(0).toString();
        return true;
    }
//    mpSqlDatabase->close();
    return false;
}

/*************************************************
Function Name： addFlockMember()
Description: 添加群成员
*************************************************/
bool TDatabaseCtrl::addFlockMember(const QString & flockID,
                                    const QString & memberID,
                                    const int status)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);


    query.prepare(COUNT_FLOCKMEMBER_IN_USERFLOCKS_SQL);
    query.addBindValue(flockID);
    query.addBindValue(memberID);
    query.exec();

    errorSQLOrder(query, "count-flock member by id");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    if (0 != num)
        return false;

    query.prepare(ADD_MEMBER_IN_USERFLOCKS_SQL);
    query.addBindValue(flockID);
    query.addBindValue(memberID);
    query.addBindValue(status);
    query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.exec();
    errorSQLOrder(query, "add-flock-member");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return true;
}

/*************************************************
Function Name： addDiscussionMember()
Description: 添加讨论组成员
*************************************************/
bool TDatabaseCtrl::addDiscussionMember(const QString & discussionID,
                                        const QString & memberID,
                                        const int status)
{       
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();
    QSqlQuery query(*mpSqlDatabase);

    query.prepare(COUNT_DISCUSSIONMEMBER_IN_USERDISCUSSIONS_SQL);
    query.addBindValue(discussionID);
    query.addBindValue(memberID);
    query.exec();

    errorSQLOrder(query, "count-discussion member by id");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    if (0 != num)
        return false;

    query.prepare(ADD_MEMBER_IN_USERDISCUSSIONS_SQL);
    query.addBindValue(discussionID);
    query.addBindValue(memberID);
    query.addBindValue(status);
    query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.exec();
    errorSQLOrder(query, "add-discussion-member");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return true;
}

/*************************************************
Function Name： addFlock()
Description: 添加群
*************************************************/
int TDatabaseCtrl::addFlock(const TTempStrings & tmpStr, QString & flockID)
{
    int sum = getFlockTimes(QDate::currentDate());

    if (0 == sum)
    {
        return EMT_CREATE_FLOCK_FAIL;
    }

    QString id = QDate::currentDate().toString("yyyyMMdd");
    id.append(QString::number(1));
    id.append(QString::number(sum));
    flockID = id;

    if (!createConnection())
    {
        return EMT_CREATE_FLOCK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(ADD_FLOCK_IN_FLOCKINFORMATOIN_SQL);
    query.addBindValue(id);
    query.addBindValue(tmpStr.mstrOne);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    query.addBindValue(tmpStr.mstrTwo);
    query.addBindValue(tmpStr.mstrThree);
    query.exec();

    errorSQLOrder(query, "create-flock");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_CREATE_FLOCK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_CREATE_FLOCK_FAIL;
    }

    QSqlDatabase::database().commit();

    if (!addFlockMember(flockID, tmpStr.mstrOne, EFS_FLOCK_HOST))
        return EMT_CREATE_FLOCK_FAIL;

//    mpSqlDatabase->close();
    if (!increaseFlockTimes(QDate::currentDate()))
            return EMT_CREATE_FLOCK_FAIL;
    return EMT_CREATE_FLOCK_SUCCESS;
}

/*************************************************
Function Name： addDiscussion()
Description: 添加讨论组
*************************************************/
int TDatabaseCtrl::addDiscussion(const TTempStrings & tmpStr, QString & discussionID)
{
    int sum = getDiscussionTimes(QDate::currentDate());

    if (0 == sum)
    {
        return EMT_CREATE_DISCUSSION_FAIL;
    }

    QString id = QDate::currentDate().toString("yyyyMMdd");
    id.append(QString::number(2));
    id.append(QString::number(sum));
    discussionID = id;

    if (!createConnection())
    {
        return EMT_CREATE_DISCUSSION_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(ADD_DISCUSSION_IN_DISCUSSIONINFORMATOIN_SQL);
    query.addBindValue(id);
    query.addBindValue(tmpStr.mstrOne);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    query.addBindValue(tmpStr.mstrTwo);
    query.addBindValue(tmpStr.mstrThree);
    query.exec();

    errorSQLOrder(query, "create-discussion");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_CREATE_DISCUSSION_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_CREATE_DISCUSSION_FAIL;
    }

    QSqlDatabase::database().commit();

    if (!addDiscussionMember(discussionID, tmpStr.mstrOne, EDS_DISCUSSION_HOST))
        return EMT_CREATE_DISCUSSION_FAIL;

//    mpSqlDatabase->close();
    if (!increaseDiscussionTimes(QDate::currentDate()))
            return EMT_CREATE_DISCUSSION_FAIL;
    return EMT_CREATE_DISCUSSION_SUCCESS;

}

/*************************************************
Function Name： deleteFlock()
Description: 删除群
*************************************************/
int TDatabaseCtrl::deleteFlock(const QString userID, const QString & flockID)
{
    if (!createConnection())
    {
        return EMT_DROP_FLOCK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(DELETE_FLOCK_IN_FLOCKINFORMATION_SQL);
    query.addBindValue(flockID);
    query.addBindValue(userID);
    query.exec();

    errorSQLOrder(query, "delete-flock");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_DROP_FLOCK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_DROP_FLOCK_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_DROP_FLOCK_SUCCESS;
}

/*************************************************
Function Name： deleteDiscussion()
Description: 删除讨论组
*************************************************/
int TDatabaseCtrl::deleteDiscussion(const QString userID, const QString & discussionID)
{
    if (!createConnection())
    {
        return EMT_DROP_DISCUSSION_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(DELETE_DISCUSSION_IN_DISCUSSIONINFORMATION_SQL);
    query.addBindValue(discussionID);
    query.addBindValue(userID);
    query.exec();

    errorSQLOrder(query, "delete-discussion");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_DROP_DISCUSSION_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_DROP_DISCUSSION_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_DROP_DISCUSSION_SUCCESS;
}

/*************************************************
Function Name： deleteFlockMember()
Description: 删除群成员
*************************************************/
int TDatabaseCtrl::deleteFlockMember(const QString & flockID,
                                       const QString & memberID)
{
    if (!createConnection())
    {
        return EMT_LEAVE_FLCCK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(DELETE_FLOCKMEMEBER_IN_USERFLOCKS_SQL);
    query.addBindValue(flockID);
    query.addBindValue(memberID);
    query.exec();

    errorSQLOrder(query, "delete-flock-member");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_LEAVE_FLCCK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_LEAVE_FLCCK_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_LEAVE_FLOCK_SUCCESS;
}

/*************************************************
Function Name： deleteDiscussionMember()
Description: 删除讨论组成员
*************************************************/
int TDatabaseCtrl::deleteDiscussionMember(const QString & discussionID, const QString & memberID)
{
    if (!createConnection())
    {
        return EMT_LEAVE_DISCUSSION_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(DELETE_DISCUSSIONMEMEBER_IN_USERDISCUSSIONS_SQL);
    query.addBindValue(discussionID);
    query.addBindValue(memberID);
    query.exec();

    errorSQLOrder(query, "delete-discussion-member");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_LEAVE_DISCUSSION_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_LEAVE_DISCUSSION_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_LEAVE_DISCUSSION_SUCCESS;
}


/*************************************************
Function Name： updateFlockInformation()
Description: 更新群信息
*************************************************/
int TDatabaseCtrl::updateFlockInformation(const TFlockInformation & flock)
{
    if (!createConnection())
    {
        return EMT_CHANGE_FLOCK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(UPDATE_FLOCKINFORMATION_SQL);

    query.addBindValue(flock.mstrFlockName);
    query.addBindValue(flock.mstrTheme);
    query.addBindValue(flock.mstrFlockID);

    query.exec();

    errorSQLOrder(query, "update-flock-inf");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_CHANGE_FLOCK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_CHANGE_FLOCK_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_CHANGE_FLOCK_SUCCESS;
}


/*************************************************
Function Name： updateDiscussionInformation()
Description: 更新讨论组信息
*************************************************/
int TDatabaseCtrl::updateDiscussionInformation(const TDiscussionInformation & discussion)
{
    if (!createConnection())
    {
        return EMT_CHANGE_DISCUSSION_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(UPDATE_DISCUSSIONINFORMATION_SQL);

    query.addBindValue(discussion.mstrDiscussionName);
    query.addBindValue(discussion.mstrTheme);
    query.addBindValue(discussion.mstrDiscussionID);

    query.exec();

    errorSQLOrder(query, "update-discussion-inf");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_CHANGE_DISCUSSION_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_CHANGE_DISCUSSION_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_CHANGE_DISCUSSION_SUCCESS;
}


/*************************************************
Function Name： updateFlockMemberRemark()
Description: 更新群成员名片
*************************************************/
int TDatabaseCtrl::updateFlockMemberRemark(const TTempStrings & tmpStr)
{
    if (!createConnection())
    {
        return  EMT_CHANGE_FLOCK_REMARK_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(UPDATE_FLOCKMEMBER_REMAKR_IN_USERFLOCKS_SQL);

    query.addBindValue(tmpStr.mstrThree); // remark
    query.addBindValue(tmpStr.mstrOne);   // flockID
    query.addBindValue(tmpStr.mstrTwo);   // userID

    query.exec();

    errorSQLOrder(query, "update-flock-member-remark");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_CHANGE_FLOCK_REMARK_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_CHANGE_FLOCK_REMARK_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_CHANGE_FLOCK_REMARK_SUCCESS;
}


/*************************************************
Function Name： insertValuesInDateCounter()
Description: 插入DateCounterb表某日的数据项
*************************************************/
bool TDatabaseCtrl::insertValuesInDateCounter(const QDate & date)
{
    if (!createConnection())
    {
        return  false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(COUNT_DATARECORD_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "count-datecounter");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

//    if (0 >= query.numRowsAffected())
//    {
//        return false;
//    }

    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    if (num == 0)
    {
        query.prepare(INSERT_VALUES_IN_DATECOUNTER_SQL);
        query.addBindValue(date.toString("yyyy-MM-dd"));
        query.exec();

        errorSQLOrder(query, "insert datecounter");
        if (!query.isActive())
        {
            qDebug() << "QUERY_FAIL::" << query.lastError().text();
            mpSqlDatabase->close();
            return false;
        }

        if (0 >= query.numRowsAffected())
        {
            return false;
        }
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return true;
}

/*************************************************
Function Name： updateFlockMemberRemark()
Description: 获取某日用户注册数量+1
*************************************************/
int TDatabaseCtrl::getUserTimes(const QDate & date)
{
    if (!insertValuesInDateCounter(QDate::currentDate()))
        return 0;

    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SELECT_USERTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "select-usertimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

//    if (0 >= query.numRowsAffected())
//    {
//        return 0;
//    }

    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    QSqlDatabase::database().commit();

    return 1 + num;
}

/*************************************************
Function Name： getFlockTimes()
Description: 获取某日新建群数量+1
*************************************************/
int TDatabaseCtrl::getFlockTimes(const QDate & date)
{
    if (!insertValuesInDateCounter(QDate::currentDate()))
        return 0;

    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SELECT_FLOCKTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "select-flocktimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

//    if (0 >= query.numRowsAffected())
//    {
//        return 0;
//    }

    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    QSqlDatabase::database().commit();

    return 1 + num;
}

/*************************************************
Function Name： getDiscussionTimes()
Description: 获取某日新建讨论组数量+1
*************************************************/
int TDatabaseCtrl::getDiscussionTimes(const QDate & date)
{
    if (!insertValuesInDateCounter(QDate::currentDate()))
        return 0;

    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SELECT_DISCUSSIONTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "select-discussiontimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

//    if (0 >= query.numRowsAffected())
//    {
//        return 0;
//    }

    int num = 0;
    if (query.next())
    {
        num = query.value(0).toInt();
    }
    QSqlDatabase::database().commit();

    return 1 + num;
}


/*************************************************
Function Name： increaseUserTimes()
Description: 增加某日用户注册数量
*************************************************/
bool TDatabaseCtrl::increaseUserTimes(const QDate & date)
{
    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(INCREASE_USERTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "increase-usertimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }
    QSqlDatabase::database().commit();
    return true;
}

/*************************************************
Function Name： increaseFlockTimes()
Description: 增加某日新建群数量
*************************************************/
bool TDatabaseCtrl::increaseFlockTimes(const QDate & date)
{
    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(INCREASE_FLOCKTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "increase-flocktimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }
    QSqlDatabase::database().commit();
    return true;
}

/*************************************************
Function Name： increaseDiscussionTimes()
Description: 增加某日新建讨论组数量
*************************************************/
bool TDatabaseCtrl::increaseDiscussionTimes(const QDate & date)
{
    if (!createConnection())
    {
        return  0;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(INCREASE_DISCUSSIONTIMES_IN_DATECOUNTER_SQL);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.exec();

    errorSQLOrder(query, "increase-discussiontimes");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }
    QSqlDatabase::database().commit();
    return true;
}


/*************************************************
Function Name： searchInMails()
Description: 获取收件箱
*************************************************/
int TDatabaseCtrl::searchInMails(const QString & id,
                                  QVector<TMailInformation> & mailsVec)
{
    if (!createConnection())
    {
        return EMT_GET_IN_MAILS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_IN_MAILS_IN_MAIL);
    query.addBindValue(id);
    query.addBindValue(MAIL_ADDRESS_FORMAT);
    query.exec();
    errorSQLOrder(query, "get-mails-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_IN_MAILS_FAIL;
    }

    mailsVec.clear();
    TMailInformation mail;
    while (query.next())
    {
        mail.mnMailID = query.value(0).toInt();
        mail.mstrSenderAddress = query.value(1).toString();
//        mail. = query.value(2).toString();
        mail.mstrReceiverAddress = query.value(3).toString();
//        mail. = query.value(4).toString();
        mail.mstrTheme = query.value(5).toString();
        mail.mstrContent = query.value(6).toString();
        mail.mdateTime = query.value(7).toDateTime();
        mail.mnStatus = query.value(8).toInt();

        mailsVec.push_back(mail);
    }

    return EMT_GET_IN_MAILS_SUCCESS;
}


/*************************************************
Function Name： searchOutMails()
Description: 获取发件箱
*************************************************/
int TDatabaseCtrl::searchOutMails(const QString & id,
                                   QVector<TMailInformation> & mailsVec)
{
    if (!createConnection())
    {
        return EMT_GET_OUT_MAILS_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_OUT_MAILS_IN_MAIL);
    query.addBindValue(id);
    query.addBindValue(MAIL_ADDRESS_FORMAT);
    query.exec();
    errorSQLOrder(query, "get-mails-search");

    QSqlDatabase::database().commit();

    if (!query.isActive())
    {
        mpSqlDatabase->close();
        return EMT_GET_OUT_MAILS_FAIL;
    }

    mailsVec.clear();
    while (query.next())
    {
        TMailInformation mail;

        mail.mnMailID = query.value(0).toInt();
        mail.mstrSenderAddress = query.value(1).toString();
//        mail. = query.value(2).toString();
        mail.mstrReceiverAddress = query.value(3).toString();
//        mail. = query.value(4).toString();
        mail.mstrTheme = query.value(5).toString();
        mail.mstrContent = query.value(6).toString();
        mail.mdateTime = query.value(7).toDateTime();
//        mail.mnStatus = query.value(8).toInt();

        mailsVec.push_back(mail);
    }

    return EMT_GET_OUT_MAILS_SUCCESS;
}


/*************************************************
Function Name： deleteMail()
Description: 删除邮件
*************************************************/
bool TDatabaseCtrl::deleteMail(const QString & receiverID, const int mailID)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(DELETE_MAIL_IN_MAIL);
    query.addBindValue(receiverID);
    query.addBindValue(mailID);
    query.exec();

    errorSQLOrder(query, "delete-mail");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return true;
}

/*************************************************
Function Name： deleteInMail()
Description: 删除收件箱邮件
*************************************************/
int TDatabaseCtrl::deleteInMail(const QString & receiverID, const int mailID)
{
    if (!createConnection())
    {
        return EMT_DELETE_IN_MAIL_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(UPDATE_MAIL_IN_MAIL);
    query.addBindValue(receiverID);
    query.addBindValue(mailID);
    query.exec();

    errorSQLOrder(query, "delete-in-mail");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_DELETE_IN_MAIL_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_DELETE_IN_MAIL_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_DELETE_IN_MAIL_SUCCESS;
}

/*************************************************
Function Name： deleteOutMail()
Description: 删除发件箱邮件
*************************************************/
int TDatabaseCtrl::deleteOutMail(const QString & senderID, const int mailID)
{
    if (!createConnection())
    {
        return EMT_DELETE_OUT_MAIL_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(UPDATE_MAIL_OUT_MAIL);
    query.addBindValue(senderID);
    query.addBindValue(mailID);
    query.exec();

    errorSQLOrder(query, "delete-out-mail");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_DELETE_OUT_MAIL_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_DELETE_OUT_MAIL_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_DELETE_OUT_MAIL_SUCCESS;
}


/*************************************************
Function Name： insertMail()
Description: 插入新邮件
*************************************************/
int TDatabaseCtrl::insertMail(TMailInformation & mail)
{
    if (!createConnection())
    {
        return EMT_SEND_MAIL_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(INSERT_MAIL_IN_MAIL);
    query.addBindValue(mail.mstrSenderAddress);
    query.addBindValue(MAIL_ADDRESS_FORMAT);
    query.addBindValue(mail.mstrReceiverAddress);
    query.addBindValue(MAIL_ADDRESS_FORMAT);
    query.addBindValue(mail.mstrTheme);
    query.addBindValue(mail.mstrContent);
    query.addBindValue(mail.mdateTime);
    query.exec();

    errorSQLOrder(query, "insert-mail");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_SEND_MAIL_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_SEND_MAIL_FAIL;
    }
    QSqlDatabase::database().commit();

    query.exec("select LAST_INSERT_ID() from Mail");
    if (query.next())
    {
        mail.mnMailID = query.value(0).toInt();
    }

//    mpSqlDatabase->close();
    return EMT_SEND_MAIL_SUCCESS;
}


/*************************************************
Function Name： setMailRead()
Description: 更新邮件，设置为已读
*************************************************/
bool TDatabaseCtrl:: setMailRead(const QString & receiverID, const int mailID)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SET_MAIL_READ_IN_MAIL);
    query.addBindValue(receiverID);
    query.addBindValue(mailID);
    query.exec();

    errorSQLOrder(query, "set-mail-read");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return true;
}


/*************好友分组********************/


/*************************************************
Function Name： moveFriendToGroup()
Description: 移动好友至其他分组
*************************************************/
int TDatabaseCtrl::moveFriendToGroup(const QString & myID, const QString & friendID,
                      const QString & title, const QString & newTitle)
{
    if (!createConnection())
    {
        return EMT_MOVE_FRIEND_BOX_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(MOVE_FRIEND_IN_USERFREINS);
    query.addBindValue(newTitle);
    query.addBindValue(myID);
    query.addBindValue(friendID);
//    query.addBindValue(title);
    query.exec();

    errorSQLOrder(query, "move-friend-to-group");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_MOVE_FRIEND_BOX_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_MOVE_FRIEND_BOX_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_MOVE_FRIEND_BOX_SUCCESS;
}


/*************************************************
Function Name： renameGroup()
Description: 重命名分组
*************************************************/
int TDatabaseCtrl::renameGroup(const QString & id, const QString & title,
                const QString & newTitle)
{
    if (!createConnection())
    {
        return EMT_RENAME_BOX_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(RENAME_GROUPNAME_IN_USERFREINS);
    query.addBindValue(newTitle);
    query.addBindValue(id);
    query.addBindValue(title);
    query.exec();

    errorSQLOrder(query, "rename-group");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_RENAME_BOX_FAIL;
    }

    if (0 >= query.numRowsAffected())
    {
        return EMT_RENAME_BOX_FAIL;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_RENAME_BOX_SUCCESS;
}

/*************************************************
Function Name： addHistoryMessage()
Description: 存储历史记录
*************************************************/
bool TDatabaseCtrl::addHistoryMessage(const TTalkMessage & mes)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(INSERT_MESSAGE_IN_HISTORYMESSAGE);
    query.addBindValue(mes.mstrSenderID);
    query.addBindValue(mes.mstrReceiverID);
    query.addBindValue(simplifyRichText(mes.mstrText));
    query.addBindValue(mes.mdateTime);


    query.exec();

    errorSQLOrder(query, "inser-mes-to-HistoryMmesage");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return true;
}

/*************************************************
Function Name： addOfflineMessage()
Description: 存储离线记录
*************************************************/
bool TDatabaseCtrl::addOfflineMessage(const TTalkMessage & mes)
{
    if (!createConnection())
    {
        return false;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(INSERT_MESSAGE_IN_OFFLINEMESSAGE);
    query.addBindValue(mes.mnType);
    query.addBindValue(mes.mstrSenderID);
    query.addBindValue(mes.mstrReceiverID);
    query.addBindValue(mes.mstrText);
    query.exec();

    errorSQLOrder(query, "inser-mes-to-Offline-Mmesage");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return false;
    }

    if (0 >= query.numRowsAffected())
    {
        return false;
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return true;
}


/*************************************************
Function Name： searchHistoryMessageOneFriendOneDate()
Description: 获取用户的某日与某位好友的聊天记录
*************************************************/
int TDatabaseCtrl::searchHistoryMessageOneFriendOneDate(
        const QString & myID, const QString &friendID,
        const QDate & date, QVector<TTalkMessage> & mesVec)
{
    if (!createConnection())
    {
        return EMT_GET_HISTORY_MESSAGE_FAIL;
    }
    //sql 事务
    QSqlDatabase::database().transaction();

    QSqlQuery query(*mpSqlDatabase);
    query.prepare(SEARCH_FRIEND_DATE_MESSAGE_IN_HISTORYMESSAGE);
    query.addBindValue(myID);
    query.addBindValue(friendID);
    query.addBindValue(friendID);
    query.addBindValue(myID);
    query.addBindValue(date);
    query.addBindValue(date.addDays(1));
    query.exec();

    errorSQLOrder(query, "search-history-Mmesage-friend-date");
    if (!query.isActive())
    {
        qDebug() << "QUERY_FAIL::" << query.lastError().text();
        mpSqlDatabase->close();
        return EMT_GET_HISTORY_MESSAGE_FAIL;
    }

    mesVec.clear();
//    if (0 >= query.numRowsAffected())
//    {
//        return NO_HISTORY_MESSAGE;
//    }
    TTalkMessage mes;
    while (query.next())
    {
        mes.mstrSenderID = query.value(0).toString();
        mes.mstrReceiverID = query.value(1).toString();
        mes.mstrText = query.value(2).toString();
        mes.mdateTime = query.value(3).toDateTime();
        mesVec.push_back(mes);
    }

    QSqlDatabase::database().commit();

//    mpSqlDatabase->close();
    return EMT_GET_HISTORY_MESSAGE_SUCCESS;
}

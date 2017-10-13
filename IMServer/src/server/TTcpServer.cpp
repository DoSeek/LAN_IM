/*********************************************
File Name： TTcpServer.cpp
Author： jet.F.R
Date： 2014.3.10
Description： 服务器类
Changes：
********************************************/

#include "TTcpServer.h"

TTcpServer::TTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    // 初始化一下数据库
}

TTcpServer::~TTcpServer()
{
}

/*************************************************
Function Name： clientDisconnected()
Description: 客户端断开连接
*************************************************/
void TTcpServer::clientDisconnected(const QString &id)
{
    //某个客户端断开连接时，就更新该用户的状态
    if (id.contains(MAIL_ADDRESS_FORMAT))
    {
        mmapOnlineUserMail.remove(id);
        emit haveLeftAddress(id);
    }
    else
    {
        mmapOnlineUser.remove(id);
        emit haveLeftID(id);
        if (mDatabaseCtrl.updateStatus(ELS_OFFLINE, id))
            tellFriendsStatusChanged(id, ELS_OFFLINE);
    }

#ifdef TEST_CODE
    qDebug() << "userMap-size:" << mmapOnlineUser.size();
    QMapIterator<QString, TClientSocketCtrl *> i(mmapOnlineUser);
    while (i.hasNext()) {
        i.next();
        qDebug() << i.key();
    }

    qDebug() << "userMailMap-size:" << mmapOnlineUserMail.size();
    QMapIterator<QString, TClientSocketCtrl *> j(mmapOnlineUserMail);
    while (j.hasNext()) {
        j.next();
        qDebug() << j.key();
    }
#endif
}


/*************************************************
Function Name： sendMessage()
Description: 处理请求，要求clientSocket发送信息
*************************************************/
void TTcpServer::sendMessage(const TSaveTmpInformation &save)
{
    mSaveTmpInfo.mnRequestKind = save.mnRequestKind;
    //根据save.kind来处理各种信息
    switch (mSaveTmpInfo.mnRequestKind)
    {
    case EMT_LOGIN:
    {
        mSaveTmpInfo.mLoginInf = save.mLoginInf;
        mSaveTmpInfo.mpClientSocket = save.mpClientSocket;
        // 判断是否已经登录
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        if (mmapOnlineUser.contains(mSaveTmpInfo.mstrMyID))
            mSaveTmpInfo.mnReplyKind = EMT_HAVE_LOGINED;
        else
        {
            // 访问数据库 登录
            mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchUserInUserInformation(
                        mSaveTmpInfo.mLoginInf, mSaveTmpInfo.mUserInf);//, mSaveTmpInfo.friendsVec);
//            if(LOGIN_SUCCESS == mSaveTmpInfo.mnReplyKind)
//                changeStatu(mSaveTmpInfo.m_loginInf.m_userID, mSaveTmpInfo.m_loginInf.mnStatus);
//                mmapOnlineUser.insert(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mpClientSocket);
        }
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        // mark改变登录状态
        //changeStatu(mSaveTmpInfo.m_loginInf.m_userID, mSaveTmpInfo.m_loginInf.mnStatus);
        break;
    }
    case EMT_REGISTER:
    {
        mSaveTmpInfo.mUserInf = save.mUserInf;
//        QDateTime dateTime;
//        dateTime.setDate(QDate::currentDate());
//        int sum = mDatabaseCtrl.countRegisteredInPeriod(dateTime, dateTime.addDays(1));

        int sum = mDatabaseCtrl.getUserTimes(QDate::currentDate());
        if (0 == sum)
        {
            mSaveTmpInfo.mnReplyKind = EMT_REGISTER_FAIL;
        }
        else
        {
            QString id = QDate::currentDate().toString("yyyyMMdd");
            id.append(QString::number(0));
            id.append(QString::number(sum));
            mSaveTmpInfo.mUserInf.mstrUserID = id;
//            qDebug() << "regDate:" << mSaveTmpInfo.mUserInf.m_regDateTime ;
            mSaveTmpInfo.mUserInf.mRegDateTime = QDateTime::currentDateTime();
            // 访问数据库 注册
            mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.addUserInUserInformation(mSaveTmpInfo.mUserInf);
        }
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_GET_QUESTION_ANSWER:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
//        mSaveTmpInfo.mTempStr.m_one = save.mstrMyID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchQuestionAndAnswer(mSaveTmpInfo.mstrMyID,
                                                                mSaveTmpInfo.mTempStr);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_CHECK_MESSAGE:
        break;
    case EMT_GET_ALL_FRIENDS:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mnStatus = save.mnStatus;
        mSaveTmpInfo.mpClientSocket = save.mpClientSocket;
        // 访问数据库 查询所有好友信息
//        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchUserInUserInformation(
//                    mSaveTmpInfo.m_loginInf, mSaveTmpInfo.mUserInf);//, mSaveTmpInfo.friendsVec);
//        QMultiMap<QString, FriendsInfo> *userMap= new QMultiMap<QString, FriendsInfo>;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchFriendsInformation(mSaveTmpInfo.mstrMyID,
                                                                 mSaveTmpInfo.mFriendsVec);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        if (EMT_GET_ALL_FRIENDS_FAIL != mSaveTmpInfo.mnReplyKind)
        {
            if (!mmapOnlineUser.contains(mSaveTmpInfo.mstrMyID))
            {
                mmapOnlineUser.insert(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mpClientSocket);
                emit haveNewID(mSaveTmpInfo.mstrMyID);
            }

            tellFriendsStatusChanged(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mnStatus);
        }
        break;
    }
    case EMT_GET_ALL_FLOCKS:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        // 获取 群信息
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchFlocksInformation(
                    mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mFlocksVec);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
//        if (GET_ALL_FLOCKS_SUCCESS != mSaveTmpInfo.mnReplyKind)
//            break;
//// 遍历群
//        // 获取 群成员信息
//        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchFlocksMembers(
//                    mSaveTmpInfo.mstrMyID,mSaveTmpInfo.m_flocksMembersVec);
//        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_GET_ALL_DISCUSSIONS:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        // 获取 讨论组信息
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchDiscussionsInformation(
                    mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mDiscussionsVec);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
//        if (GET_ALL_DISCUSSIONS_SUCCESS != mSaveTmpInfo.mnReplyKind)
//            break;
//// 遍历组
//        // 获取 讨论组成员信息
//        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchDiscussionsMembers(
//                    mSaveTmpInfo.mstrMyID,mSaveTmpInfo.m_discussionsMembersVec);
//        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_GET_FLOCK_MEMBERS:
    {
        mSaveTmpInfo.mstrFlockID = save.mstrFlockID;
        // 获取群成员
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchFlockMembers(
                    mSaveTmpInfo.mstrFlockID, mSaveTmpInfo.mFlockMembersVec);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_GET_DISCUSSIONS_MEMBERS:
    {
        mSaveTmpInfo.mstrDiscussionID = save.mstrDiscussionID;
        // 获取群成员
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchDiscussionMembers(
                    mSaveTmpInfo.mstrDiscussionID, mSaveTmpInfo.mDiscussionMembersVec);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_TALK:
    {
        talkRequest(save);
        break;
    }
    case EMT_CHANGE_INFORMATION:
    {
        mSaveTmpInfo.mUserInf = save.mUserInf;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.updateMyselfInformation(save.mUserInf);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        if (mSaveTmpInfo.mnReplyKind == EMT_CHANGE_INFORMATION_SUCCESS)
        {
//            if (mDatabaseCtrl.searchFriendSimInformation(save.mFriendInf))
                tellFriendsInformationChanged(mSaveTmpInfo.mUserInf.mstrUserID);
        }
        break;
    }
    case EMT_GET_MYSELF_INFORMATION:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchMyselfInformationByID(
                    mSaveTmpInfo.mstrMyID,
                    mSaveTmpInfo.mUserInf);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_GET_FRIEND_INFORMATION:
    {
        mSaveTmpInfo.mstrPeerID = save.mstrPeerID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchFriendInformationByID(
                    mSaveTmpInfo.mstrPeerID,
                    mSaveTmpInfo.mUserInf);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }

    case EMT_DELETE_FRIEND:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrPeerID = save.mstrPeerID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.deleteFriend(mSaveTmpInfo.mstrMyID,
                                                     mSaveTmpInfo.mstrPeerID,
                                                     mSaveTmpInfo.mstrGroupName,
                                                     mSaveTmpInfo.mstrGroupNameF);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);

        // 如果好友在线，就发送被删除信息， 如果不在线，就先存储
        QMap<QString, TClientSocketCtrl*>::iterator iter;
        iter = mmapOnlineUser.find(mSaveTmpInfo.mstrPeerID);
        if(mmapOnlineUser.end() == iter)
        {
            TTalkMessage mes;
            mes.mnType = EMT_BE_DELETED;
            mes.mstrSenderID = save.mstrMyID;
            mes.mstrReceiverID = save.mstrPeerID;
            mes.mstrText = save.mstrGroupNameF;
            // 存储离线消息
            //mDatabaseCtrl.messageRequest(save.message);
        }
        else
        {
            mSaveTmpInfo.mstrMyID = save.mstrPeerID;
            mSaveTmpInfo.mstrPeerID = save.mstrMyID;
            mSaveTmpInfo.mnReplyKind = EMT_BE_DELETED;
            mSaveTmpInfo.mstrGroupName = mSaveTmpInfo.mstrGroupNameF;
            iter.value()->sendMessage(mSaveTmpInfo);
        }
        break;
    }
    case EMT_CHANGE_REMARK:
    {
        mSaveTmpInfo.mTempStr = save.mTempStr;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.updateRemark(save.mTempStr);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
//        if (mSaveTmpInfo.mnReplyKind == CHANGE_INFORMATION_SUCCESS)
//        {
//        }
        break;
    }
    case EMT_ADD_FRIEND:
    {
        friendRequest(save);
        break;
    }
    case EMT_CHANGE_STATUE:
    {
        if (mDatabaseCtrl.updateStatus(save.mnStatus, save.mstrMyID))
            tellFriendsStatusChanged(save.mstrMyID, save.mnStatus);
        break;
    }
    case EMT_ADD_FLOCK:
    {
        flockRequest(save);
        break;
    }
    case EMT_DROP_FLOCK:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrFlockID = save.mstrFlockID;

        tellMembersFlockDroped(mSaveTmpInfo.mstrFlockID);

        /*mSaveTmpInfo.mnReplyKind = */
        mDatabaseCtrl.deleteFlock(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mstrFlockID);
//        if (DROP_FLOCK_SUCCESS == mSaveTmpInfo.mnReplyKind)
//        {

//        }
        break;
    }
    case EMT_LEAVE_FLOCK:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrFlockID = save.mstrFlockID;

      //  tellMembersFlockDroped(mSaveTmpInfo.mstrFlockID);

        mSaveTmpInfo.mnReplyKind =
                mDatabaseCtrl.deleteFlockMember(mSaveTmpInfo.mstrFlockID, mSaveTmpInfo.mstrMyID);

        // 如果删除成功，通知群主，群成员退出群
        if ( EMT_LEAVE_FLOCK_SUCCESS == mSaveTmpInfo.mnReplyKind)
        {
            // 通过群号flockID 获取到 群主号peerID
            if (mDatabaseCtrl.searchFlockHost(mSaveTmpInfo.mstrFlockID, mSaveTmpInfo.mstrPeerID))
            {
//                save.mpClientSocket->sendMessage(mSaveTmpInfo);

                // 如果群主在线，就发送群成员退出群信息， 如果不在线，就先存储
                QMap<QString, TClientSocketCtrl*>::iterator iter;
                iter = mmapOnlineUser.find(mSaveTmpInfo.mstrPeerID);
                if(mmapOnlineUser.end() == iter)
                {
                    TTalkMessage mes;
                    mes.mnType = EMT_LEAVE_FLOCK_SUCCESS;
                    mes.mstrSenderID = save.mstrMyID;
                    mes.mstrReceiverID = save.mstrPeerID;
//                    mes.m_text = save.mstrGroupNameF;
                    // 存储离线消息
                    //mDatabaseCtrl.messageRequest(save.message);
                }
                else
                {
//                    TTalkMessage mes;
//                    mes.m_type = LEAVE_FLOCK_SUCCESS;
//                    mes.mstrSenderID = save.mstrMyID;
//                    mes.m_receiverID = save.mstrMyID;
                    iter.value()->sendMessage(mSaveTmpInfo);
                }

                // 如果群成员在线，就发送群成员退出群信息， 如果不在线，就先存储
//                QMap<QString, TClientSocketCtrl*>::iterator iter;
                iter = mmapOnlineUser.find(mSaveTmpInfo.mstrMyID);
                if(mmapOnlineUser.end() == iter)
                {
                    TTalkMessage mes;
                    mes.mnType = EMT_LEAVE_FLOCK_SUCCESS;
                    mes.mstrSenderID = save.mstrMyID;
                    mes.mstrReceiverID = save.mstrPeerID;
//                    mes.m_text = save.mstrGroupNameF;
                    // 存储离线消息
                    //mDatabaseCtrl.messageRequest(save.message);
                }
                else
                {
//                    TTalkMessage mes;
//                    mes.m_type = LEAVE_FLOCK_SUCCESS;
//                    mes.mstrSenderID = save.mstrMyID;
//                    mes.m_receiverID = save.mstrMyID;
                    iter.value()->sendMessage(mSaveTmpInfo);
                }


            }
//            tellFlockMemberHaveLeavedMember(mSaveTmpInfo.mstrFlockID);

            tellFlockMemberHaveLeavedMember(mSaveTmpInfo.mstrFlockID);
        }
//        save.mpClientSocket->sendMessage(mSaveTmpInfo);


        break;
    }

    case EMT_CHANGE_FLOCK:
    {
        mSaveTmpInfo.mFlockInf = save.mFlockInf;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.updateFlockInformation(mSaveTmpInfo.mFlockInf);
        if (EMT_CHANGE_FLOCK_SUCCESS == mSaveTmpInfo.mnReplyKind)
        {
            tellMembersFlockChanged(mSaveTmpInfo.mFlockInf);
        }
        break;
    }
    case EMT_CHANGE_FLOCK_REMARK:
    {
        mSaveTmpInfo.mTempStr = save.mTempStr;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.updateFlockMemberRemark(mSaveTmpInfo.mTempStr);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_CREATE_FLOCK:
    {
        mSaveTmpInfo.mTempStr = save.mTempStr;
        QString flockID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.addFlock(mSaveTmpInfo.mTempStr, flockID);
        if (EMT_CREATE_FLOCK_SUCCESS == mSaveTmpInfo.mnReplyKind)
        {
            mDatabaseCtrl.searchFlockInformation(flockID, mSaveTmpInfo.mFlockInf);
        }
            save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }

    case EMT_ADD_DISCUSSION:
    {
        discussionRequest(save);
        break;
    }
    case EMT_DROP_DISCUSSION:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrDiscussionID = save.mstrDiscussionID;

        tellMembersDiscussionDroped(mSaveTmpInfo.mstrDiscussionID);

        /*mSaveTmpInfo.mnReplyKind = */
        mDatabaseCtrl.deleteDiscussion(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mstrDiscussionID);
//        if (DROP_FLOCK_SUCCESS == mSaveTmpInfo.mnReplyKind)
//        {

//        }
        break;
    }
    case EMT_LEAVE_DISCUSSION:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrDiscussionID = save.mstrDiscussionID;

      //  tellMembersFlockDroped(mSaveTmpInfo.mstrFlockID);

        mSaveTmpInfo.mnReplyKind  =  mDatabaseCtrl.deleteDiscussionMember(mSaveTmpInfo.mstrDiscussionID,
                                                           mSaveTmpInfo.mstrMyID);

        // 如果删除成功
        if ( EMT_LEAVE_DISCUSSION_SUCCESS == mSaveTmpInfo.mnReplyKind)
        {

            {
            #if 0
//            // 通过组号flockID 获取到 组长号peerID
//            if (mDatabaseCtrl.searchFlockHost(mSaveTmpInfo.mstrDiscussionID, mSaveTmpInfo.mstrPeerID))
//            {
////                save.mpClientSocket->sendMessage(mSaveTmpInfo);

//                // 如果组长在线，就发送成员退出信息， 如果不在线，就先存储
//                QMap<QString, TClientSocketCtrl*>::iterator iter;
//                iter = mmapOnlineUser.find(mSaveTmpInfo.mstrPeerID);
//                if(mmapOnlineUser.end() == iter)
//                {
//                    TTalkMessage mes;
//                    mes.m_type = LEAVE_DISCUSSION_SUCCESS;
//                    mes.mstrSenderID = save.mstrMyID;
//                    mes.m_receiverID = save.mstrPeerID;
////                    mes.m_text = save.mstrGroupNameF;
//                    // 存储离线消息
//                    //mDatabaseCtrl.messageRequest(save.message);
//                }
//                else
//                {
////                    TTalkMessage mes;
////                    mes.m_type = LEAVE_FLOCK_SUCCESS;
////                    mes.mstrSenderID = save.mstrMyID;
////                    mes.m_receiverID = save.mstrMyID;
//                    iter.value()->sendMessage(mSaveTmpInfo);
//                }

//                // 如果成员在线，就发送成员退出群信息， 如果不在线，就先存储
////                QMap<QString, TClientSocketCtrl*>::iterator iter;
////                iter = mmapOnlineUser.find(mSaveTmpInfo.mstrMyID);
////                if(mmapOnlineUser.end() == iter)
////                {
////                    TTalkMessage mes;
////                    mes.m_type = LEAVE_DISCUSSION_SUCCESS;
////                    mes.mstrSenderID = save.mstrMyID;
////                    mes.m_receiverID = save.mstrPeerID;
//////                    mes.m_text = save.mstrGroupNameF;
////                    // 存储离线消息
////                    //mDatabaseCtrl.messageRequest(save.message);
////                }
////                else
//                {
////                    TTalkMessage mes;
////                    mes.m_type = LEAVE_FLOCK_SUCCESS;
////                    mes.mstrSenderID = save.mstrMyID;
////                    mes.m_receiverID = save.mstrMyID;
//                    iter.value()->sendMessage(mSaveTmpInfo);
//                }
//            }
            #endif
            }
            mSaveTmpInfo.mnReplyKind = EMT_LEAVE_DISCUSSION_MEMBER;

//            // 发送成员退出讨论组信息
//            QMap<QString, TClientSocketCtrl*>::iterator iter;
//            iter = mmapOnlineUser.find(mSaveTmpInfo.mstrMyID);
//            if(mmapOnlineUser.end() != iter)
//                iter.value()->sendMessage(mSaveTmpInfo);
            // 通知讨论组成员， 有成员退出
            tellDiscussionMemberHaveLeavedMember(mSaveTmpInfo.mstrDiscussionID);

        }
//        TTalkMessage mes;
//        mes.m_type = LEAVE_DISCUSSION_SUCCESS;
//        mes.mstrSenderID = save.mstrMyID;
//        mes.m_receiverID = save.mstrPeerID;
//        save.mpClientSocket->sendMessage(mSaveTmpInfo);

        save.mpClientSocket->sendMessage(mSaveTmpInfo);

        break;
    }

    case EMT_CHANGE_DISCUSSION:
    {
        mSaveTmpInfo.mDiscussionInf = save.mDiscussionInf;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.updateDiscussionInformation(mSaveTmpInfo.mDiscussionInf);
        if (EMT_CHANGE_DISCUSSION_SUCCESS == mSaveTmpInfo.mnReplyKind)
        {
            tellMembersDiscussionChanged(mSaveTmpInfo.mDiscussionInf);
        }
        break;
    }
    case EMT_CREATE_DISCUSSION:
    {
        mSaveTmpInfo.mTempStr = save.mTempStr;
        QString discussionID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.addDiscussion(mSaveTmpInfo.mTempStr, discussionID);
        if (EMT_CREATE_DISCUSSION_SUCCESS == mSaveTmpInfo.mnReplyKind)
        {
            mDatabaseCtrl.searchDiscussionInformation(discussionID, mSaveTmpInfo.mDiscussionInf);
        }
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_GET_IN_MAILS:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchInMails(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mInMailsVec);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        if (EMT_GET_IN_MAILS_SUCCESS == mSaveTmpInfo.mnReplyKind)
        {
            QString mailAddress =  save.mstrMyID;
            mailAddress.append(MAIL_ADDRESS_FORMAT);
            if (mmapOnlineUserMail.contains(mailAddress))
            {
                qDebug() << "have connected:" << mailAddress;
            }
            else
            {
                mmapOnlineUserMail.insert(mailAddress, save.mpClientSocket);
                emit haveNewAddress(mailAddress);
            }
        }
        break;
    }
    case EMT_GET_OUT_MAILS:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchOutMails(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mOutMailsVec);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_DELETE_IN_MAIL:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mnMailID = save.mnMailID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.deleteInMail(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mnMailID);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_DELETE_OUT_MAIL:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mnMailID = save.mnMailID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.deleteOutMail(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mnMailID);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_SEND_MAIL:
    {
        mSaveTmpInfo.mMail = save.mMail;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.insertMail(mSaveTmpInfo.mMail);

        save.mpClientSocket->sendMessage(mSaveTmpInfo);
//        if (mDatabaseCtrl.searchMail(mSaveTmpInfo.mMail.))
//        {
//            return;
//        }
        if (mSaveTmpInfo.mnReplyKind == EMT_SEND_MAIL_SUCCESS)
        {
            QString mailAddress =  mSaveTmpInfo.mMail.mstrReceiverAddress;
            mailAddress.append(MAIL_ADDRESS_FORMAT);

            QMap<QString, TClientSocketCtrl*>::iterator iter;
            iter = mmapOnlineUserMail.find(mailAddress);
            if(mmapOnlineUserMail.end() != iter)
            {
                mSaveTmpInfo.mnReplyKind = EMT_RECEIVE_MAIL;
                iter.value()->sendMessage(mSaveTmpInfo);
            }
        }
        break;
    }
    case EMT_SET_MAIL_READ:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mnMailID = save.mnMailID;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.setMailRead(mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mnMailID);
//        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }

    case EMT_REMOVE_BOX:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrTitle = save.mstrTitle;
        break;
    }
    case EMT_CREATE_BOX:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrTitle = save.mstrTitle;
        break;
    }
    case EMT_MOVE_FRIEND_BOX:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrTitle = save.mstrTitle;
        mSaveTmpInfo.mstrPeerID = save.mstrPeerID;
        mSaveTmpInfo.mstrNewTitle = save.mstrNewTitle;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.moveFriendToGroup(
                    mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mstrPeerID,
                    mSaveTmpInfo.mstrTitle, mSaveTmpInfo.mstrNewTitle);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_RENAME_BOX:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrTitle = save.mstrTitle;
        mSaveTmpInfo.mstrNewTitle = save.mstrNewTitle;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.renameGroup(
                    mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mstrTitle, mSaveTmpInfo.mstrNewTitle);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_GET_HISTORY_MESSAGE:
    {
        mSaveTmpInfo.mstrMyID = save.mstrMyID;
        mSaveTmpInfo.mstrPeerID = save.mstrPeerID;
        mSaveTmpInfo.mDate = save.mDate;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchHistoryMessageOneFriendOneDate(
                    mSaveTmpInfo.mstrMyID, mSaveTmpInfo.mstrPeerID,
                    mSaveTmpInfo.mDate, mSaveTmpInfo.mHistoryMessageVec);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    case EMT_CHANGE_PASSWORD:
    {
        mSaveTmpInfo.mTempStr = save.mTempStr;
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.updatePassword(mSaveTmpInfo.mTempStr);
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        break;
    }
    default:
        break;
    }
}


/*************************************************
Function Name： incomingConnection()
Description: 当有新的连接时，会调用此函数。用于创建新的线程去控制socket通信
Input： int socketDescriptor：socket标识，用于创建socket
Output：nullptr
Changes： nullptr
*************************************************/
void TTcpServer::incomingConnection(int socketDescriptor)
{
//    IMClientSocket *socket = new IMClientSocket(this);
//    socket->setSocketDescriptor(socketDescriptor);
//    connect(this, SIGNAL(readyRead()), this, SLOT(readMessage()));
//    qDebug("new");
//    IMThread *thread = new IMThread(this, socketDescriptor);
//    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    //当线程结束时，将socket从map中删除
//    //connect(thread, SIGNAL(finished()), this, SLOT(deleteSocketFormMap()));
//    //connect(thread, SIGNAL())
//    thread->start();

    qDebug() << "asdfas";
    //建立新连接，并连接相应的信号/糟
    TClientSocketCtrl *clientSocket = new TClientSocketCtrl(this);
    clientSocket->setSocketDescriptor(socketDescriptor);
    connect(clientSocket, SIGNAL(deleteSignal(const QString &)),
        this, SLOT(clientDisconnected(const QString &)));
    connect(clientSocket, SIGNAL(sendSignal(const TSaveTmpInformation &)),
        this, SLOT(sendMessage(const TSaveTmpInformation &)));

    qDebug() <<"new client IP:" << clientSocket->peerAddress();

    qDebug() << "userMap-size:" << mmapOnlineUser.size();

#ifdef TEST_CODE
    QMapIterator<QString, TClientSocketCtrl *> i(mmapOnlineUser);
    while (i.hasNext()) {
        i.next();
        qDebug() << i.key();
    }

    qDebug() << "userMailMap-size:" << mmapOnlineUserMail.size();
    QMapIterator<QString, TClientSocketCtrl *> j(mmapOnlineUserMail);
    while (j.hasNext()) {
        j.next();
        qDebug() << j.key();
    }
#endif
}

/*************************************************
Function Name： friendRequest()
Description:  处理好友请求
*************************************************/
void TTcpServer::friendRequest(const TSaveTmpInformation &save)
{
    mSaveTmpInfo.mnReplyKind = EMT_ADD_FRIEND;
    mSaveTmpInfo.mMessage = save.mMessage;
    switch (save.mMessage.mnType)
    {
    case EMT_REQUEST_FRIEND:
    {
        if (EMT_GET_FRIEND_SUCCESS != mDatabaseCtrl.searchFriendInformationByID(
                 mSaveTmpInfo.mMessage.mstrSenderID, mSaveTmpInfo.mUserInf))
        {
            return;
        }
        qDebug() << "request add friend";
        break;
    }
    case EMT_REFUSE_FRIEND:
        qDebug() << "refuse add friend";
        break;
    case EMT_AGREE_FRIEND:
    {
        if (!mDatabaseCtrl.addFriend(mSaveTmpInfo.mMessage))
        {
            mSaveTmpInfo.mMessage.mnType = EMT_ADD_FRIEND_FAIL;
            break;
        }

        mSaveTmpInfo.mMessage.mstrSenderID = save.mMessage.mstrReceiverID;
        mSaveTmpInfo.mMessage.mstrReceiverID = save.mMessage.mstrSenderID;
        if (EMT_GET_FRIEND_SUCCESS != mDatabaseCtrl.searchFriendInformationByID(
                 mSaveTmpInfo.mMessage.mstrSenderID, mSaveTmpInfo.mUserInf))
        {
            return;
        }
        save.mpClientSocket->sendMessage(mSaveTmpInfo);

        mSaveTmpInfo.mMessage.mstrSenderID = save.mMessage.mstrSenderID;
        mSaveTmpInfo.mMessage.mstrReceiverID = save.mMessage.mstrReceiverID;
        if (EMT_GET_FRIEND_SUCCESS != mDatabaseCtrl.searchFriendInformationByID(
                 mSaveTmpInfo.mMessage.mstrSenderID, mSaveTmpInfo.mUserInf))
        {
            mSaveTmpInfo.mMessage.mnType = EMT_ADD_FRIEND_FAIL;
            return;
        }
        qDebug() << "agree add friend";


        break;
    }
    default:
        return;
    }
//    mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.addFriend(mSaveTmpInfo.mMessage);
//    save.mpClientSocket->sendMessage(mSaveTmpInfo);

    QMap<QString, TClientSocketCtrl*>::iterator iter;
    iter = mmapOnlineUser.find(mSaveTmpInfo.mMessage.mstrReceiverID);
    if(mmapOnlineUser.end() == iter)
    {
        // 存储离线消息
        //mDatabaseCtrl.messageRequest(save.message);
    }
    else
        iter.value()->sendMessage(mSaveTmpInfo);
}


/*************************************************
Function Name： flockRequest()
Description:  处理群请求
*************************************************/
void TTcpServer::flockRequest(const TSaveTmpInformation & save)
{
    mSaveTmpInfo.mnReplyKind = EMT_ADD_FLOCK;
    mSaveTmpInfo.mMessage = save.mMessage;
    switch (save.mMessage.mnType)
    {
    case EMT_REQUEST_FLOCK:
    {
        // 查询到群主的ID
        if (!mDatabaseCtrl.searchFlockHost(mSaveTmpInfo.mMessage.mstrReceiverID,
                                        mSaveTmpInfo.mstrPeerID))
        {
            return;
        }
        qDebug() << "request user add flock";
        break;
    }
    case EMT_FLOCK_REQUEST:
    {
        mSaveTmpInfo.mstrPeerID = mSaveTmpInfo.mMessage.mstrReceiverID;
        break;
    }
    case EMT_FLOCK_REFUSE:
        mSaveTmpInfo.mstrPeerID = mSaveTmpInfo.mMessage.mstrReceiverID;
        qDebug() << "refuse user add flock";
        break;
    case EMT_FLOCK_AGREE:
    {
        if (!mDatabaseCtrl.addFlockMember(mSaveTmpInfo.mMessage.mstrSenderID,
                                       mSaveTmpInfo.mMessage.mstrReceiverID))
        {
            mSaveTmpInfo.mMessage.mnType = EMT_ADD_FLOCK_FAIL;
            break;
        }
//-----------------------------------------
        mSaveTmpInfo.mMessage.mstrSenderID = save.mMessage.mstrReceiverID; // 新成员
        mSaveTmpInfo.mMessage.mstrReceiverID = save.mMessage.mstrSenderID; // 群号

        // 获取新成员信息，发送给群主
        if (!mDatabaseCtrl.searchFlockMember(mSaveTmpInfo.mMessage.mstrReceiverID,
                                          mSaveTmpInfo.mMessage.mstrSenderID,
                                          mSaveTmpInfo.mFlockMember))
        {
            return;
        }

        // 获取群信息，发送给请求入群者 以及 群主
        if (!mDatabaseCtrl.searchFlockInformation(
                    mSaveTmpInfo.mMessage.mstrReceiverID, mSaveTmpInfo.mFlockInf))
        {
            mSaveTmpInfo.mMessage.mnType = EMT_ADD_FLOCK_FAIL;
            return;
        }

        mSaveTmpInfo.mMessage.mnType = EMT_AGREE_FLOCK;
        save.mpClientSocket->sendMessage(mSaveTmpInfo);

        tellFlockMemberHaveNewMember(mSaveTmpInfo.mFlockInf.mstrFlockID);

//-----------------------------------------
        mSaveTmpInfo.mMessage.mnType = EMT_FLOCK_AGREE;
        mSaveTmpInfo.mMessage.mstrSenderID = save.mMessage.mstrSenderID;     // 群号
        mSaveTmpInfo.mMessage.mstrReceiverID = save.mMessage.mstrReceiverID; // 新成员
        mSaveTmpInfo.mstrPeerID = mSaveTmpInfo.mMessage.mstrReceiverID;
        qDebug() << "agree user add flock";

        break;
    }
    case EMT_AGREE_FLOCK:
    {
        // 通过群ID(receiverID)查询到群主的ID(peerID)
        if (!mDatabaseCtrl.searchFlockHost(mSaveTmpInfo.mMessage.mstrReceiverID,
                                        mSaveTmpInfo.mstrPeerID))
        {
            return;
        }

        // 在数据库中添加群成员信息
        if (!mDatabaseCtrl.addFlockMember(mSaveTmpInfo.mMessage.mstrReceiverID,
                                       mSaveTmpInfo.mMessage.mstrSenderID))
        {
            mSaveTmpInfo.mMessage.mnType = EMT_ADD_FLOCK_FAIL;
            return;
        }

        // 获取新成员信息，发送给群主
        if (!mDatabaseCtrl.searchFlockMember(mSaveTmpInfo.mMessage.mstrReceiverID,
                                          mSaveTmpInfo.mMessage.mstrSenderID,
                                          mSaveTmpInfo.mFlockMember))
        {
            return;
        }
        // 获取群信息，发送给入群者 以及 群主
        if (!mDatabaseCtrl.searchFlockInformation(
                    mSaveTmpInfo.mMessage.mstrReceiverID, mSaveTmpInfo.mFlockInf))
        {
            mSaveTmpInfo.mMessage.mnType = EMT_ADD_FLOCK_FAIL;
            return;
        }

        mSaveTmpInfo.mMessage.mnType = EMT_FLOCK_AGREE;
        mSaveTmpInfo.mMessage.mstrSenderID = save.mMessage.mstrReceiverID;     // 群号
        mSaveTmpInfo.mMessage.mstrReceiverID = save.mMessage.mstrSenderID; // 新成员
        save.mpClientSocket->sendMessage(mSaveTmpInfo);

        tellFlockMemberHaveNewMember(mSaveTmpInfo.mFlockInf.mstrFlockID);

        mSaveTmpInfo.mMessage.mnType = EMT_AGREE_FLOCK;
        mSaveTmpInfo.mMessage.mstrSenderID = save.mMessage.mstrSenderID;     // 新成员
        mSaveTmpInfo.mMessage.mstrReceiverID = save.mMessage.mstrReceiverID; // 群号

        break;
    }
    case EMT_REFUSE_FLOCK:
    {
        break;
    }
    default:
        return;
    }

    QMap<QString, TClientSocketCtrl*>::iterator iter;
    iter = mmapOnlineUser.find(mSaveTmpInfo.mstrPeerID);
    if(mmapOnlineUser.end() == iter)
    {
        // 存储离线消息
        //mDatabaseCtrl.messageRequest(save.message);
    }
    else
        iter.value()->sendMessage(mSaveTmpInfo);
}


/*************************************************
Function Name： flockRequest()
Description:  处理讨论组请求
*************************************************/
void TTcpServer::discussionRequest(const TSaveTmpInformation & save)
{
    mSaveTmpInfo.mnReplyKind = EMT_ADD_DISCUSSION;
    mSaveTmpInfo.mMessage = save.mMessage;

    if (EMT_ADD_DISCUSSION != mSaveTmpInfo.mMessage.mnType)
        return;

    // 在数据库中添加讨论组成员信息
    if (!mDatabaseCtrl.addDiscussionMember(mSaveTmpInfo.mMessage.mstrSenderID,
                                   mSaveTmpInfo.mMessage.mstrReceiverID))
    {
        mSaveTmpInfo.mMessage.mnType = EMT_ADD_DISCUSSION_FAIL;
        save.mpClientSocket->sendMessage(mSaveTmpInfo);
        return;
    }

    // 获取讨论组信息，发送给新加入的成员
    if (mDatabaseCtrl.searchDiscussionInformation(
                mSaveTmpInfo.mMessage.mstrSenderID, mSaveTmpInfo.mDiscussionInf))
    {
        mSaveTmpInfo.mMessage.mnType = EMT_ADD_DISCUSSION_SUCCESS;
        QMap<QString, TClientSocketCtrl*>::iterator iter;
        iter = mmapOnlineUser.find(mSaveTmpInfo.mMessage.mstrReceiverID);
        if(mmapOnlineUser.end() != iter)
            iter.value()->sendMessage(mSaveTmpInfo);
    }


    // 获取新成员信息， 发送给之前的成员
    if (mDatabaseCtrl.searchDiscussionMember(mSaveTmpInfo.mMessage.mstrSenderID,
                                      mSaveTmpInfo.mMessage.mstrReceiverID,
                                      mSaveTmpInfo.mDiscussionMember))
    {
        int replyKind = mDatabaseCtrl.searchDiscussionMembers(mSaveTmpInfo.mMessage.mstrSenderID,
                                                           mSaveTmpInfo.mDiscussionMembersVec);
        if (EMT_GET_DISCUSSION_MEMBERS_SUCCESS == replyKind)
        {
//             mSaveTmpInfo.mnReplyKind
            mSaveTmpInfo.mMessage.mnType = EMT_NEW_DISCUSSION_MEMBER;
            QMap<QString, TClientSocketCtrl*>::iterator iter;
            for(int i=0; i<mSaveTmpInfo.mDiscussionMembersVec.size(); ++i)
            {
                if (mSaveTmpInfo.mDiscussionMembersVec[i].mstrUserID.
                        compare(mSaveTmpInfo.mMessage.mstrReceiverID) == 0)
                    continue;

                iter = mmapOnlineUser.find(mSaveTmpInfo.mDiscussionMembersVec[i].mstrUserID);
                if(mmapOnlineUser.end() != iter)
                    iter.value()->sendMessage(mSaveTmpInfo);
            }
        }
    }


}

/*************************************************
Function Name： talkRequest()
Description:  处理对话请求
*************************************************/
void TTcpServer::talkRequest(const TSaveTmpInformation & save)
{
    mSaveTmpInfo.mMessage = save.mMessage;
    switch (mSaveTmpInfo.mMessage.mnType)
    {
    case EMT_TALK_MESSAGE:// == mSaveTmpInfo.mMessage.m_type)
    {
        mSaveTmpInfo.mnReplyKind = EMT_TALK;
        QMap<QString, TClientSocketCtrl*>::iterator iter;
        iter = mmapOnlineUser.find(mSaveTmpInfo.mMessage.mstrReceiverID);

        // 聊天记录存储到数据库
        mDatabaseCtrl.addHistoryMessage(mSaveTmpInfo.mMessage);

        if(mmapOnlineUser.end() == iter)
        {
            // 存储离线消息到数据库
            //mDatabaseCtrl.messageRequest(mSaveTmpInfo.message);
            mDatabaseCtrl.addOfflineMessage(mSaveTmpInfo.mMessage);
        }
        else
            iter.value()->sendMessage(mSaveTmpInfo);
        break;
    }

    case EMT_REQUEST_FILE:
    case EMT_REFUSE_FILE:
    case EMT_REQUEST_VIDEO:
    {
        mSaveTmpInfo.mnReplyKind = EMT_TALK;
        QMap<QString, TClientSocketCtrl*>::iterator iter;
        iter = mmapOnlineUser.find(mSaveTmpInfo.mMessage.mstrReceiverID);
        if(mmapOnlineUser.end() != iter)
        {
            //mSaveTmpInfo.mMessage.m_receiverIP =  iter.value()->peerAddress();
            iter.value()->sendMessage(mSaveTmpInfo);
        }
        break;
    }

    case EMT_TALK_FLOCK:
    {
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchFlockMembers(mSaveTmpInfo.mMessage.mstrReceiverID,
                                                           mSaveTmpInfo.mFlockMembersVec);
        if (EMT_GET_FLOCK_MEMBERS_SUCCESS == mSaveTmpInfo.mnReplyKind)
        {
             mSaveTmpInfo.mnReplyKind = EMT_TALK;
            QMap<QString, TClientSocketCtrl*>::iterator iter;
            for(int i=0; i<mSaveTmpInfo.mFlockMembersVec.size(); ++i)
            {
                iter = mmapOnlineUser.find(mSaveTmpInfo.mFlockMembersVec[i].mstrUserID);
                if(mmapOnlineUser.end() == iter)
                {
                    // 存储离线消息
                    //mDatabaseCtrl.messageRequest(save.message);
                }
                else
                    iter.value()->sendMessage(mSaveTmpInfo);
            }
        }
        break;
    }

    case EMT_TALK_DISCUSSION:
    {
        mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchDiscussionMembers(mSaveTmpInfo.mMessage.mstrReceiverID,
                                                           mSaveTmpInfo.mDiscussionMembersVec);
        if (EMT_GET_DISCUSSION_MEMBERS_SUCCESS == mSaveTmpInfo.mnReplyKind)
        {
             mSaveTmpInfo.mnReplyKind = EMT_TALK;
            QMap<QString, TClientSocketCtrl*>::iterator iter;
            for(int i=0; i<mSaveTmpInfo.mDiscussionMembersVec.size(); ++i)
            {
                iter = mmapOnlineUser.find(mSaveTmpInfo.mDiscussionMembersVec[i].mstrUserID);
                if(mmapOnlineUser.end() == iter)
                {
                    // 存储离线消息
                    //mDatabaseCtrl.messageRequest(save.message);
                }
                else
                    iter.value()->sendMessage(mSaveTmpInfo);
            }
        }
        break;
    }

    default:
        break;
    }
}

/*************************************************
Function Name： changeStatu()
Description:  通知好友，改变用户状态
*************************************************/
void TTcpServer::tellFriendsStatusChanged(const QString &id, int status)
{
    mSaveTmpInfo.mnReplyKind = EMT_CHANGE_STATUE;
    mSaveTmpInfo.mnStatus = status;
    mSaveTmpInfo.mstrPeerID = id;
    mDatabaseCtrl.searchFriendsID(id, mSaveTmpInfo.mIDVec);

    QMap<QString, TClientSocketCtrl*>::iterator iter;
    for(int i=0; i<mSaveTmpInfo.mIDVec.size(); ++i)
    {
        iter = mmapOnlineUser.find(mSaveTmpInfo.mIDVec[i]);
        if(iter != mmapOnlineUser.end())
        {
            iter.value()->sendMessage(mSaveTmpInfo);
        }
    }
}

/*************************************************
Function Name： tellFriendsInformationChanged()
Description:  通知好友，改变用户信息
*************************************************/
void TTcpServer::tellFriendsInformationChanged(const QString & id)
//                                                const TFriendInformation &fri)
{
    mSaveTmpInfo.mnReplyKind = EMT_CHANGE_INFORMATION;
//    mSaveTmpInfo.mFriendInf = fri;
    if (!mDatabaseCtrl.searchFriendsID(id, mSaveTmpInfo.mIDVec))
        return;
    mSaveTmpInfo.mFriendInf.mnHeadPortrait = mSaveTmpInfo.mUserInf.mnHeadPortrait;
    mSaveTmpInfo.mFriendInf.mstrUserID = mSaveTmpInfo.mUserInf.mstrUserID;
    mSaveTmpInfo.mFriendInf.mstrNickName = mSaveTmpInfo.mUserInf.mstrNickName;
//    mSaveTmpInfo.mFriendInf.mstrGroupName =
//    mSaveTmpInfo.mFriendInf.m_remarkName =
    mSaveTmpInfo.mFriendInf.mnStatus = mSaveTmpInfo.mUserInf.mnStatus;

    QMap<QString, TClientSocketCtrl*>::iterator iter;
    for(int i=0; i<mSaveTmpInfo.mIDVec.size(); ++i)
    {
        iter = mmapOnlineUser.find(mSaveTmpInfo.mIDVec[i]);
        if(iter != mmapOnlineUser.end())
        {
            iter.value()->sendMessage(mSaveTmpInfo);
        }
    }
}


/*************************************************
Function Name： tellMembersFlockDroped()
Description:  通知群成员，群被解散
*************************************************/
void TTcpServer::tellMembersFlockDroped(const QString & flockID)
{
    mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchFlockMembers(flockID,
                                                       mSaveTmpInfo.mFlockMembersVec);
    if (EMT_GET_FLOCK_MEMBERS_SUCCESS == mSaveTmpInfo.mnReplyKind)
    {
         mSaveTmpInfo.mnReplyKind = EMT_DROP_FLOCK_SUCCESS;
         mSaveTmpInfo.mstrFlockID = flockID;
        QMap<QString, TClientSocketCtrl*>::iterator iter;
        for(int i=0; i<mSaveTmpInfo.mFlockMembersVec.size(); ++i)
        {
            iter = mmapOnlineUser.find(mSaveTmpInfo.mFlockMembersVec[i].mstrUserID);
//                if(iter != mmapOnlineUser.end())
//                {
//                    iter.value()->sendMessage(mSaveTmpInfo);
//                }
            if(mmapOnlineUser.end() == iter)
            {
                // 存储离线消息
                //mDatabaseCtrl.messageRequest(save.message);
            }
            else
                iter.value()->sendMessage(mSaveTmpInfo);
        }
    }


}



/*************************************************
Function Name： tellMembersDiscussionDroped()
Description:  通知讨论组成员， 讨论组被解散了
*************************************************/
void TTcpServer::tellMembersDiscussionDroped(const QString & discussionID)
{
    mSaveTmpInfo.mnReplyKind = mDatabaseCtrl.searchDiscussionMembers(discussionID,
                                                       mSaveTmpInfo.mDiscussionMembersVec);
    if (EMT_GET_DISCUSSION_MEMBERS_SUCCESS == mSaveTmpInfo.mnReplyKind)
    {
         mSaveTmpInfo.mnReplyKind = EMT_DROP_DISCUSSION_SUCCESS;
         mSaveTmpInfo.mstrDiscussionID = discussionID;
        QMap<QString, TClientSocketCtrl*>::iterator iter;
        for(int i=0; i<mSaveTmpInfo.mDiscussionMembersVec.size(); ++i)
        {
            iter = mmapOnlineUser.find(mSaveTmpInfo.mDiscussionMembersVec[i].mstrUserID);
//                if(iter != mmapOnlineUser.end())
//                {
//                    iter.value()->sendMessage(mSaveTmpInfo);
//                }
            if(mmapOnlineUser.end() == iter)
            {
                // 存储离线消息
                //mDatabaseCtrl.messageRequest(save.message);
            }
            else
                iter.value()->sendMessage(mSaveTmpInfo);
        }
    }
}

/*************************************************
Function Name： tellMembersFlockChanged()
Description:  通知群成员，群信息被改变
*************************************************/
void TTcpServer::tellMembersFlockChanged(const TFlockInformation & flock)
{
    int kind = mDatabaseCtrl.searchFlockMembers(flock.mstrFlockID,
                                             mSaveTmpInfo.mFlockMembersVec);
    if (EMT_GET_FLOCK_MEMBERS_SUCCESS == kind)
    {
        QMap<QString, TClientSocketCtrl*>::iterator iter;
        for(int i=0; i<mSaveTmpInfo.mFlockMembersVec.size(); ++i)
        {
            iter = mmapOnlineUser.find(mSaveTmpInfo.mFlockMembersVec[i].mstrUserID);
            if(iter != mmapOnlineUser.end())
            {
                iter.value()->sendMessage(mSaveTmpInfo);
            }
        }
    }
}



/*************************************************
Function Name： tellMembersDiscussionChanged()
Description:  通知讨论组成员，讨论组信息被修改了
*************************************************/
void TTcpServer::tellMembersDiscussionChanged(const TDiscussionInformation & discussion)
{
    int kind = mDatabaseCtrl.searchDiscussionMembers(discussion.mstrDiscussionID,
                                             mSaveTmpInfo.mDiscussionMembersVec);
    if (EMT_GET_DISCUSSION_MEMBERS_SUCCESS == kind)
    {
        QMap<QString, TClientSocketCtrl*>::iterator iter;
        for(int i=0; i<mSaveTmpInfo.mDiscussionMembersVec.size(); ++i)
        {
            iter = mmapOnlineUser.find(mSaveTmpInfo.mDiscussionMembersVec[i].mstrUserID);
            if(iter != mmapOnlineUser.end())
            {
                iter.value()->sendMessage(mSaveTmpInfo);
            }
        }
    }
}


/*************************************************
Function Name： tellFlockMemberHaveNewMember()
Description:  通知群成员， 有新的成员
*************************************************/
void TTcpServer::tellFlockMemberHaveNewMember(const QString & flockID)
{
    int replyKind = mDatabaseCtrl.searchFlockMembers(flockID,
                                                       mSaveTmpInfo.mFlockMembersVec);
    if (EMT_GET_FLOCK_MEMBERS_SUCCESS == replyKind)
    {
//         mSaveTmpInfo.mnReplyKind = ADD_FLOCK;
//         mSaveTmpInfo.mstrFlockID = flockID;
        mSaveTmpInfo.mMessage.mnType = EMT_NEW_FLOCK_MEMBER;

        QMap<QString, TClientSocketCtrl*>::iterator iter;
        for(int i=0; i<mSaveTmpInfo.mFlockMembersVec.size(); ++i)
        {
            if (mSaveTmpInfo.mFlockMembersVec[i].mstrUserID.compare(mSaveTmpInfo.mFlockMember.mstrUserID) == 0)
                continue;
            if (mSaveTmpInfo.mFlockMembersVec[i].mstrUserID.compare(mSaveTmpInfo.mFlockInf.mstrCreatorID) == 0)
                continue;

            iter = mmapOnlineUser.find(mSaveTmpInfo.mFlockMembersVec[i].mstrUserID);
            if(mmapOnlineUser.end() == iter)
            {
                // 存储离线消息
                //mDatabaseCtrl.messageRequest(save.message);
            }
            else
                iter.value()->sendMessage(mSaveTmpInfo);
        }
    }
}



/*************************************************
Function Name： tellFlockMemberHaveLeavedMember()
Description:  通知群成员， 有成员离开
*************************************************/
void TTcpServer::tellFlockMemberHaveLeavedMember(const QString & flockID)
{
    int replyKind = mDatabaseCtrl.searchFlockMembers(flockID,  mSaveTmpInfo.mFlockMembersVec);
    if (EMT_GET_FLOCK_MEMBERS_SUCCESS == replyKind)
    {
//         mSaveTmpInfo.mnReplyKind = ADD_FLOCK;
//         mSaveTmpInfo.mstrFlockID = flockID;
        mSaveTmpInfo.mnReplyKind = EMT_LEAVE_FLOCK_MEMBER;

        QMap<QString, TClientSocketCtrl*>::iterator iter;
        for(int i=0; i<mSaveTmpInfo.mFlockMembersVec.size(); ++i)
        {
//            if (mSaveTmpInfo.mFlockMembersVec[i].m_userID.compare(mSaveTmpInfo.mFlockMember.m_userID) == 0)
//                continue;
            if (mSaveTmpInfo.mFlockMembersVec[i].mstrUserID.compare(mSaveTmpInfo.mstrPeerID) == 0)
                continue;

            iter = mmapOnlineUser.find(mSaveTmpInfo.mFlockMembersVec[i].mstrUserID);
            if(mmapOnlineUser.end() != iter)
                iter.value()->sendMessage(mSaveTmpInfo);
        }
    }
}


/*************************************************
Function Name： tellDiscussionMemberHaveNewMember()
Description:  通知群成员， 有新的成员
*************************************************/
void TTcpServer::tellDiscussionMemberHaveNewMember(const QString & discussionID)
{
}


/*************************************************
Function Name： tellDiscussionMemberHaveLeavedMember()
Description:  通知讨论组成员， 有成员退出
*************************************************/
void TTcpServer::tellDiscussionMemberHaveLeavedMember(const QString & discussionID)
{
    int replyKind = mDatabaseCtrl.searchDiscussionMembers(mSaveTmpInfo.mMessage.mstrSenderID,
                                                       mSaveTmpInfo.mDiscussionMembersVec);
    if (EMT_GET_DISCUSSION_MEMBERS_SUCCESS == replyKind)
    {
        mSaveTmpInfo.mnReplyKind = EMT_LEAVE_DISCUSSION_MEMBER;
/*        mSaveTmpInfo.mMessage.m_type */
        QMap<QString, TClientSocketCtrl*>::iterator iter;
        for(int i=0; i<mSaveTmpInfo.mDiscussionMembersVec.size(); ++i)
        {
            iter = mmapOnlineUser.find(mSaveTmpInfo.mDiscussionMembersVec[i].mstrUserID);
            if(mmapOnlineUser.end() != iter)
                iter.value()->sendMessage(mSaveTmpInfo);
        }
    }
}

/*
    enum ConnectionType {
        AutoConnection,
        DirectConnection,
        QueuedConnection,
        AutoCompatConnection,
        BlockingQueuedConnection,
        UniqueConnection =  0x80
    };
    */

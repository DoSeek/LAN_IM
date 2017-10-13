/*********************************************
File Name： TClientSocketCtrl.cpp
Author： jet.F.R
Date： 2014.3.10
Description： 用于控制客户端socket的类
              与客户端连接的QTcpSocket类
Changes：
********************************************/

#include "TClientSocketCtrl.h"
#include <QMultiMap>
#include <QDateTime>


TClientSocketCtrl::TClientSocketCtrl(QObject *parent):
    QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteScoket()));
    mnBlockSize = 0;
    mSaveTmpInfo.mpClientSocket = this;
}

TClientSocketCtrl::~TClientSocketCtrl()
{
    if (mDatabaseCtrl.isOpen())
        mDatabaseCtrl.close();
}


/*************************************************
Function Name： receiveMessage()
Description: 接收用户发送的信息
*************************************************/
void TClientSocketCtrl::receiveMessage()

{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_8);
    if (mnBlockSize == 0)
    {
        if (bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> mnBlockSize;
    }

    if (bytesAvailable() < mnBlockSize)
        return;

    in >> mSaveTmpInfo.mnRequestKind;

    qDebug() << "receive message: " << mSaveTmpInfo.mnRequestKind;

    switch (mSaveTmpInfo.mnRequestKind)
    {
    case EMT_CHECK_CONNECTION:
    {
        mSaveTmpInfo.mnReplyKind = EMT_CHECK_CONNECTION;
        sendMessage(mSaveTmpInfo);
        mnBlockSize = 0;
        break;
    }
    case EMT_LOGIN:
    {
        in >> mSaveTmpInfo.mLoginInf;
        mSaveTmpInfo.mstrMyID = mSaveTmpInfo.mLoginInf.mstrUserID;
        mSaveTmpInfo.mstrSocketID = mSaveTmpInfo.mstrMyID;
        break;
    }
    case EMT_CHECK_MESSAGE:
    {
        in >> mSaveTmpInfo.mstrMyID;
        break;
    }
    case EMT_REGISTER:
    {
        in >> mSaveTmpInfo.mUserInf;
        break;
    }
    case EMT_GET_QUESTION_ANSWER:
    {
        in >> mSaveTmpInfo.mstrMyID;
        break;
    }
    case EMT_TALK:
    {
        in >> mSaveTmpInfo.mMessage;
        break;
    }
    case EMT_GET_ALL_FRIENDS:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mnStatus;
        mSaveTmpInfo.mstrSocketID = mSaveTmpInfo.mstrMyID;
        break;
    }
    case EMT_GET_ALL_FLOCKS:
    {
        in >> mSaveTmpInfo.mstrMyID;
        break;
    }
    case EMT_GET_ALL_DISCUSSIONS:
    {
        in >> mSaveTmpInfo.mstrMyID;
        break;
    }
    case EMT_GET_FLOCK_MEMBERS:
    {
        in >> mSaveTmpInfo.mstrFlockID;
        break;
    }
    case EMT_GET_DISCUSSIONS_MEMBERS:
    {
        in >> mSaveTmpInfo.mstrDiscussionID;
        break;
    }

    case EMT_GET_FRIEND_INFORMATION:
    {
        in >> mSaveTmpInfo.mstrPeerID;
        break;
    }
    case EMT_GET_MYSELF_INFORMATION:
    {
        in >> mSaveTmpInfo.mstrMyID;
        break;
    }
    case EMT_CHANGE_INFORMATION:
    {
        in >> mSaveTmpInfo.mUserInf;
        break;
    }
    case EMT_CHANGE_REMARK:
    {
        in >> mSaveTmpInfo.mTempStr;
        break;
    }
    case EMT_CHANGE_STATUE:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mnStatus;
        break;
    }
    case EMT_ADD_FRIEND:
    {
        in >> mSaveTmpInfo.mMessage;
        break;
    }
    case EMT_REFUSE_FRIEND:
    {
        //in >> mSaveTmpInfo.mstrPeerID;
        break;
    }
    case EMT_DELETE_FRIEND:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mstrPeerID;
        break;
    }

    case EMT_ADD_FLOCK:
    {
        in >> mSaveTmpInfo.mMessage;
        break;
    }
    case EMT_DROP_FLOCK:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mstrFlockID;
        break;
    }
    case EMT_LEAVE_FLOCK:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mstrFlockID;
        break;
    }
    case EMT_CHANGE_FLOCK:
    {
        in >> mSaveTmpInfo.mstrFlockID;
        break;
    }
    case EMT_CHANGE_FLOCK_REMARK:
    {
        in >> mSaveTmpInfo.mTempStr;
        break;
    }
    case EMT_CREATE_FLOCK:
    {
        in >> mSaveTmpInfo.mTempStr;
        break;
    }

    case EMT_ADD_DISCUSSION:
    {
        in >> mSaveTmpInfo.mMessage;
        break;
    }
    case EMT_DROP_DISCUSSION:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mstrDiscussionID;
        break;
    }
    case EMT_LEAVE_DISCUSSION:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mstrDiscussionID;
        break;
    }
    case EMT_CHANGE_DISCUSSION:
    {
        in >> mSaveTmpInfo.mDiscussionInf;
        break;
    }
    case EMT_CREATE_DISCUSSION:
    {
        in >> mSaveTmpInfo.mTempStr;
        break;
    }
    case EMT_GET_IN_MAILS:
    case EMT_GET_OUT_MAILS:
    {
        in >> mSaveTmpInfo.mstrMyID;
        mSaveTmpInfo.mstrSocketID = mSaveTmpInfo.mstrMyID;
        mSaveTmpInfo.mstrSocketID.append(MAIL_ADDRESS_FORMAT);
        break;
    }
    case EMT_DELETE_IN_MAIL:
    case EMT_DELETE_OUT_MAIL:
    case EMT_SET_MAIL_READ:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mnMailID;
        break;
    }
    case EMT_SEND_MAIL:
    {
        in >> mSaveTmpInfo.mMail;
        break;
    }

    case EMT_REMOVE_BOX:
    case EMT_CREATE_BOX:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mstrTitle;
        break;
    }
    case EMT_MOVE_FRIEND_BOX:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mstrPeerID
           >> mSaveTmpInfo.mstrTitle >> mSaveTmpInfo.mstrNewTitle;
        break;
    }
    case EMT_RENAME_BOX:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mstrTitle >> mSaveTmpInfo.mstrNewTitle;
        break;
    }
    case EMT_GET_HISTORY_MESSAGE:
    {
        in >> mSaveTmpInfo.mstrMyID >> mSaveTmpInfo.mstrPeerID >> mSaveTmpInfo.mDate;
        break;
    }
    case EMT_CHANGE_PASSWORD:
    {
        in >> mSaveTmpInfo.mTempStr;
        break;
    }

    default:
        break;
    }


    QByteArray data = this->readAll();
    qDebug() << "leaved in socket: " << data.size();

    mnBlockSize =0;
    emit sendSignal(mSaveTmpInfo);
}
/*************************************************
Function Name： sendMessage()
Description: 发送信息
*************************************************/
void TClientSocketCtrl::sendMessage(const TSaveTmpInformation &temp)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << quint16(0) << (int)temp.mnReplyKind;

    switch (temp.mnReplyKind)
    {
    case EMT_LOGIN_SUCCESS:
    {
        out << temp.mUserInf;
        break;
    }
    case EMT_REGISTER_SUCCESS:
    {
        out << temp.mUserInf.mstrUserID << temp.mUserInf.mnHeadPortrait;
        break;
    }
    case EMT_GET_QUESTION_ANSWER_SUCCESS:
    {
        out << temp.mTempStr;
        break;
    }
    case EMT_GET_ALL_FRIENDS_SUCCESS:
    {
        int len = temp.mFriendsVec.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.mFriendsVec[i];
        break;
    }
    case EMT_GET_ALL_FLOCKS_SUCCESS:
    {
        int len = temp.mFlocksVec.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.mFlocksVec[i];
        break;
    }
    case EMT_GET_ALL_DISCUSSIONS_SUCCESS:
    {
        int len = temp.mDiscussionsVec.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.mDiscussionsVec[i];
        break;
    }
    case EMT_GET_FLOCK_MEMBERS_SUCCESS:
    {
        int len = temp.mFlockMembersVec.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.mFlockMembersVec[i];
        break;
    }
    case EMT_GET_DISCUSSION_MEMBERS_SUCCESS:
    {
        int len = temp.mDiscussionMembersVec.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.mDiscussionMembersVec[i];
        break;
    }


    case EMT_HAVE_MESSAGE:
        break;
    case EMT_CHANGE_STATUE:
        out << temp.mstrPeerID << temp.mnStatus;
        break;
    case EMT_GET_MYSELF_SUCCESS:
    {
        out << temp.mUserInf;
        break;
    }
    case EMT_DELETE_FRIEND_SUCCESS:
    {
        out << temp.mstrMyID << temp.mstrPeerID << temp.mstrGroupName;
        break;
    }
    case EMT_CHANGE_INFORMATION_SUCCESS:
    {
        out << temp.mUserInf;
        break;
    }
    case EMT_CHANGE_INFORMATION:
    {
        out << temp.mFriendInf;
        break;
    }
    case EMT_ADD_FRIEND:
    {
        out << temp.mMessage << temp.mUserInf;
        break;
    }
    case EMT_GET_FRIEND_SUCCESS:
    {
        out << temp.mUserInf;
        break;
    }
    case EMT_TALK:
        out << temp.mMessage;
        break;
    case EMT_CHANGE_REMARK_SUCCESS:
    {
        out << temp.mTempStr;
        break;
    }
    case EMT_BE_DELETED:
    {
        out << temp.mstrMyID << temp.mstrPeerID << temp.mstrGroupNameF;
        break;
    }
    case EMT_REQUEST_FLOCK:
    {
        out << temp.mMessage;
        break;
    }
    case EMT_ADD_FLOCK:
    {
        out << temp.mMessage << temp.mFlockMember << temp.mFlockInf;
        break;
    }

    case EMT_DROP_FLOCK_SUCCESS:
    {
        out << temp.mstrFlockID;
        break;
    }
     case EMT_LEAVE_FLOCK_SUCCESS:
    {
        out << temp.mstrMyID << temp.mstrFlockID << temp.mstrPeerID;
        break;
    }
    case EMT_LEAVE_FLOCK_MEMBER:
    {
        out << temp.mstrMyID << temp.mstrFlockID << temp.mstrPeerID;
        break;
    }
    case EMT_CHANGE_FLOCK_SUCCESS:
    {
        out << temp.mFlockInf;
        break;
    }
    case EMT_CHANGE_FLOCK_REMARK_SUCCESS:
    {
        out << temp.mTempStr;
        break;
    }
    case EMT_CREATE_FLOCK_SUCCESS:
    {
        out << temp.mFlockInf;
        break;
    }



    case EMT_ADD_DISCUSSION:
    {
        out << temp.mMessage;
        if (EMT_ADD_DISCUSSION_SUCCESS == temp.mMessage.mnType)
            out << temp.mDiscussionInf;
        else if (EMT_NEW_DISCUSSION_MEMBER == temp.mMessage.mnType)
            out << temp.mDiscussionMember;
        break;
    }

    case EMT_DROP_DISCUSSION_SUCCESS:
    {
        out << temp.mstrDiscussionID;
        break;
    }
     case EMT_LEAVE_DISCUSSION_MEMBER:
    {
        out << temp.mstrMyID << temp.mstrDiscussionID;// << temp.mstrPeerID;
        break;
    }
    case EMT_CHANGE_DISCUSSION_SUCCESS:
    {
        out << temp.mDiscussionInf;
        break;
    }

    case EMT_CREATE_DISCUSSION_SUCCESS:
    {
        out << temp.mDiscussionInf;
        break;
    }
    /*****************mail*****************/
    case EMT_GET_IN_MAILS_SUCCESS:
    {
        int len = temp.mInMailsVec.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.mInMailsVec[i];
        break;
    }
    case EMT_GET_OUT_MAILS_SUCCESS:
    {
        int len = temp.mOutMailsVec.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.mOutMailsVec[i];
        break;
    }

    case EMT_DELETE_IN_MAIL_SUCCESS:
    case EMT_DELETE_OUT_MAIL_SUCCESS:
    {
        out << temp.mstrMyID << temp.mnMailID;
        break;
    }

    case EMT_SEND_MAIL_SUCCESS:
    case EMT_RECEIVE_MAIL:
    {
        out << temp.mMail;
        break;
    }

    case EMT_RENAME_BOX_SUCCESS:
    {
        out << temp.mstrMyID << temp.mstrTitle << temp.mstrNewTitle;
        break;
    }
    case EMT_MOVE_FRIEND_BOX_SUCCESS:
    {
        out << temp.mstrMyID << temp.mstrPeerID << temp.mstrTitle << temp.mstrNewTitle;
        break;
    }
    case EMT_GET_HISTORY_MESSAGE_SUCCESS:
    {
        out << temp.mstrMyID << temp.mstrPeerID << temp.mDate;
        int len = temp.mHistoryMessageVec.size();
        out << len;
        for (int i=0; i<len; ++i)
            out << temp.mHistoryMessageVec[i];
        break;
    }
    default:
        break;
    }

    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    write(block);
}

/*************************************************
Function Name： deleteScoket()
Description: 删除socket
*************************************************/
void TClientSocketCtrl::deleteScoket()
{
    if (!mSaveTmpInfo.mstrSocketID.isEmpty())
    {
        if ( EMT_LOGIN != mSaveTmpInfo.mnRequestKind)
        emit deleteSignal(mSaveTmpInfo.mstrSocketID);
    }
    deleteLater();
}

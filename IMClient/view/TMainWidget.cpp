/*********************************************
File Name： TMainWidget.cpp
Author： jet.F.R
Date： 2014.3.10
Description： 客户端主界面
Changes：
********************************************/

#include "TMainWidget.h"
#include "TFriendListWidget.h"
#include "TFlockListWidget.h"
#include "TDiscussionListWidget.h"
#include "TLatestMessageListWidget.h"
#include "TToolBox.h"
#include "TMainCtrl.h"
#include "TUser.h"
#include "TChatWidget.h"
#include "TFriendButton.h"
#include "TFlockButton.h"
#include "TDiscussionButton.h"
#include "TInformationWidget.h"
#include "TClickLabel.h"
#include "TFlockChatWidget.h"
#include "TFlockInformationWidget.h"
#include "TDiscussionChatWidget.h"
#include "TDiscussionInformationWidget.h"
#include "TMailWidget.h"
#include "TMessageManageWidget.h"
#include "TLinkButton.h"
#include "TLinkFriendButton.h"
#include "TLinkFlockButton.h"
#include "TLinkDiscussionButton.h"
#include "TSearchListWidget.h"
#include "TEncryption.h"



static const QString TMP_LOCAL_HISTORY_PARH = "/tmp/local_history_message";


//拷贝文件：
bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

TMainWidget::TMainWidget(const TUserInformation me, QWidget *parent) :
    QWidget(parent), m_database(me.m_userID)
{
    m_flag = 0;
    m_timerId = 0;
    m_myself = me;
    m_onlineCounter = 0;
    m_mailWidget = nullptr;
    m_messageManWidget = nullptr;
    initIMMainWidget();
    linkSignalWithSlot();
    creatMenu();
    setWindowTitle(tr("MPR IM"));
}

TMainWidget::~TMainWidget()
{
    // 关闭定时器
    if (m_timerId != 0)
    {
        killTimer(m_timerId);
        m_timerId = 0;
    }

    if (m_database.isOpen())
    {
        m_database.close();
    }
    if (m_mailWidget != nullptr)
    {
        m_mailWidget->close();
        delete m_mailWidget;
        m_mailWidget = 0;
    }
    if (m_messageManWidget != nullptr)
    {
        m_messageManWidget->close();
        delete m_messageManWidget;
        m_messageManWidget = 0;
    }
    // 清空容器
    clearContainers();
    qDeleteAll(m_chatRoomMap.begin(), m_chatRoomMap.end());
    m_chatRoomMap.clear();

    qDeleteAll(m_flockRoomMap.begin(), m_flockRoomMap.end());
    m_flockRoomMap.clear();

    qDeleteAll(m_discussionRoomMap.begin(), m_discussionRoomMap.end());
    m_discussionRoomMap.clear();

    qDeleteAll(m_informationMap.begin(), m_informationMap.end());
    m_informationMap.clear();

    qDeleteAll(m_flockInformationMap.begin(), m_flockInformationMap.end());
    m_flockInformationMap.clear();

    qDeleteAll(m_discussionInformationMap.begin(), m_discussionInformationMap.end());
    m_discussionInformationMap.clear();
}

/*************************************************
Function Name： getLocalMyInformation()
Description: 获取本地个人信息
*************************************************/
TUserInformation TMainWidget::getLocalMyInformation() const
{
    return m_myself;
}

/*************************************************
Function Name： getLocalMyInformation()
Description: 获取本地个人信息
*************************************************/
bool TMainWidget::isMyFriend(const QString & id) const
{
    if (m_friendMap.contains(id))
        return true;
    return false;
}


/*************************************************
Function Name： setNickname()
Description: 设置昵称
Input： const QString &nickname: 昵称
Output： nullptr
Changes： nullptr
*************************************************/
//void TMainWidget::setNickname(const QString &nickname)
//{
//    m_myself->setNickname(nickname);
//}

/*************************************************
Function Name： setUserID()
Description: 设置id
*************************************************/
//void TMainWidget::setUserID(const QString &id)
//{
//    m_myself->setUserID(id);

//    m_mainCtrl->getFriendsInfo(m_myself->getUserID());
//}

/*************************************************
Function Name： setLoginStatus()
Description: 设置登录状态
Input： const QString &nickname: 昵称
Output： nullptr
Changes： nullptr
*************************************************/
//void TMainWidget::setLoginStatus(const ELoginStatus status)
//{
//    m_myself->setStatus(status);
//}



/*************************************************
Function Name： insertChatRoomMap()
Description: 插入聊天室
*************************************************/
void TMainWidget::insertChatRoomMap(const QString &id, TChatWidget *chatRoom)
{
    m_chatRoomMap.insert(id, chatRoom);
}

/*************************************************
Function Name： insertFlockRoomMap()
Description: 插入聊天室
*************************************************/
void TMainWidget::insertFlockRoomMap(const QString & id,
                                      TFlockChatWidget *chatRoom)
{
    m_flockRoomMap.insert(id, chatRoom);
}


/*************************************************
Function Name： insertDiscussionRoomMap()
Description: 插入聊天室
*************************************************/
void TMainWidget::insertDiscussionRoomMap(const QString & id,
                                           TDiscussionChatWidget *chatRoom)
{
    m_discussionRoomMap.insert(id, chatRoom);
}


/*************************************************
Function Name： getFriendInformation
Description: 获取好友信息（发送至服务器）
*************************************************/
void TMainWidget::getFriendInformation(const QString &id)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->getFriendInformation(id);
}

/*************************************************
Function Name： changeMyInformation
Description: 改变自己信息（发送至服务器）
*************************************************/
void TMainWidget::changeMyInformation(const TUserInformation &me)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->changeMyInformation(me);
}


/*************************************************
Function Name： changeFriendRemark
Description: 改变好友备注（发送至服务器）
*************************************************/
void TMainWidget::changeFriendRemark(const QString & friendID,
                                      const QString & remark)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->changeFriendRemark(m_myself.m_userID, friendID, remark);
}

/*************************************************
Function Name： deleteFriend
Description: 删除好友（发送至服务器
*************************************************/
void TMainWidget::deleteFriend(const QString & friendID)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->deleteFriend(m_myself.m_userID, friendID);
}

/*************************************************
Function Name： removeInformationWidget
Description: 删除存储于m_informationMap中的信息窗口
*************************************************/
void TMainWidget::removeInformationWidget(const QString & userID)
{
    // mark
    m_informationMap[userID]->deleteLater();
    m_informationMap.remove(userID);
}

/*************************************************
Function Name： removeFlockInformationWidget
Description: 删除存储与m_flockInformationMap中的信息窗口
*************************************************/
void TMainWidget::removeFlockInformationWidget(const QString & flockID)
{
    // mark
    m_flockInformationMap[flockID]->deleteLater();
    m_flockInformationMap.remove(flockID);
}

/*************************************************
Function Name： removeDiscussionInformationWidget
Description: 删除存储与m_discussionInformationMap中的信息窗口
*************************************************/
void TMainWidget::removeDiscussionInformationWidget(const QString & discussionID)
{
    // mark
    m_discussionInformationMap[discussionID]->deleteLater();
    m_discussionInformationMap.remove(discussionID);
}

/*************************************************
Function Name： changeFlockInformation
Description: 修改群信息（发送至服务器）
*************************************************/
void TMainWidget::changeFlockInformation(const TFlockInformation & flock)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->changeFlockInformation(flock);
}


/*************************************************
Function Name： changeDiscussionInformation
Description: 修改讨论组信息 （发送至服务器
*************************************************/
void TMainWidget::changeDiscussionInformation(const TDiscussionInformation & discussion)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->changeDiscussionInformation(discussion);
}

/*************************************************
Function Name： removeChatWidget
Description: 删除存储于m_chatRoomMap中的聊天窗口
*************************************************/
void TMainWidget::removeChatWidget(const QString &friendID)
{
    // mark
    m_chatRoomMap[friendID]->deleteLater();
    m_chatRoomMap.remove(friendID);
}

/*************************************************
Function Name： removeFlockChatWidget
Description: 删除存储于m_flockRoomMap中的群聊窗口
*************************************************/
void TMainWidget::removeFlockChatWidget(const QString & flockID)
{
    // mark
    qDebug() << "size-font:" << m_flockRoomMap.size();
    m_flockRoomMap[flockID]->deleteLater();
    m_flockRoomMap.remove(flockID);
    qDebug() << "size-behind:" << m_flockRoomMap.size();
}
/*************************************************
Function Name： removeDiscussionChatWidget
Description: 删除存储于m_discussionRoomMap中的讨论组窗口
*************************************************/
void TMainWidget::removeDiscussionChatWidget(const QString & discussionID)
{
    // mark
    m_discussionRoomMap[discussionID]->deleteLater();
    m_discussionRoomMap.remove(discussionID);
}

/*************************************************
Function Name： dropFlock
Description: 解散群(发送至服务器)
*************************************************/
void TMainWidget::dropFlock(const QString & flockID)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->dropFlock(m_myself.m_userID, flockID);
}

/*************************************************
Function Name： dropDiscussion
Description: 解散讨论组（发送至服务器）
*************************************************/
void TMainWidget::dropDiscussion(const QString & discussionID)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->dropDiscussion(m_myself.m_userID, discussionID);
}

/*************************************************
Function Name： leaveFlock
Description: 退出群（发送至服务器）
*************************************************/
void TMainWidget::leaveFlock(const QString &memberID, const QString & flockID)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->leaveFlock(memberID, flockID);
}

/*************************************************
Function Name： leaveFlock
Description: 退出讨论组（发送至服务器）
*************************************************/
void TMainWidget::leaveDiscussion(const QString & memberID, const QString & discussionID)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->leaveDiscussion(memberID, discussionID);
}
/*************************************************
Function Name： leaveFlock
Description: 添加好友（发送至服务器）
*************************************************/
void TMainWidget::addFriend(const QString friendID)
{
    if (0 == friendID.compare(m_myself.m_userID))
    {
        QMessageBox::critical(nullptr, tr("添加失败"), tr("不能添加自己"));
        return;
    }
    if (m_friendMap.contains(friendID))
    {
        QMessageBox::critical(nullptr, tr("添加失败"), tr("该用户已经是您的好友"));
        return;
    }

    bool isOkMes = false;
    QString checkMes = QInputDialog::getText(nullptr, "添加好友",
                                           "请输入验证信息",
                                           QLineEdit::Normal,
                                           QString(tr("你好,我是: %1")).
                                             arg(m_myself.m_nickname),
                                           &isOkMes);
    if (!isOkMes)
        return;

//    TTempStrings temp;
//    temp.m_one = m_myself.m_userID;
//    temp.m_two = friendID;
//    temp.m_three = checkMes;

    TTalkMessage mes;
    mes.m_senderID = m_myself.m_userID;
    mes.m_receiverID = friendID;
    mes.m_text = checkMes;
    mes.m_type = REQUEST_FRIEND;

    if (nullptr != m_mainCtrl)
        m_mainCtrl->addFriend(mes);
}


/*************************************************
Function Name： changeFlockMemberRemark
Description: 修改群成员名片（发送至服务器）
*************************************************/
void TMainWidget::changeFlockMemberRemark(const TTempStrings & tmpStr)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->changeFlockMemberRemark(tmpStr);
}

void TMainWidget::moveFriendToBox(const QString & friendID,
                                   const QString & groupName,
                                   const QString & title)
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->moveFriendToBox(friendID, groupName, title);
}

/*************************************************
Function Name： setOnlineCounter
Description: 设置在线人数
*************************************************/
void TMainWidget::setOnlineCounter(int num)
{
    m_onlineCounter = num;
    m_labelOnlineNum->setText(QString(tr("在线人数: %1"))
                              .arg(QString::number(num)));

}

// 获取所有分组
QStringList TMainWidget::getGroupFriendList(const QString & groupName)
{
    m_friendsMarkList.clear();
    m_friendsIDList.clear();
    if (!m_indexFriendsGroupMap.contains(groupName))
    {
        return m_friendsMarkList;
    }

    int groupIndex = m_indexFriendsGroupMap[groupName];
    TToolItem * group = m_toolItemsFriendsVec[groupIndex];
    const QList<QWidget *>* friendList = group->getWidgetList();

    QString mark;
    TFriendInformation friendInfor;

    for (int i=0; i<friendList->size(); i++)
    {
        TFriendButton *friendBtn = (TFriendButton *)friendList->value(i);
        friendInfor = friendBtn->getInformation();

        if(friendInfor.m_remarkName.isEmpty())
            mark = friendInfor.m_nickname;
        else
            mark = friendInfor.m_remarkName;
        m_friendsMarkList.append(mark);
        m_friendsIDList.append(friendInfor.m_userID);
    }
    return m_friendsMarkList;
}


/*************************************************
Function Name： getLocalHistoryMessage()
Description: 获取本地某日的消息记录
*************************************************/
void TMainWidget::getLocalHistoryMessage(const int index, const QDate & date)
{
    m_localHistoryMessageVec.clear();
    if (index < 0 || index >= m_friendsIDList.size())
        return;
    bool isOk = m_database.searchHistoryMessage(m_myself.m_userID,
                                                m_friendsIDList[index],
                                                date,
                                                m_localHistoryMessageVec);
    qDebug() << "m_localHistoryMessageVec:" << isOk << m_localHistoryMessageVec.size();
    m_messageManWidget->setLocalMessageWidget(m_localHistoryMessageVec);
}


/*************************************************
Function Name： exportLocalMessage()
Description: 导出消息记录
*************************************************/
void TMainWidget::exportLocalMessage(const int index)
{
    QString csv = tr("CSV文件 (*.csv)");
    QString excel = tr("EXCEL文件 (*.xls)");
    QString txt = tr("TXT文件 (*.txt)");
    QString history = "resource/history";
    qDebug() << "export: id: " << index << m_friendsIDList.size();
    QString id = m_friendsIDList[index];
    QString selFilter="All files (*.*)";
    QDir dir = QDir::current();
    if (!dir.exists(history))
        dir.mkpath(history);
    QString path = QString("%1/%2_%3").arg(history, m_myself.m_userID, id);
    QString filter = QString(tr("%1;;%2;;%3").arg(csv, excel, txt));
    QString name = QFileDialog::getSaveFileName(this,tr("保存文件"), path,
                                                filter, &selFilter);
    if(name.isEmpty())
    {
        return;
    }
    qDebug() << "export-path" << name << selFilter;
    if (selFilter == csv)
    {
        if (!name.endsWith(".csv", Qt::CaseInsensitive))
            name.append(".csv");
    }
    else if (selFilter == excel)
    {
        if (!name.endsWith(".xls", Qt::CaseInsensitive))
            name.append(".xls");
    }
    else if (selFilter == txt)
    {
        if (!name.endsWith(".txt", Qt::CaseInsensitive))
            name.append(".txt");
    }

    QString tmpPath = TMP_LOCAL_HISTORY_PARH;
    QString newPath = tmpPath;
    static int i = 0;
    while(1)
    {

        QFile file(newPath.append(".csv"));
        if (file.exists())
        {
            newPath = tmpPath.append("_").append(QString::number(++i));
            tmpPath = TMP_LOCAL_HISTORY_PARH;
        }
        else
            break;
    }

    qDebug() << "newpath:" << newPath;

    int type =  m_database.exportHistoryMessageOneFriend(m_myself.m_userID,
                                                        id, newPath);
    QString temp;
    if (type < 0)
    {
        temp = QString(tr("聊天记录导出失败！"));

    }
    else if (type == 0)
    {
        temp = QString(tr("聊天记录为空！"));
    }
    else
    {
        temp = QString(tr("聊天记录导出成功:%1.")).arg(name);
        copyFileToPath(newPath, name, true);
        emit exportHistoryMessageOneFriendSuccess(m_myself.m_userID, id, newPath);
    }

    QMessageBox::information(nullptr, tr("导出聊天记录"), temp);
    m_messageManWidget->raise();
//    m_messageManWidget->activateWindow();
//    m_messageManWidget->show();
}


/*************************************************
Function Name： exportLocalMessageAll()
Description: 导出所有消息记录
*************************************************/
void TMainWidget::exportLocalMessageAll()
{
    // 代码有点冗余
    QString csv = tr("CSV文件 (*.csv)");
    QString excel = tr("EXCEL文件 (*.xls)");
    QString txt = tr("TXT文件 (*.txt)");
    QString history = "resource/history";
    QString selFilter="All files (*.*)";
    QDir dir = QDir::current();
    if (!dir.exists(history))
        dir.mkpath(history);
    QString path = QString("%1/%2").arg(history, m_myself.m_userID);
    QString filter = QString(tr("%1;;%2;;%3").arg(csv, excel, txt));
    QString name = QFileDialog::getSaveFileName(this,tr("保存文件"), path,
                                                filter, &selFilter);
    if(name.isEmpty())
    {
        return;
    }
    qDebug() << "export-path" << name << selFilter;
    if (selFilter == csv)
    {
        if (!name.endsWith(".csv", Qt::CaseInsensitive))
            name.append(".csv");
    }
    else if (selFilter == excel)
    {
        if (!name.endsWith(".xls", Qt::CaseInsensitive))
            name.append(".xls");
    }
    else if (selFilter == txt)
    {
        if (!name.endsWith(".txt", Qt::CaseInsensitive))
            name.append(".txt");
    }

//    TMP_LOCAL_HISTORY_PARH.append(QTime::currentTime().toString("hh-mm-ss"));
    QString tmpPath = TMP_LOCAL_HISTORY_PARH;
    QString newPath = tmpPath;
    static int i = 0;
    while(1)
    {

        QFile file(newPath.append(".csv"));
        if (file.exists())
        {
            newPath = tmpPath.append("_").append(QString::number(++i));
            tmpPath = TMP_LOCAL_HISTORY_PARH;
        }
        else
            break;
    }

    qDebug() << "newpath:" << newPath;

    int type =  m_database.exportHistoryMessageAll(m_myself.m_userID, newPath);

    QString temp;
    if (type < 0)
    {
        temp = QString(tr("聊天记录导出失败！"));

    }
    else if (type == 0)
    {
        temp = QString(tr("聊天记录为空！"));
    }
    else
    {
        temp = QString(tr("聊天记录导出成功:%1.")).arg(name);
        copyFileToPath(newPath, name, true);
         emit exportHistoryMessageAllSuccess(m_myself.m_userID, newPath);
    }

    QMessageBox::information(nullptr, tr("导出聊天记录"), temp);
    m_messageManWidget->raise();
}

/*************************************************
Function Name： deleteLocalMessage()
Description: 删除单个好友消息记录
*************************************************/
void TMainWidget::deleteLocalMessage(const int index)
{
    QString id = m_friendsIDList[index];
    int type =  m_database.deleteHistoryMessageOneFriend(m_myself.m_userID, id);
    QString temp;
    if (type < 0)
    {
        temp = QString(tr("删除聊天记录失败！"));
    }
    else if (type == 0)
    {
        temp = QString(tr("聊天记录为空！"));
    }
    else
    {
        temp = QString(tr("删除聊天记录成功！"));
    }
    QMessageBox::information(nullptr, tr("删除聊天记录"), temp);
    m_messageManWidget->raise();
    return;
}

/*************************************************
Function Name： deleteLocalMessageAll()
Description: 删除所有消息记录
*************************************************/
void TMainWidget::deleteLocalMessageAll()
{
    // 代码有点冗余
    int type =  m_database.deleteHistoryMessageAll(m_myself.m_userID);
    QString temp;
    if (type < 0)
    {
        temp = QString(tr("删除聊天记录失败！"));
    }
    else if (type == 0)
    {
        temp = QString(tr("聊天记录为空！"));
    }
    else
    {
        temp = QString(tr("删除聊天记录成功！"));
    }
    QMessageBox::information(nullptr, tr("删除聊天记录"), temp);
    m_messageManWidget->raise();
    return;
}



#if 0
//void TMainWidget::exportLocalMessage(const int &index)
//{
////    QString history = "resource/history";
//    QString history = "/tmp";
//    QString id = m_friendsIDList[index];
////    QString selFilter="All files (*.*)";
////    QDir dir = QDir::current();
////    if (!dir.exists(history))
////        dir.mkpath(history);
//    QString fileName = QString("%1/%2_%3").arg(history, m_myself.m_userID, id);
////    QString filter = QString(
////                tr("CSV文件 (*.xls);;EXCEL文件 (*.xls);; TXT文件 (*.txt)"));
////    QString name = QFileDialog::getSaveFileName(this,tr("保存文件"), path,
////                                                filter, &selFilter);
////    if(name.isEmpty())
////    {
////        return;
////    }
////    qDebug() << "export-path" << name << selFilter;

////    QFile file(dir.filePath(TMP_LOCAL_HISTORY_PARH));
////    QString tmpPath = TMP_LOCAL_HISTORY_PARH;
//    QString tmpPath = fileName;
//    QString newPath = tmpPath;
//    static int i = 0;
//    while(1)
//    {

//        QFile file(newPath.append(".csv"));
//        if (file.exists())
//        {
//            newPath = tmpPath.append("_").append(QString::number(++i));
//            tmpPath = fileName;
//        }
//        else
//            break;
//    }

//    qDebug() << "newpath:" << newPath;

//    int type =  m_database.exportHistoryMessageOneFriend(m_myself.m_userID,
//                                                        id, newPath);
//    if (type < 0)
//    {
//        QString temp = QString(tr("聊天记录导出失败！"));
//        QMessageBox::information(nullptr, tr("聊天记录"), temp);
//        return;
//    }
////    qDebug() << "type: "<< type;
////    if (type >=0)
////    {
////        QFile file(newPath);
////        file.open( QIODevice::WriteOnly );
////        file.copy("/tmp/nimei.xls");
////        file.close();
////    }
//    QString temp = QString(tr("聊天记录导出成功:%1。\n请选择保存路径。")).arg(newPath);
//    QMessageBox::information(nullptr, tr("聊天记录"), temp);

//    emit exportHistoryMessageOneFriendSuccess(m_myself.m_userID, id, newPath);

////    QFile file("/tmp/2014050703_2014050701_3.csv");
////    file.open( QIODevice::WriteOnly | QIODevice::Text );
////    file.copy("/tmp/nimei.csv");
////    file.close();

////    copyFileToPath(newPath, "/tmp/nini.csv", true);
//    QFile::copy(newPath, "/tmp/nini.csv");

//}
#endif

/*************************************************
Function Name： getNetWorkHistoryMessage()
Description: 获取网络某日某好友的消息记录
*************************************************/
void TMainWidget::getNetWorkHistoryMessage(const int index, const QDate &date)
{
//    m_localHistoryMessageVec.clear();
    if (index < 0 || index >= m_friendsIDList.size())
        return;
    if (nullptr != m_mainCtrl)
        m_mainCtrl->requestGetNetWorkHistoryMessageOneFriendOneDate(
                     m_friendsIDList[index], date);
}

// mark: public slots:-------------------------------------------

/*************************************************
Function Name： getFriendsInformation()
Description: 获取好友信息列表（发送至服务器）
*************************************************/
void TMainWidget::getFriendsInformation()
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->getFriendsInformation(m_myself.m_userID, m_myself.m_status);
}

/*************************************************
Function Name： getFlocksInformation()
Description: 获取群信息列表（发送至服务器）
*************************************************/
void TMainWidget::getFlocksInformation()
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->getFlocksInformation(m_myself.m_userID);
}

/*************************************************
Function Name： getDiscussionsInformation()
Description: 获取讨论组信息列表（发送至服务器）
*************************************************/
void TMainWidget::getDiscussionsInformation()
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->getDiscussionsInformation(m_myself.m_userID);
}

/*************************************************
Function Name： getLatestMessage()
Description: 获取本地最近消息（访问本地数据库）
*************************************************/
void TMainWidget::getLatestMessage()
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->getLatestMessage(m_myself.m_userID);
}


/*************************************************
Function Name： setNetworkMessageWidget()
Description: 设置消息界面
*************************************************/
void TMainWidget::setNetworkMessageWidget(const QString & friendID, const QDate & date,
                             const QVector<TTalkMessage> & mesVec)
{
//    if (friendID != m_friendsIDList[index])
//        return;
    m_messageManWidget->setNetworkMessageWidget(mesVec);
}


/*************************************************
Function Name： setFriendListWidget
Description: 设置好友列表
*************************************************/
void TMainWidget::setFriendListWidget(const QVector<TFriendInformation> &friendsVec)
{
//    return;
//    #if 0
//    clearContainers();

//    if (!m_friendsGroupList.contains(GROUP_NAME_DEFAULT))
//    {
//        // 存储 新的分组号
//        m_friendsGroupList.append(GROUP_NAME_DEFAULT);

//        // 新建并存储分组
//        TToolBox *toolBox = new TToolBox;
//        TToolItem *toolItem = new TToolItem(GROUP_NAME_DEFAULT);
//        m_toolItemsFriendsVec.push_back(toolItem);
//        toolBox->addItem(toolItem);
//        m_friendListWidget->addItem(toolBox);

//        // 存储映射关系
//        m_indexFriendsGroupMap.insert(GROUP_NAME_DEFAULT, m_toolItemsFriendsVec.size()-1);
//    }

    int num = 0;
    int len = friendsVec.size();
    for(int i=0; i<len; i++)
    {
        addFriendButton(friendsVec[i]);
        if (ELS_OFFLINE != friendsVec[i].m_status && INVISIBLE != friendsVec[i].m_status)
            num++;
    }

    setOnlineCounter(num);
//    #endif

        getFlocksInformation();
}

/*************************************************
Function Name： setFlockListWidget
Description: 设置群列表
*************************************************/
void TMainWidget::setFlockListWidget(const QVector<TFlockInformation> & flocksVec)
{
//    return;

    qDebug() << "flock-size:" << flocksVec.size();

    int len = flocksVec.size();
    for(int i=0; i<len; i++)
    {
        addFlockButton(flocksVec[i]);
    }
    getDiscussionsInformation();
}

/*************************************************
Function Name： setDiscussionListWidget
Description: 设置讨论组列表
*************************************************/
void TMainWidget::setDiscussionListWidget(const QVector<TDiscussionInformation> & disVec)
{
//    return;
    qDebug() << "dis-seize:" << disVec.size();

    int len = disVec.size();
    for(int i=0; i<len; i++)
    {
        addDiscussionButton(disVec[i]);
    }
}


/*************************************************
Function Name： addFriendButton
Description: 添加好友按钮
*************************************************/
bool TMainWidget::addFriendButton(const TFriendInformation & friInfo)
{
    if (m_friendMap.contains(friInfo.m_userID))
        return false;
    QString groupName = friInfo.m_groupName;
    if (!m_friendsGroupList.contains(groupName))
    {
        // 存储 新的分组号
        m_friendsGroupList.append(groupName);

        // 新建并存储分组
//        TToolBox *toolBox = new TToolBox;
        TToolItem *toolItem = new TToolItem(groupName);

        connect(toolItem, SIGNAL(removeBoxSignal(QString)),
                this, SLOT(removeBox(QString)));
        connect(toolItem, SIGNAL(renameBoxSignal(QString)),
                this, SLOT(renameBox(QString)));

        m_toolItemsFriendsVec.push_back(toolItem);
//        toolBox->addItem(toolItem);
//        m_friendListWidget->addItem(toolBox);
        m_friendListWidget->addItem(toolItem);

        // 存储映射关系
        m_indexFriendsGroupMap.insert(groupName, m_toolItemsFriendsVec.size()-1);
    }

    TFriendButton *button = new TFriendButton(friInfo,
                                                &m_friendsGroupList,this, this);
//    button->setAutoRaise(true);
    m_toolItemsFriendsVec[m_indexFriendsGroupMap.value(groupName)]->addItem(button);
    m_friendMap.insert(friInfo.m_userID, button);
    return true;
}


/*************************************************
Function Name： addFlockButton
Description: 添加群按钮
*************************************************/
bool TMainWidget::addFlockButton(const TFlockInformation & flcokInfo)
{
    if (m_flockMap.contains(flcokInfo.m_flockID))
        return false;

    TFlockButton *button = new TFlockButton(flcokInfo, this, this);
//    button->setAutoRaise(true);

    m_flockMap.insert(flcokInfo.m_flockID, button);
    m_flockListWidget->addItem(button);

    return true;
}

/*************************************************
Function Name： addDiscussionButton
Description: 添加讨论组按钮
*************************************************/

bool TMainWidget::addDiscussionButton(const TDiscussionInformation & disInfo)
{
    if (m_discussionMap.contains(disInfo.m_discussionID))
        return false;

    TDiscussionButton *button = new TDiscussionButton(disInfo, this, this);
//    button->setAutoRaise(true);

    m_discussionMap.insert(disInfo.m_discussionID, button);
    m_discussionListWidget->addItem(button);

    return true;
}




/*************************************************
Function Name： receiveFriendTalkMessage
Description: 获取好友发来的消息
*************************************************/
void TMainWidget::receiveFriendTalkMessage(const TTalkMessage &mes)
{
    if (TALK_FLOCK == mes.m_type)
    {
        if (!m_flockMap.contains(mes.m_receiverID))
            return;
        if (0 == mes.m_senderID.compare(m_myself.m_userID))
            return;

        TFlockChatWidget *flockRoom = nullptr;
        if (m_flockRoomMap.contains(mes.m_receiverID))
        {
//            qDebug() << "flock room contains";
//            m_flockRoomMap[mes.m_receiverID]->raise();
//            m_flockRoomMap[mes.m_receiverID]->activateWindow();
//            m_flockRoomMap[mes.m_receiverID]->show();
            flockRoom = m_flockRoomMap[mes.m_receiverID];
        }
        else
        {
            m_flockMap[mes.m_receiverID]->openChatRoom();
            flockRoom = m_flockMap[mes.m_receiverID]->getRoomPoint();
        }
        flockRoom->appendMessageShow(mes);

        if (!m_flockMap[mes.m_receiverID]->isRoomShow())
        {
            // 创建 IMLinkFlcokButton
            if (!m_linkMap.contains(mes.m_receiverID))
            {
                IMLinkFlockButton * btn = new IMLinkFlockButton(
                            m_flockMap[mes.m_receiverID], this);
                connect(btn, SIGNAL(deleteLinkButton(QString)),
                        this, SLOT(removeLinkButton(QString)));
                m_linkMap.insert(mes.m_receiverID, btn);
                m_messageListWidget->addItem(btn);
                m_tabWidget->setTabText(3, QString(tr("新消息%1"))
                                        .arg(m_messageListWidget->size()));
            }

             m_linkMap[mes.m_receiverID]->setLatestMessage(mes);
        }
        return;
    }
    if (TALK_DISCUSSION == mes.m_type)
    {
        if (!m_discussionMap.contains(mes.m_receiverID))
            return;
        if (0 == mes.m_senderID.compare(m_myself.m_userID))
            return;

        TDiscussionChatWidget *discussionRoom = nullptr;
        if (m_discussionRoomMap.contains(mes.m_receiverID))
        {
//            qDebug() << "discussion room contains";
//            m_discussionRoomMap[mes.m_receiverID]->raise();
//            m_discussionRoomMap[mes.m_receiverID]->activateWindow();
//            m_discussionRoomMap[mes.m_receiverID]->show();
            discussionRoom = m_discussionRoomMap[mes.m_receiverID];
        }
        else
        {
            m_discussionMap[mes.m_receiverID]->openChatRoom();
            discussionRoom = m_discussionMap[mes.m_receiverID]->getRoomPoint();
        }
        discussionRoom->appendMessageShow(mes);

        if (!m_discussionMap[mes.m_receiverID]->isRoomShow())
        {
            // 创建 IMLinkDiscussionButton
            if (!m_linkMap.contains(mes.m_receiverID))
            {
                IMLinkDiscussionButton * btn = new IMLinkDiscussionButton(
                            m_discussionMap[mes.m_receiverID], this);
                connect(btn, SIGNAL(deleteLinkButton(QString)),
                        this, SLOT(removeLinkButton(QString)));
                m_linkMap.insert(mes.m_receiverID, btn);
                m_messageListWidget->addItem(btn);
                m_tabWidget->setTabText(3, QString(tr("新消息%1"))
                                        .arg(m_messageListWidget->size()));
            }
            m_linkMap[mes.m_receiverID]->setLatestMessage(mes);
        }
        return;
    }


    TChatWidget *chatRoom = nullptr;

    if (!m_friendMap.contains( mes.m_senderID))
    {
        return;
    }

    if (m_chatRoomMap.contains(mes.m_senderID))
    {
//        qDebug("chat room contains");
//        m_chatRoomMap[mes.m_senderID]->raise();
//        m_chatRoomMap[mes.m_senderID]->activateWindow();
//        m_chatRoomMap[mes.m_senderID]->show();
       chatRoom =  m_chatRoomMap[mes.m_senderID];
    }
    else
    {
//        TChatInformation chatInf;
//        chatInf.m_friendID = mes.m_senderID;

//        chatInf.m_headPortrait = m_info.m_headPortrait;
//    //    chatInf.m_friendMark = m_info.m_mark;
//        chatInf.m_friendNickname = m_info.m_nickname;
//        chatInf.m_friendRemark = m_info.m_remarkName;

//        chatRoom = new TChatWidget(chatInf);
//        m_chatRoomMap.insert(chatInf.m_friendID, chatRoom);
//        connect(chatRoom, SIGNAL(sendMessagesFromChat(TTalkMessage &)),
//                this, SLOT(receiveMessagesFromChat(TTalkMessage &)));
//    //    connect(room, SIGNAL(roomQuitSignal()),  this, SLOT(chatRoomQuit()));

//        chatRoom->show();
        m_friendMap[mes.m_senderID]->openChatRoom();
        chatRoom = m_friendMap[mes.m_senderID]->getRoomPoint();
    }

    switch (mes.m_type)
    {
    case TALK_MESSAGE:
        chatRoom->appendMessageShow(mes);
        // 存储消息记录 至 数据库
        m_database.addHistoryMessage(mes);
        if (!m_friendMap[mes.m_senderID]->isRoomShow())
        {
            // 创建 IMLinkFriendButton
            if (!m_linkMap.contains(mes.m_senderID))
            {
                IMLinkFriendButton * btn = new IMLinkFriendButton(
                            m_friendMap[mes.m_senderID], this);
                connect(btn, SIGNAL(deleteLinkButton(QString)),
                        this, SLOT(removeLinkButton(QString)));
                m_linkMap.insert(mes.m_senderID, btn);
                m_messageListWidget->addItem(btn);
                m_tabWidget->setTabText(3, QString(tr("新消息%1"))
                                        .arg(m_messageListWidget->size()));
            }
            m_linkMap[mes.m_senderID]->setLatestMessage(mes);
        }

        break;
    case REQUEST_VIDEO:
        chatRoom->pendingVideo(mes);
    case AGREE_VOICE:
//        chatRoom->showCamera(mes);
        // 创建视频窗口
        // 发送 接收
    case REFUSE_VOICE:
        chatRoom->closeVideoWidget();
        break;

    case REQUEST_FILE:
    {
        chatRoom->pendingFile(mes);
        break;
    }
    case AGREE_FILE:
    {
        break;
    }
    case REFUSE_FILE:
    {
        chatRoom->refusedSendFile();
        break;
    }
    }

//	//打开聊天窗口，并显示对方发来的信息
//	iter.value()->openChatRoom();
//	iter.value()->getRoomPoint()->setOutputLine(mes);

    //appendMessageShow();
}

/*************************************************
Function Name： setFriendStatus
Description: 设置好友状态
*************************************************/
void TMainWidget::setFriendStatus(const QString & id,int status)
{
    QMap<QString, TFriendButton*>::iterator iter;
    iter = m_friendMap.find(id);
    //如果好友列表中不存在这个人，就直接返回
    if(m_friendMap.end() == iter)
        return;

    iter.value()->getInformation().m_status = status;
    iter.value()->setButtonStatus();
}


/*************************************************
Function Name： setFriendInformation
Description: 设置好友信息
*************************************************/
void TMainWidget::setFriendInformation(const TFriendInformation &fri)
{
    QMap<QString, TFriendButton*>::iterator iter;
    iter = m_friendMap.find(fri.m_userID);
    //如果好友列表中不存在这个人，就直接返回
    if(m_friendMap.end() == iter)
        return;

    iter.value()->setChangedInformation(fri);
    iter.value()->setButtonStatus();
}

/*************************************************
Function Name： showFriendInformation
Description: 显示好友信息
*************************************************/
void TMainWidget::showFriendInformation(const TUserInformation &user)
{
    if (m_informationMap.contains(user.m_userID))
    {
        m_informationMap[user.m_userID]->setUserInformation(user);
        m_informationMap[user.m_userID]->raise();
        m_informationMap[user.m_userID]->activateWindow();
        m_informationMap[user.m_userID]->show();
        return;
    }

    TInformationWidget *infWidget = new TInformationWidget(this);
    m_informationMap.insert(user.m_userID, infWidget);
    infWidget->setReadOnly(true, false);
    infWidget->setUserInformation(user);
    infWidget->show();
}

/*************************************************
Function Name： showMyInformation
Description: 显示自己信息
*************************************************/
void TMainWidget::showMyInformation(const TUserInformation &user)
{
    m_myself = user;
    if (m_informationMap.contains(m_myself.m_userID))
    {
        m_informationMap[m_myself.m_userID]->raise();
        m_informationMap[m_myself.m_userID]->activateWindow();
        m_informationMap[m_myself.m_userID]->show();
        return;
    }

    TInformationWidget *infWidget = new TInformationWidget(this);
    m_informationMap.insert(m_myself.m_userID, infWidget);
    infWidget->setReadOnly(true, true);
    infWidget->setUserInformation(user);
    infWidget->show();
}

/*************************************************
Function Name： setMyInformation
Description: 设置个人信息
*************************************************/
void TMainWidget::setMyInformation(const TUserInformation &user)
{
    QString str = QString("resource/image/head/%1.bmp").
            arg(QString::number(user.m_headPortrait));
    m_labelHead->setPixmap(QPixmap(str));
//    m_cbStatus->setCurrentIndex(user.m_status);
    m_labelNickname->setText(QString("%1\n%2").
                             arg(user.m_nickname, user.m_userID));
//    m_labelMark->setText(user.m_mark); // 说说 签名
}

/*************************************************
Function Name： showFlockInformation
Description: 显示群信息
*************************************************/
void TMainWidget::showFlockInformation(const TFlockInformation & flock)
{
    if (m_flockInformationMap.contains(flock.m_flockID))
    {
        m_flockInformationMap[flock.m_flockID]->raise();
        m_flockInformationMap[flock.m_flockID]->activateWindow();
        m_flockInformationMap[flock.m_flockID]->show();
        return;
    }

    TFlockInformationWidget *infWidget = new TFlockInformationWidget(this);
    m_flockInformationMap.insert(flock.m_flockID, infWidget);
    if (flock.m_creatorID.compare(m_myself.m_userID) == 0)
        infWidget->setReadOnly(true, true);
    else
        infWidget->setReadOnly(true, false);

    infWidget->setFlockInformation(flock);
    infWidget->show();
}


/*************************************************
Function Name： showDiscussionInformation
Description: 显示讨论组信息
*************************************************/
void TMainWidget::showDiscussionInformation(const TDiscussionInformation & discussion)
{
    if (m_discussionInformationMap.contains(discussion.m_discussionID))
    {
        m_discussionInformationMap[discussion.m_discussionID]->raise();
        m_discussionInformationMap[discussion.m_discussionID]->activateWindow();
        m_discussionInformationMap[discussion.m_discussionID]->show();
        return;
    }

    TDiscussionInformationWidget *infWidget = new TDiscussionInformationWidget(this);
    m_discussionInformationMap.insert(discussion.m_discussionID, infWidget);
//    if (discussion.m_creatorID.compare(m_myself.m_userID) == 0)
        infWidget->setReadOnly(true, true);
//    else
//        infWidget->setReadOnly(true, false);

    infWidget->setDiscussionInformation(discussion);
    infWidget->show();
}


/*************************************************
Function Name： setFlockInformation
Description: 设置群信息
*************************************************/
void TMainWidget::setFlockInformation(const TFlockInformation & flock)
{
//    if (m_flockInformationMap.contains(flock.m_flockID))
//    {
//        m_flockInformationMap[flock.m_flockID]->setFlockInformation(flock);
//        m_flockInformationMap[flock.m_flockID]->raise();
//        m_flockInformationMap[flock.m_flockID]->activateWindow();
//        m_flockInformationMap[flock.m_flockID]->show();
//        return;
//    }
    if (m_flockMap.contains(flock.m_flockID))
    {
        m_flockMap[flock.m_flockID]->setChangedInformation(flock);
    }
    if (m_flockRoomMap.contains(flock.m_flockID))
    {
        m_flockRoomMap[flock.m_flockID]->setChatInformation(flock);
    }

}



/*************************************************
Function Name： setDiscussionInformation
Description: 设置讨论组信息
*************************************************/
void TMainWidget::setDiscussionInformation(const TDiscussionInformation & discussion)
{
    if (m_discussionMap.contains(discussion.m_discussionID))
    {
        m_discussionMap[discussion.m_discussionID]->setChangedInformation(discussion);
    }
    if (m_discussionRoomMap.contains(discussion.m_discussionID))
    {
        m_discussionRoomMap[discussion.m_discussionID]->setChatInformation(discussion);
    }
}


/*************************************************
Function Name： setFlockMemberRemark
Description: 设置群成员名片
*************************************************/
void TMainWidget::setFlockMemberRemark(const TTempStrings & tmpStr)
{
    if (m_flockRoomMap.contains(tmpStr.m_one))
    {
        m_flockRoomMap[tmpStr.m_one]->setFlockMemberRemark(tmpStr);
    }

}

/*************************************************
Function Name： setFriendRemark
Description: 设置好友备注
*************************************************/
void TMainWidget::setFriendRemark(const TTempStrings& tempStr)
{
    if (tempStr.m_one != m_myself.m_userID)
        return;
    QMap<QString, TFriendButton*>::iterator iter;
    iter = m_friendMap.find(tempStr.m_two);
    //如果好友列表中不存在这个人，就直接返回
    if(m_friendMap.end() == iter)
        return;

    iter.value()->getInformation().m_remarkName = tempStr.m_three;
    iter.value()->setButtonStatus();
}


/*************************************************
Function Name： deleteFriend
Description: 已删除好友
*************************************************/
void TMainWidget::deletedFriend(const QString & myID, const QString & friendID,
                                 const QString & groupName)
{
    if (myID != m_myself.m_userID)
        return;


    if (!m_friendMap.contains(friendID))
        return;

    m_toolItemsFriendsVec[m_indexFriendsGroupMap.value(groupName)]->removeItem(m_friendMap[friendID]);
//    removeItem(m_indexFriendsGroupMap[friendID]);
//    m_indexFriendsGroupMap[friendID]->setVisible(false);
//    m_indexFriendsGroupMap[friendID]->setEnabled(false);
//    delete m_indexFriendsGroupMap[friendID];
//    TFriendButton *button = m_indexFriendsGroupMap.find();
    m_friendMap[friendID]->deleteLater();
    m_friendMap.remove(friendID);

    if (m_chatRoomMap.contains(friendID))
    {
        m_chatRoomMap[friendID]->close();
        m_chatRoomMap[friendID]->deleteLater();
        m_chatRoomMap.remove(friendID);
    }
//    m_indexFriendsGroupMap.erase(friendID);

//    QMap<QString, TFriendButton*>::iterator iter;
//    iter = m_indexFriendsGroupMap.find(friendID);
//    //如果好友列表中不存在这个人，就直接返回
//    if(m_indexFriendsGroupMap.end() == iter)
//        return;
//    iter.value()
    qDebug() << "friend deleted";
    setOnlineCounter(m_onlineCounter - 1);
}


/*************************************************
Function Name： dropedFlock
Description: 已删除群
*************************************************/
void TMainWidget::dropedFlock(const QString & flockID)
{
    if (!m_flockMap.contains(flockID))
        return;

    QMessageBox::information(nullptr, tr("群消息"),
                             QString(tr("群%1已经被群主解散")).arg(flockID));

    if (m_flockRoomMap.contains(flockID))
    {
        m_flockRoomMap[flockID]->close();
        m_flockRoomMap[flockID]->deleteLater();
        m_flockRoomMap[flockID] = nullptr;
        m_flockRoomMap.remove(flockID);
    }

    m_flockListWidget->removeItem( m_flockMap[flockID] );
    m_flockMap[flockID]->deleteLater();
    m_flockMap.remove(flockID);




    qDebug() << "flock drop";
}



/*************************************************
Function Name： dropedDiscussion
Description: 已删除讨论组
*************************************************/
void TMainWidget::dropedDiscussion(const QString & discussionID)

{
    if (!m_discussionMap.contains(discussionID))
        return;

    QMessageBox::information(nullptr, tr("讨论组消息"),
                             QString(tr("讨论组%1已经被组长解散")).arg(discussionID));

    if (m_discussionRoomMap.contains(discussionID))
    {
        m_discussionRoomMap[discussionID]->close();
        m_discussionRoomMap[discussionID]->deleteLater();
        m_discussionRoomMap[discussionID] == nullptr;
        m_discussionRoomMap.remove(discussionID);
    }


    m_discussionListWidget->removeItem( m_discussionMap[discussionID] );
    m_discussionMap[discussionID]->deleteLater();
    m_discussionMap.remove(discussionID);




    qDebug() << "discussion drop";
}

/*************************************************
Function Name： leavedFlock
Description: 已退出群
*************************************************/
void TMainWidget::leavedFlock(const QString & memberID,
                               const QString & flockID,
                               const QString & creatorID)
{
    if (creatorID == m_myself.m_userID)
    {
        QMessageBox::information(nullptr, tr("群消息"),
                                 QString(tr("用户%1已经退出群%2")).
                                 arg(memberID, flockID));

        // 如果群是打开的，最好及时刷新，将成员在群中删除
        // 不然要等到下次打开群的时候，才能看到成员已被删除
        // 关系也不大
        if (m_flockRoomMap.contains(flockID))
        {
            m_flockRoomMap[flockID]->removeFlockMemberButton(memberID);
        }
        if (m_flockMap.contains(flockID))
        {
            m_flockMap[flockID]->deleteFlockMember(memberID);
        }
    }
    else if (memberID == m_myself.m_userID)
    {
        QMessageBox::information(nullptr, tr("群消息"),
                                 QString(tr("您好,%1,您已经退出群%2")).
                                 arg(memberID, flockID));

        if (m_flockRoomMap.contains(flockID))
        {
            m_flockRoomMap[flockID]->close();
            m_flockRoomMap[flockID]->deleteLater();
            m_flockRoomMap.remove(flockID);
        }

        if (m_flockMap.contains(flockID))
        {
            m_flockListWidget->removeItem( m_flockMap[flockID] );
            m_flockMap[flockID]->deleteLater();
            m_flockMap.remove(flockID);
        }
        qDebug() << "leaved flock";
    }
    else
    {
        if (m_flockRoomMap.contains(flockID))
        {
            m_flockRoomMap[flockID]->removeFlockMemberButton(memberID);
        }
        if (m_flockMap.contains(flockID))
        {
            m_flockMap[flockID]->deleteFlockMember(memberID);
        }
    }


}


/*************************************************
Function Name： leavedDiscussion
Description: 已退出讨论组
*************************************************/
void TMainWidget::leavedDiscussion(const QString & memberID,
                                    const QString & discussionID)
{
    if (memberID == m_myself.m_userID)
    {
//        QMessageBox::information(nullptr, tr("讨论组消息"),
//                                 QString(tr("您好,%1,您已经退出讨论组%2")).
//                                 arg(memberID, discussionID));

        if (m_discussionRoomMap.contains(discussionID))
        {
            m_discussionRoomMap[discussionID]->close();
            m_discussionRoomMap[discussionID]->deleteLater();
            m_discussionRoomMap.remove(discussionID);
        }

        if (m_discussionMap.contains(discussionID))
        {
            m_discussionListWidget->removeItem( m_discussionMap[discussionID] );
            m_discussionMap[discussionID]->deleteLater();
            m_discussionMap.remove(discussionID);
        }
        qDebug() << "leaved discussion";
    }

//         if (creatorID == m_myself.m_userID)
    else
    {
//             QMessageBox::information(nullptr, tr("群消息"),
//                                      QString(tr("用户%1已经退出讨论组%2")).
//                                      arg(memberID, discussionID));

//             // 如果讨论组是打开的，最好及时刷新，将成员在讨论组中删除
//             // 不然要等到下次打开群的时候，才能看到成员已被删除
//             // 关系也不大
             if (m_discussionRoomMap.contains(discussionID))
             {
                 m_discussionRoomMap[discussionID]->removeDiscussionMemberButton(memberID);
             }
             if (m_discussionMap.contains(discussionID))
             {
                 m_discussionMap[discussionID]->deleteDiscussionMember(memberID);
             }
    }


}


/*************************************************
Function Name： createFlockSuccess
Description: 创建群成功
*************************************************/
void TMainWidget::createFlockSuccess(const TFlockInformation & flock)
{
    if (flock.m_creatorID.compare(m_myself.m_userID) != 0)
        return;

    if (m_flockMap.contains(flock.m_flockID))
        return;
    if (!addFlockButton(flock))
        return;
    QString temp =  QString(tr("您好,%1(%2).\n您已经成功创建群%3(%4).\n创建日期: %5.")).
                    arg(m_myself.m_userID, m_myself.m_nickname,
                        flock.m_flockID, flock.m_flockName,
                        flock.m_createDate.toString("yyyy-MM-dd"));

    QMessageBox::information(nullptr, tr("群消息"), temp);
}


/*************************************************
Function Name： createDiscussionSuccess
Description: 创建讨论组成功
*************************************************/
void TMainWidget::createDiscussionSuccess(const TDiscussionInformation & discussion)
{
    if (discussion.m_creatorID.compare(m_myself.m_userID) != 0)
        return;

    if (m_discussionMap.contains(discussion.m_discussionID))

        return;
    if (!addDiscussionButton(discussion))
        return;
    QString temp =  QString(tr("您好,%1(%2).\n您已经成功创建讨论组%3(%4).\n创建日期: %5.")).
                    arg(m_myself.m_userID, m_myself.m_nickname,
                        discussion.m_discussionID, discussion.m_discussionName,
                        discussion.m_createDate.toString("yyyy-MM-dd"));

    QMessageBox::information(nullptr, tr("讨论组消息"), temp);
}


/*************************************************
Function Name： searchFriend
Description: 搜索友好
*************************************************/
void TMainWidget::searchFriend(const QString & text)
{
    m_searchListWidget->setVisible(true);
    m_tabWidget->setVisible(false);
    if (text.isEmpty())
    {
        qDebug() << "empty";
        onClickSearchFriend();
        return;
    }
    m_toolBtnSearch->setIcon(QIcon(":/images/cancel.png"));
    m_toolBtnSearch->setEnabled(true);

    m_searchListWidget->clearAll();
    //搜索匹配的好友
    QMapIterator<QString, TFriendButton *> i(m_friendMap);
     while (i.hasNext()) {
         i.next();
         if(i.key().contains(text)
                 || i.value()->getInformation().m_nickname.contains(text)
                 || i.value()->getInformation().m_remarkName.contains(text))
         {
             IMLinkFriendButton * btn = new IMLinkFriendButton(
                         m_friendMap[i.key()], this);
             connect(btn, SIGNAL(deleteLinkButton(QString)),
                     this, SLOT(removeLinkButton(QString)));
             m_searchListWidget->addItem(btn);
         }
     }

//     IMLinkFriendButton * btn = new IMLinkFriendButton(
//                 m_friendMap[mes.m_senderID], this);
//     connect(btn, SIGNAL(deleteLinkButton(QString)),
//             this, SLOT(removeLinkButton(QString)));
//     m_linkMap.insert(mes.m_senderID, btn);
//     m_messageListWidget->addItem(btn);
}

/*************************************************
Function Name： onClickSearchFriend
Description: 点击“搜索”按钮
*************************************************/
void TMainWidget::onClickSearchFriend()
{
    qDebug() << "click search friend.";

    m_toolBtnSearch->setIcon(QIcon(":/images/search.png"));
    m_toolBtnSearch->setEnabled(false);
    m_searchListWidget->setVisible(false);
    m_tabWidget->setVisible(true);
}

/*************************************************
Function Name： onClickAddFriend
Description: 添加好友
*************************************************/
void TMainWidget::onClickAddFriend()
{
    bool isOk = false;
    QString friendID = QInputDialog::getText(nullptr, "添加好友",
                                           "请输入对方帐号",
                                           QLineEdit::Normal,
                                           "",
                                           &isOk);
    if (!isOk)
        return;

    // mark
    // 检验 是否全是 数字 或者 一开始就限制 或者 重写一个messagebox

    addFriend(friendID);
}

/*************************************************
Function Name： addFlock
Description: 添加群
*************************************************/
void TMainWidget::onClickAddFlock()
{
    bool isOk = false;
    QString flockID = QInputDialog::getText(nullptr, "添加群",
                                           "请输入群帐号",
                                           QLineEdit::Normal,
                                           "",
                                           &isOk);
    if (!isOk)
        return;

    // mark
    // 检验 是否全是 数字 或者 一开始就限制 或者 重写一个messagebox

    if (m_flockMap.contains(flockID))
    {
        QMessageBox::critical(nullptr, tr("添加失败"), tr("你已经加入该群"));
        return;
    }

    bool isOkMes = false;
    QString checkMes = QInputDialog::getText(nullptr, "添加群",
                                           "请输入验证信息",
                                           QLineEdit::Normal,
                                           QString(tr("你好,我是: %1")).
                                             arg(m_myself.m_nickname),
                                           &isOkMes);
    if (!isOkMes)
        return;

//    TTempStrings temp;
//    temp.m_one = m_myself.m_userID;
//    temp.m_two = friendID;
//    temp.m_three = checkMes;

    TTalkMessage mes;
    mes.m_senderID = m_myself.m_userID;
    mes.m_receiverID = flockID;
    mes.m_text = checkMes;
    mes.m_type = REQUEST_FLOCK;

    if (nullptr != m_mainCtrl)
        m_mainCtrl->addFlock(mes);

}


/*************************************************
Function Name： onClickChangePwd
Description: 修改密码
*************************************************/
void TMainWidget::onClickChangePwd()
{
    bool isOkMes = false;
    QString oldPwd = QInputDialog::getText(nullptr, "修改密码",
                                           "请输入原密码:",
                                           QLineEdit::Password,
                                           nullptr,
                                           &isOkMes);
    if (!isOkMes)
        return;

    while(1)
    {
        QString newPwdOne = QInputDialog::getText(nullptr, "修改密码",
                                               "请输入新密码:",
                                               QLineEdit::Password,
                                               nullptr,
                                               &isOkMes);
        if (!isOkMes)
            return;
        if (newPwdOne.length() < 6)
        {
            QMessageBox::critical(nullptr, tr("修改密码"),
                                  tr("新密码长度不能少于6个字符！"));
            continue;
        }
        if (newPwdOne.length() > 14)
        {
            QMessageBox::critical(nullptr, tr("修改密码"),
                                  tr("新密码长度不能超过14个字符！"));
            continue;
        }

        QString newPwdTwo = QInputDialog::getText(nullptr, "修改密码",
                                               "请再次输入新密码:",
                                               QLineEdit::Password,
                                               nullptr,
                                               &isOkMes);
        if (!isOkMes)
            return;
        if (newPwdTwo != newPwdOne)
        {
            QMessageBox::critical(nullptr, tr("修改密码"), tr("两次新密码不一致！"));
            continue;
        }
        else
        {
            if (nullptr != m_mainCtrl)
            {
                QString oldPassword = IMEncryption::getInstace().
                        getXorEncryptDecrypt(oldPwd, 10);
                QString newPassword = IMEncryption::getInstace().
                        getXorEncryptDecrypt(newPwdOne, 10);
                m_mainCtrl->requestChangePwd(oldPassword, newPassword);
                break;
            }
        }

    }
}

/*************************************************
Function Name： createFlock
Description: 创建群
*************************************************/
void TMainWidget::createFlock()
{
    bool isOk = false;
    QString flockName = QInputDialog::getText(nullptr, "创建群",
                                           "请输入群名",
                                           QLineEdit::Normal,
                                           QString("%1的IM群").
                                              arg(m_myself.m_nickname),
                                           &isOk);
    if (!isOk)
        return;

    isOk = false;
    QString flockTheme = QInputDialog::getText(nullptr, "创建群",
                                           "请输入群主题",
                                           QLineEdit::Normal,
                                           tr("没有主题"),
                                           &isOk);
    if (!isOk)
        return;

    TTempStrings tmpStr;
    tmpStr.m_one = m_myself.m_userID;
    tmpStr.m_two = flockName;
    tmpStr.m_three = flockTheme;

    if (nullptr != m_mainCtrl)
        m_mainCtrl->createFlock(tmpStr);
}


/*************************************************
Function Name： createDiscussion
Description: 创建讨论组
*************************************************/
void TMainWidget::createDiscussion()
{
    bool isOk = false;
    QString name = QInputDialog::getText(nullptr, "创建讨论组",
                                           "请输入组名",
                                           QLineEdit::Normal,
                                           QString("%1的IM讨论组").
                                              arg(m_myself.m_nickname),
                                           &isOk);
    if (!isOk)
        return;

    isOk = false;
    QString theme = QInputDialog::getText(nullptr, "创建讨论组",
                                           "请输入主题",
                                           QLineEdit::Normal,
                                           tr("没有主题"),
                                           &isOk);
    if (!isOk)
        return;

    TTempStrings tmpStr;
    tmpStr.m_one = m_myself.m_userID;
    tmpStr.m_two = name;
    tmpStr.m_three = theme;

    if (nullptr != m_mainCtrl)
        m_mainCtrl->createDiscussion(tmpStr);
}

/*************************************************
Function Name： showMessageWidget
Description: 显示消息管理窗口
*************************************************/
void TMainWidget::showMessageWidget()
{
    if (m_messageManWidget == nullptr)
    {
        m_messageManWidget = new TMessageManageWidget(this);
    }
    m_messageManWidget->setListModelGroup();

    m_messageManWidget->raise();
    m_messageManWidget->activateWindow();
    m_messageManWidget->show();
}

/*************************************************
Function Name： showMailWidget
Description: 显示邮箱
*************************************************/
void TMainWidget::showMailWidget()
{
    if (m_mailWidget == nullptr)
    {
        m_mailWidget = new TMailWidget(m_myself.m_userID);
    }
    m_mailWidget->raise();
    m_mailWidget->activateWindow();
    m_mailWidget->show();
}

/*************************************************
Function Name： showMailWriteWidget
Description: 显示邮箱中写信窗口
*************************************************/
void TMainWidget::showMailWriteWidget(const QString & address)
{
    showMailWidget();
    m_mailWidget->showMailWriteWidget(address);
}

/*************************************************
Function Name： showLatestMessageWidget
Description: 显示最新消息窗口
*************************************************/
void TMainWidget::showLatestMessageWidget()
{
    if (m_messageListWidget == nullptr)
    {
        return;
    }
    m_tabWidget->setCurrentWidget(m_messageListWidget);
}

/*************************************************
Function Name： setTimerStatus
Description: 设置显示有新消息的定时器
*************************************************/
void TMainWidget::setTimerStatus(bool isOpen)
{
    if (isOpen)
    {
        if (0 == m_timerId)
        {
            m_toolBtnNewMes->setHidden(false);
            m_timerId=startTimer(400);//400ms
            m_toolBtnNewMes->setStatusTip(tr("查阅最新消息"));
            m_toolBtnNewMes->setToolTip(QString(tr("有新消息")));
//                                        .arg(m_messageListWidget->size()));
        }
    }

    else if (!isOpen && m_timerId != 0)
    {
        killTimer(m_timerId);//kill timer
        m_timerId = 0;
        m_toolBtnNewMes->setToolTip(tr("无新消息"));
        m_toolBtnNewMes->setStatusTip(tr("无最新消息"));
        m_toolBtnNewMes->setHidden(true);
    }

}

/*************************************************
Function Name： renameBoxSuccess
Description: 重命名分组成功
*************************************************/
void TMainWidget::renameBoxSuccess(const QString & title, const QString & newTitle)
{
    // 先修改： 组名 与 下标 映射表
    int index = m_indexFriendsGroupMap.value(title);
//    if (index == 0)
//    {
//        qDebug("cannot find this group");
//        return;
//    }
    m_indexFriendsGroupMap.remove(title);
    m_indexFriendsGroupMap.insert(newTitle, index);

    // 通过 存储每个分组的向量 获取分组 item ，再修改组名
    m_toolItemsFriendsVec[index]->setTile(newTitle);

    m_friendsGroupList[m_friendsGroupList.indexOf(title)] = newTitle;

    if (m_messageManWidget != nullptr)
        m_messageManWidget->setListModelGroup();
    refreshFriendButtonMenu();
}


/*************************************************
Function Name： moveFriendToBoxSuccess
Description: 移动好友至其他分组
*************************************************/
void TMainWidget::moveFriendToBoxSuccess(const QString & friendID, const QString & title,
                            const QString & newTitle)
{
    if (!m_friendMap.contains(friendID))
        return;
    m_toolItemsFriendsVec[m_indexFriendsGroupMap.value(title)]->removeItem(m_friendMap[friendID]);

    m_friendMap[friendID]->getInformation().m_groupName = newTitle;
    m_friendMap[friendID]->refreshMoveMenu();
    m_toolItemsFriendsVec[m_indexFriendsGroupMap.value(newTitle)]->addItem(m_friendMap[friendID]);
}


/*************************************************
Function Name： onClickRightButton()
Description: 右键点击按键
*************************************************/
void TMainWidget::onClickRightButton()
{
    if (m_menu == nullptr)
    {
        //创建右键菜单
        creatMenu();
    }
    m_menu->exec(QCursor::pos() - QPoint(105, 125));
}

/*************************************************
Function Name： createBox
Description: 新建组
*************************************************/
void TMainWidget::createBox()
{
    bool isOk = false;
    QString groupName = QInputDialog::getText(nullptr, "新建组",
                                           "请输入分组名",
                                           QLineEdit::Normal,
                                           tr("new group"),
                                           &isOk);

    if (isOk)
    {
        if (m_indexFriendsGroupMap.contains(groupName))
        {
            QMessageBox::information(nullptr, tr("新建组"), tr("无法创建，因为存在同名的组！"));
            return;
        }

        // 存储 新的分组号
        m_friendsGroupList.append(groupName);

        // 新建并存储分组
//        TToolBox *toolBox = new TToolBox;
        TToolItem *toolItem = new TToolItem(groupName);

        connect(toolItem, SIGNAL(removeBoxSignal(QString)),
                this, SLOT(removeBox(QString)));
        connect(toolItem, SIGNAL(renameBoxSignal(QString)),
                this, SLOT(renameBox(QString)));

        m_toolItemsFriendsVec.push_back(toolItem);
        m_friendListWidget->addItem(toolItem);

        // 存储映射关系
        m_indexFriendsGroupMap.insert(groupName, m_toolItemsFriendsVec.size()-1);

        if (m_messageManWidget != nullptr)
            m_messageManWidget->setListModelGroup();
        refreshFriendButtonMenu();
    }

}


/*************************************************
Function Name： removeLinkButton
Description: 删除新消息连接按钮
*************************************************/
void TMainWidget::removeLinkButton(const QString & id)
{
    if (m_linkMap.contains(id))
    {
        m_messageListWidget->removeItem(m_linkMap[id]);
        m_linkMap[id]->deleteLater();
        m_linkMap.remove(id);
        m_tabWidget->setTabText(3, QString(tr("新消息(%1)"))
                                .arg(m_messageListWidget->size()));
    }
    else
    {
        qDebug() << "there is some error in linkButton delete.";
    }
}

/*************************************************
Function Name： deleteAllLinkButton
Description: 删除所有消息连接按钮
*************************************************/
void TMainWidget::removeAllLinkButton()
{
    QMapIterator<QString, TLinkButton*> i(m_linkMap);
    while (i.hasNext())
    {
        i.next();
        m_messageListWidget->removeItem(i.value());
        m_linkMap[i.key()]->deleteLater();
        m_linkMap.remove(i.key());
    }
    m_tabWidget->setTabText(3, QString(tr("新消息(%1)")).arg(0));
}

/*************************************************
Function Name： showAllChatRoom
Description: 显示所有聊天窗口
*************************************************/
void TMainWidget::showAllChatRoom()
{
    QMapIterator<QString, TLinkButton*> i(m_linkMap);
    while (i.hasNext())
    {
        i.next();
        i.value()->onDoubleClickButton();
    }
//    removeAllLinkButton();
}

// mark protected:------------------------------------------------------
/*************************************************
Function Name： closeEvent
Description: 关闭事件
*************************************************/
void TMainWidget::closeEvent(QCloseEvent *event)
{
    if(continueToClose())
    {
        //关闭主窗口之前，先关闭所有聊天窗口
        closeAllWindows();
        event->accept();
    }
    else
        event->ignore();
}

/*************************************************
Function Name： mousePressEvent
Description: 鼠标点击事件
*************************************************/
void TMainWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
//        onClickRightButton();
        return;
    }
    QWidget::mousePressEvent(event);
}

/*************************************************
Function Name： timerEvent
Description: 定时器事件
*************************************************/
void TMainWidget::timerEvent(QTimerEvent *event)
{
    static bool show = true;
    if (event->timerId() == m_timerId)
    {
        if (show)
            m_toolBtnNewMes->setIcon(QIcon(":/images/newMessage.png"));
        else
            m_toolBtnNewMes->setIcon(QIcon(""));//":/images/noMessage.png"));
        show = !show;
    }
}

// mark: private slots:-------------------------------------------

/*************************************************
Function Name： closeWindow
Description: 关闭主窗口
*************************************************/
void TMainWidget::closeWindow()
{
    m_flag = 1;
    closeAllWindows();
    m_mainCtrl->closeConnect();
    close();
}

/*************************************************
Function Name： closeAllWindows
Description: 关闭所有别的窗口
*************************************************/
void TMainWidget::closeAllWindows()
{
    QMap<QString, TChatWidget*>::iterator iterFriendChat;
    for(iterFriendChat=m_chatRoomMap.begin();
        iterFriendChat!=m_chatRoomMap.end();
        ++iterFriendChat)
    {
//        TChatWidget * widget = iterFriendChat.value();
//        if ( widget!= nullptr)
//        {
//            iterFriendChat.value()->close();
        iterFriendChat.value()->deleteLater();

//            widget->close();
//            widget->deleteLater();
//            widget = nullptr;
//        }
    }

    QMap<QString, TFlockChatWidget*>::iterator iterFlockChat;
    for(iterFlockChat=m_flockRoomMap.begin();
        iterFlockChat!=m_flockRoomMap.end();
        ++iterFlockChat)
    {
//            iterFlockChat.value()->close();
        iterFlockChat.value()->deleteLater();
    }

    QMap<QString, TDiscussionChatWidget*>::iterator iterDiscussionChat;
    for(iterDiscussionChat=m_discussionRoomMap.begin();
        iterDiscussionChat!=m_discussionRoomMap.end();
        ++iterDiscussionChat)
    {
//            iterDiscussionChat.value()->close();
        iterDiscussionChat.value()->deleteLater();
    }

    QMap<QString, TInformationWidget*>::iterator iterInformation;
    for(iterInformation=m_informationMap.begin();
        iterInformation!=m_informationMap.end();
        ++iterInformation)
    {
//            iterInformation.value()->close();
        iterInformation.value()->deleteLater();
    }

    QMap<QString, TFlockInformationWidget*>::iterator iterFlockInformation;
    for(iterFlockInformation=m_flockInformationMap.begin();
        iterFlockInformation!=m_flockInformationMap.end();
        ++iterFlockInformation)
    {
//            iterFlockInformation.value()->close();
        iterFlockInformation.value()->deleteLater();
    }

    QMap<QString, TDiscussionInformationWidget*>::iterator iterDiscussionInformation;
    for(iterDiscussionInformation=m_discussionInformationMap.begin();
        iterDiscussionInformation!=m_discussionInformationMap.end();
        ++iterDiscussionInformation)
    {
//            iterDiscussionInformation.value()->close();
        iterDiscussionInformation.value()->deleteLater();
    }

//        m_mailWidget->close();
//        m_messageManWidget->close();
    if (m_mailWidget != nullptr)
    {
        m_mailWidget->deleteLater();
    }
    if (m_messageManWidget != nullptr)
    {
       m_messageManWidget->deleteLater();
    }

}

/*************************************************
Function Name： createChatRoom
Description: 创建聊天室
*************************************************/
void TMainWidget::getMyInformation()
{
    if (nullptr != m_mainCtrl)
        m_mainCtrl->getMynformation(m_myself.m_userID);
}

//void TMainWidget::createChatRoom(QListWidgetItem *item)
//{

//    TChatInformation chatInf;
//    chatInf.m_friendID = item->text();
////    chatInf.m_friendMark = "";
////    chatInf.m_friendNickname = "";
////    TChatInformation.m_friendRemark = "";
//    TChatWidget *chatRoom = new TChatWidget(chatInf);
//    m_chatRoomMap.insert(chatInf.m_friendID, chatRoom);
//    connect(chatRoom, SIGNAL(sendMessagesFromChat(TTalkMessage &)),
//            this, SLOT(receiveMessagesFromChat(TTalkMessage &)));
////    connect(room, SIGNAL(roomQuitSignal()),  this, SLOT(chatRoomQuit()));
//    chatRoom->show();
//}

/*************************************************
Function Name： receiveMessagesFromChat
Description: 获取好友聊天室发出的消息
*************************************************/
void TMainWidget::receiveMessagesFromChat(TTalkMessage &mes)
{
    mes.m_senderID = m_myself.m_userID;
    if (TALK_MESSAGE == mes.m_type)
        m_database.addHistoryMessage(mes);
    if (nullptr != m_mainCtrl)
        m_mainCtrl->sendTalkMessage(mes);
}


/*************************************************
Function Name： receiveMessagesFromFlockChat
Description: 获取群发出的消息
*************************************************/
//void TMainWidget::receiveMessagesFromFlockChat(TTalkMessage &)
//{
//    if (nullptr != m_mainCtrl)
//        m_mainCtrl->sendTalkMessage(mes);
//}

/*************************************************
Function Name： receiveMessagesFromDiscussionChat
Description: 获取讨论组发出的消息
*************************************************/
//void TMainWidget::receiveMessagesFromDiscussionChat(TTalkMessage &)
//{
//    if (nullptr != m_mainCtrl)
//        m_mainCtrl->sendTalkMessage(mes);
//}

/*************************************************
Function Name： changeStatus
Description: 改变个人状态（发送至服务器）
*************************************************/
void TMainWidget::changeMyStatus(const int index)
{
    int status = ELS_ONLINE;
    switch (index)
    {
    case 0:
        status = ELS_ONLINE;
        break;
    case 1:
        status = INVISIBLE;
        break;
    case 2:
        status = ELS_BUSY;
        break;
    case 3:
        status = ELS_LEAVE;
        break;
    case 4:
        status = ELS_NOTDISTURB;
        break;
    default:
        status = ELS_ONLINE;
        break;
    }

    if (nullptr != m_mainCtrl)
        m_mainCtrl->changeMyStatus(m_myself.m_userID,status);
}



/*************************************************
Function Name： receiveFriendRequest
Description: 接收并且处理好友的请求
*************************************************/
void TMainWidget::receiveFriendRequest(const TTalkMessage & mes,
                                        const TUserInformation & friendInf)
{
    if (0 != mes.m_receiverID.compare(m_myself.m_userID))
        return;
    TTalkMessage returnMes;

    switch (mes.m_type)
    {
    case REQUEST_FRIEND:
    {
        QMessageBox::StandardButton rb = QMessageBox::question(
                    nullptr, tr("好友请求"),
                    QString(tr("是否同意用户%1(%2)添加你为好友?\n验证消息:\n%3")).
                    arg(mes.m_senderID, friendInf.m_nickname, mes.m_text),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            returnMes.m_type = AGREE_FRIEND;
        }
        else
        {
            returnMes.m_type = REFUSE_FRIEND;
        }
        returnMes.m_senderID = mes.m_receiverID;
        returnMes.m_receiverID = mes.m_senderID;
        if (nullptr != m_mainCtrl)
            m_mainCtrl->resultOfFriendRequest(returnMes);
        break;
    }
    case AGREE_FRIEND:
    {
        TFriendInformation fri;
        fri.m_headPortrait = friendInf.m_headPortrait;
        fri.m_userID = friendInf.m_userID;
        fri.m_nickname = friendInf.m_nickname;
//        fri.m_groupName = tr("New Friends");
        fri.m_status = friendInf.m_status;
        if (!addFriendButton(fri))
            return;
        QString temp = QString(tr("成功添加好友%1(%2).")).
                arg(friendInf.m_userID, friendInf.m_nickname);
        QMessageBox::information(nullptr, tr("消息"), temp);
        break;
    }
    case REFUSE_FRIEND:
    {
        QString temp = QString(tr("用户%1(%2)\n拒绝您的好友添加请求.")).
                arg(friendInf.m_userID, friendInf.m_nickname);
        QMessageBox::information(nullptr, tr("消息"), temp);
        break;
    }
    case DELETE_FRIEND:
    {
        break;
    }
    }
}

/*************************************************
Function Name： receiveFlockRequest
Description: 接收并且处理群的请求
*************************************************/
void TMainWidget::receiveFlockRequest(const TTalkMessage & mes,
                                       const TFlockMember & member,
                                       const TFlockInformation &flock)
{
    TTalkMessage returnMes;
    switch (mes.m_type)
    {
    case REQUEST_FLOCK:
    {
        QMessageBox::StandardButton rb = QMessageBox::question(
                    nullptr, tr("用户请求"),
                    QString(tr("是否同意用户%1加入群%2?\n验证消息:\n%3")).
                    arg(mes.m_senderID, mes.m_receiverID, mes.m_text),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if(rb == QMessageBox::Yes)
        {
            returnMes.m_type = FLOCK_AGREE;
        }
        else
        {
            returnMes.m_type = FLOCK_REFUSE;
        }
        returnMes.m_senderID = mes.m_receiverID; // 群
        returnMes.m_receiverID = mes.m_senderID; // 用户
        if (nullptr != m_mainCtrl)
            m_mainCtrl->resultOfFlockRequest(returnMes);
        break;
    }

    case AGREE_FLOCK:
    {
        if (!m_flockMap.contains(mes.m_receiverID))
            return;
        if (!m_flockRoomMap.contains(mes.m_receiverID))
            return;
        if (!m_flockRoomMap[mes.m_receiverID]->addFlockMemberButton(member))
            return;

        m_flockMap[mes.m_receiverID]->addFlockMember(member.m_userID);

        QString temp = QString(tr("用户%1,成功加入群%2(%3).")).
                arg(mes.m_senderID, flock.m_flockID, flock.m_flockName);
        QMessageBox::information(nullptr, tr("群消息"), temp);
//        out << temp.m_message << m_save.m_flockMember;
        break;
    }
    case REFUSE_FLOCK:
    {
        QString temp = QString(tr("您好,%1,用户%2,拒绝加入群%3\n的群.")).
                arg(m_myself.m_userID, mes.m_senderID, mes.m_receiverID);
        QMessageBox::information(nullptr, tr("群消息"), temp);
        break;
    }
    case FLOCK_AGREE:
    {
        if (m_flockMap.contains(flock.m_flockID))
            return;
        if (!addFlockButton(flock))
            return;
        QString temp = QString(tr("成功加入群%1(%2).")).
                arg(flock.m_flockID, flock.m_flockName);
        QMessageBox::information(nullptr, tr("群消息"), temp);

        break;
    }
    case FLOCK_REFUSE:
    {
        QString temp = QString(tr("您好,%1,群%2\n的群主拒绝您的加入请求.")).
                arg(m_myself.m_userID, mes.m_senderID);
        QMessageBox::information(nullptr, tr("群消息"), temp);
        break;
    }
    case FLOCK_REQUEST:
    {
        QMessageBox::StandardButton rb = QMessageBox::question(
                    nullptr, tr("群请求"),
                    QString(tr("是否同意加入群%1?\n验证消息:\n%2")).
                    arg(mes.m_senderID, mes.m_text),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if(rb == QMessageBox::Yes)
        {
            returnMes.m_type = AGREE_FLOCK;
        }
        else
        {
            returnMes.m_type = REFUSE_FLOCK;
        }
        returnMes.m_senderID = mes.m_receiverID; // 用户
        returnMes.m_receiverID = mes.m_senderID; // 群
        if (nullptr != m_mainCtrl)
            m_mainCtrl->resultOfFlockRequest(returnMes);
        break;
    }
    case NEW_FLOCK_MEMBER:
    {
        if (!m_flockMap.contains(flock.m_flockID))
            return;
        if (!m_flockRoomMap.contains(flock.m_flockID))
            return;
        if (!m_flockRoomMap[flock.m_flockID]->addFlockMemberButton(member))
            return;

        m_flockMap[flock.m_flockID]->addFlockMember(member.m_userID);
        break;
    }
    default:
        break;
    }
}


/*************************************************
Function Name： receiveDiscussionRequest
Description: 接收并处理讨论组请求
*************************************************/
void TMainWidget::receiveDiscussionRequest(const TTalkMessage & mes,
                                            const TDiscussionMember & member,
                                            const TDiscussionInformation & discussion)
{
    switch (mes.m_type)
    {
    case ADD_DISCUSSION_FAIL:
    {
        QString temp = QString(tr("您好,%1,用户%2加入讨论组%3失败.")).
                arg(m_myself.m_userID, mes.m_receiverID, mes.m_senderID);
        QMessageBox::information(nullptr, tr("讨论组消息"), temp);
        break;
    }

    case NEW_DISCUSSION_MEMBER:
    {
        if (!m_discussionMap.contains(mes.m_senderID))
            return;
        if (!m_discussionRoomMap.contains(mes.m_senderID))
            return;
        if (!m_discussionRoomMap[mes.m_senderID]->addDiscussionMemberButton(member))
            return;

        m_discussionMap[mes.m_senderID]->addDiscussionMember(member.m_userID);

        break;
    }
    case ADD_DISCUSSION_SUCCESS:
    {
        if (m_discussionMap.contains(mes.m_senderID))
            return;
        if (!addDiscussionButton(discussion))
            return;
        break;
    }
    default:
        break;
    }
}


// 删除组
/*************************************************
Function Name： removeBox
Description: 删除组
*************************************************/
void TMainWidget::removeBox(const QString & title)
{
    QMessageBox::StandardButton returnBtn;

    returnBtn = QMessageBox::question(nullptr, tr("删除分组"),
        QString(tr("你确定要删除分组(%1)吗？")).arg(title),
        QMessageBox::Yes | QMessageBox::No,  QMessageBox::No);

    if(QMessageBox::No == returnBtn)
        return;

    int index = m_indexFriendsGroupMap[title];

    if (m_toolItemsFriendsVec[index]->getSize() > 0)
    {
        QMessageBox::information(nullptr, tr("删除组"), tr("无法删除，因为该组里还有组员！"));
        return;
    }

    m_toolItemsFriendsVec[index]->deleteLater();
    m_toolItemsFriendsVec[index] = nullptr;
    m_friendsGroupList.removeAt(m_friendsGroupList.indexOf(title));
    // 删除映射
    m_indexFriendsGroupMap.remove(title);
    if (m_messageManWidget != nullptr)
        m_messageManWidget->setListModelGroup();
    //    if (m_mainWidget == nullptr)
//        return;
//    m_mainWidget->dropFlock(m_flockInfo.m_flockID);
//    m_mainCtrl->removeBox(title);
    refreshFriendButtonMenu();
}

/*************************************************
Function Name： renameBox
Description: 重命名组
*************************************************/
void TMainWidget::renameBox(const QString & title)
{
    bool isOk = false;
    QString newTitle = QInputDialog::getText(nullptr, "重命名分组",
                                           "请输入新的分组名",
                                           QLineEdit::Normal,
                                           title,
                                           &isOk);
    if (!isOk)
        return;

    if (m_indexFriendsGroupMap.contains(newTitle))
    {
        QMessageBox::information(nullptr, tr("重命名分组"), tr("失败，因为存在同名的组！"));
        return;
    }

    int index = m_indexFriendsGroupMap.value(title);

    if (m_toolItemsFriendsVec[index]->getSize() <= 0)
    {
        renameBoxSuccess(title, newTitle);
        refreshFriendButtonMenu();
        return;
    }

    if (0 != newTitle.compare(title))
    {
        m_mainCtrl->renameBox(title, newTitle);
    }
}


// mark: private:--------------------------------------------------
/*************************************************
Function Name： initIMMainWidget
Description: 初始化
*************************************************/
void TMainWidget::initIMMainWidget()
{
    m_mainCtrl = new IMMainCtrl(m_myself.m_userID);

    m_labelHead = new TClickLabel(this);
    /*
border: 2px solid green;
     border-radius: 4px;
     padding: 2px;
     background-image: url(images/welcome.png);
     */
    qDebug() << "m_headPortrait: " << m_myself.m_headPortrait;
    m_labelHead->setFixedSize(HEAD_BIG_SIZE,HEAD_BIG_SIZE);
//    m_frameHead->setObjectName("headFrame");
//    QString str = QString("QFrame#headFrame{border-image:url(resource/image/head/%1.bmp);}")
//            .arg(QString::number(m_myself.m_headPortrait));
//    m_frameHead->setStyleSheet(str);
    QString str = QString("resource/image/head/%1.bmp").
            arg(QString::number(m_myself.m_headPortrait));
    m_labelHead->setPixmap(QPixmap(str));
    m_cbStatus = new QComboBox(this);
//    m_cbStatus->setSizeAdjustPolicy(QComboBox::AdjustToContents);
//    m_cbStatus->setStyleSheet("QComboBox{border: 0;font-size: 16px;}");
//                              "QComboBox::drop-down{border-style:none;border:0px;width:40px}");

    /*
    ELS_ONLINE = 0,    // 在线
    INVISIBLE = 1, // 隐身
    ELS_BUSY = 2,      // 忙碌
    ELS_LEAVE = 3,     // 离开
    ELS_NOTDISTURB = 4,// 请勿打扰
    */
    m_cbStatus->addItem("在线");
    m_cbStatus->addItem("隐身");
    m_cbStatus->addItem("忙碌");
    m_cbStatus->addItem("离开");
    m_cbStatus->addItem("请勿打扰");
    m_cbStatus->setCurrentIndex(m_myself.m_status);

    // add item
    m_labelNickname = new QLabel(QString("%1\n%2")
                                 .arg(m_myself.m_nickname, m_myself.m_userID));
    m_labelMark = new QLabel(tr("我的签名")); // 说说 签名

    m_toolBtnSpace = new QToolButton(this);  // 空间
    m_toolBtnSpace->setIcon(QIcon(":/images/space.png"));
    m_toolBtnSpace->setAutoRaise(true);
    m_toolBtnSpace->setToolTip(tr("空间"));
    m_toolBtnSpace->setStatusTip(tr("打开空间"));

//    m_toolBtnSpace->setAutoFillBackground(true);
    m_toolBtnMail = new QToolButton(this);   // 邮件
    m_toolBtnMail->setIcon(QIcon(":/images/mail.png"));
    m_toolBtnMail->setAutoRaise(true);
    m_toolBtnMail->setToolTip(tr("邮箱"));
    m_toolBtnMail->setStatusTip(tr("打开邮箱"));

    m_toolBtnMesBox = new QToolButton(this); // 消息盒子
    m_toolBtnMesBox->setIcon(QIcon(":/images/messageBox.png"));
    m_toolBtnMesBox->setAutoRaise(true);
    m_toolBtnMesBox->setToolTip(tr("消息管理"));
    m_toolBtnMesBox->setStatusTip(tr("打开消息管理器"));

    m_toolBtnNewMes = new QToolButton(this);
    m_toolBtnNewMes->setIcon(QIcon(""));//:/images/noMessage.png"));
    m_toolBtnNewMes->setAutoRaise(true);
    m_toolBtnNewMes->setToolTip(tr("无新消息"));
    m_toolBtnNewMes->setStatusTip(tr("无最新消息"));
    m_toolBtnNewMes->setHidden(true);

    QLabel *searchLabel = new QLabel(this);
    searchLabel->setStyleSheet("border: 1px;\
                               border-radius:3px;\
                               border-style: solid;\
                               border-color: gray;\
                               background: white;");
    searchLabel->setFixedHeight(25);

    m_leSearch = new QLineEdit();
    m_leSearch->setStyleSheet("border: 0px;");
    m_leSearch->setPlaceholderText(tr("search friends"));

    m_toolBtnSearch = new QToolButton();
    m_toolBtnSearch->setIcon(QIcon(":/images/search.png"));
    m_toolBtnSearch->setStyleSheet("border: 0px;");
    m_toolBtnSearch->setEnabled(false);
//    m_labelSearch = new QLabel(this);
//    m_labelSearch->setStyleSheet("border: 0px;");

//    QPixmap pixmap;
//    QIcon icon(QString(":/images/search.png"));
//    pixmap = icon.pixmap(QSize(CHAT_WIDGET_TOOL_SMALL_SIZE,
//                               CHAT_WIDGET_TOOL_SMALL_SIZE));
//    m_labelSearch->setPixmap(pixmap);

    QHBoxLayout *layoutTopSearch = new QHBoxLayout;
    layoutTopSearch->setContentsMargins(0, 0, 0, 0);
    layoutTopSearch->addWidget(m_leSearch);
    layoutTopSearch->addWidget(m_toolBtnSearch);
//    layoutTopSearch->addWidget(m_labelSearch);
//    layoutTopSearch->addSpacing(2);
    searchLabel->setLayout(layoutTopSearch);

//    m_toolBtnSearch = new QToolButton();
//    m_toolBtnSearch->setIcon(QIcon(":/images/search.png"));
//    m_toolBtnSearch->setAutoRaise(true);
//    m_toolBtnSearch->setToolTip(tr("搜索好友"));
//    m_toolBtnSearch->setStatusTip(tr("搜索好友"));

//    m_toolBtnSearch->adjustSize();
//    m_toolBtnSearch->setCursor(Qt::ArrowCursor);
//    m_toolBtnSearch ->setStyleSheet("background:transparent;");

    m_searchListWidget = new TSearchListWidget;
    m_searchListWidget->setVisible(false);

    m_tabWidget= new QTabWidget;
//    m_tabWidget->setTabShape(QTabWidget::Triangular);
    m_friendListWidget = new TFriendListWidget(this, this);
    m_flockListWidget = new TFlockListWidget(this, this);
    m_discussionListWidget = new TDiscussionListWidget(this, this);
    m_messageListWidget = new TLatestMessageListWidget(this, this);
    m_tabWidget->addTab(m_friendListWidget, tr("联系人"));
    m_tabWidget->addTab(m_flockListWidget,tr("IM群"));
    m_tabWidget->addTab(m_discussionListWidget, tr("讨论组"));
    m_tabWidget->addTab(m_messageListWidget, tr("新消息(0)"));

    m_labelOnlineNum = new QLabel("在线人数: 0");
    m_labelOnlineNum->setHidden(true);

    m_toolBtnAddFlock = new QToolButton;
    m_toolBtnAddFlock->setIcon(QIcon(":/images/addFlock.png"));
    m_toolBtnAddFlock->setAutoRaise(true);
    m_toolBtnAddFlock->setToolTip(tr("加群"));
    m_toolBtnAddFlock->setStatusTip(tr("添加群"));

//    m_toolBtnAddDiscussion = new QToolButton;
//    m_toolBtnAddDiscussion->setIcon(QIcon(":/images/addDiscussion.png"));
//    m_toolBtnAddDiscussion->setAutoRaise(true);
//    m_toolBtnAddDiscussion->setToolTip(tr(""));
//    m_toolBtnAddDiscussion->setStatusTip(tr(""));

//    m_toolBtnSearchAdd = new QToolButton;
//    m_toolBtnSearchAdd->setIcon(QIcon(":/images/searchAdd.png"));
//    m_toolBtnSearchAdd->setAutoRaise(true);
//    m_toolBtnSearchAdd->setToolTip(tr(""));
//    m_toolBtnSearchAdd->setStatusTip(tr(""));

    m_toolBtnConfig = new QToolButton;
    m_toolBtnConfig->setIcon(QIcon(":/images/config.png"));
    m_toolBtnConfig->setAutoRaise(true);
    m_toolBtnConfig->setToolTip(tr("系统管理"));
    m_toolBtnConfig->setStatusTip(tr("打开系统管理"));

    m_toolBtnAddFriend = new QToolButton;
    m_toolBtnAddFriend->setIcon(QIcon(":/images/addFriend.png"));
    m_toolBtnAddFriend->setAutoRaise(true);
    m_toolBtnAddFriend->setToolTip(tr("加好友"));
    m_toolBtnAddFriend->setStatusTip(tr("添加好友"));

    /****************头部*********/
    QVBoxLayout *layoutTopRight = new QVBoxLayout;
//    layoutTopRight1->addWidget(m_cbStatus);
    layoutTopRight->addWidget(m_labelNickname);
//    layoutTopRight->addWidget(m_labelMark);

    QHBoxLayout *layoutTop = new QHBoxLayout;
    layoutTop->addWidget(m_labelHead);
    layoutTop->addLayout(layoutTopRight);

    QHBoxLayout *layoutTopTool = new QHBoxLayout;
    layoutTopTool->addWidget(m_cbStatus);
    layoutTopTool->addWidget(m_toolBtnSpace);
    layoutTopTool->addWidget(m_toolBtnMail);
    layoutTopTool->addWidget(m_toolBtnMesBox);
    layoutTopTool->addWidget(m_toolBtnNewMes);
    layoutTopTool->addStretch();

//    QHBoxLayout *layoutTopSearch = new QHBoxLayout;
//    layoutTopSearch->addWidget(m_leSearch);
////    layoutTopSearch->addWidget(m_toolBtnSearch);

    QVBoxLayout *layoutHead = new QVBoxLayout;
    layoutHead->addLayout(layoutTop);
    layoutHead->addLayout(layoutTopTool);
    layoutHead->addWidget(searchLabel);
//    layoutHead->addLayout(layoutTopSearch);
//    layoutTop->addStretch();
    /***********ennd 头部************/

    QHBoxLayout *layoutBtm = new QHBoxLayout;
    layoutBtm->addWidget(m_labelOnlineNum);
    layoutBtm->addStretch();
    layoutBtm->addWidget(m_toolBtnAddFlock);
//    layoutBtm->addWidget(m_toolBtnAddDiscussion);
    layoutBtm->addWidget(m_toolBtnAddFriend);
//    layoutBtm->addWidget(m_toolBtnSearchAdd);
    layoutBtm->addWidget(m_toolBtnConfig);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(layoutHead);
    layout->addWidget(m_tabWidget);
    layout->addWidget(m_searchListWidget);
    layout->addLayout(layoutBtm);

    setLayout(layout);

    setMinimumSize(278, 300);
    resize(278,500);

}

/*************************************************
Function Name： linkSignalWithSlot
Description:  连接信号与槽
*************************************************/
void TMainWidget::linkSignalWithSlot()
{
    connect(m_mainCtrl, SIGNAL(getFriendsSuccess(QVector<TFriendInformation>)),
            this, SLOT(setFriendListWidget(QVector<TFriendInformation>)));
    connect(m_mainCtrl, SIGNAL(getFlocksSuccess(QVector<TFlockInformation>)),
            this, SLOT(setFlockListWidget(QVector<TFlockInformation>)));
    connect(m_mainCtrl, SIGNAL(getDiscussionsSuccess(QVector<TDiscussionInformation>)),
            this, SLOT(setDiscussionListWidget(QVector<TDiscussionInformation>)));

    connect(m_mainCtrl, SIGNAL(getFriendTalkMessage(TTalkMessage)),
            this, SLOT(receiveFriendTalkMessage(TTalkMessage)));

    connect(m_mainCtrl, SIGNAL(getFriendChangedStatus(QString,int)),
            this, SLOT(setFriendStatus(QString,int)));
    connect(m_mainCtrl, SIGNAL(getFriendChangedInformation(TFriendInformation)),
            this, SLOT(setFriendInformation(TFriendInformation)));
//    connect(m_friendsList, SIGNA.L(itemDoubleClicked(QListWidgetItem *)),
//            this, SLOT(createChatRoom(QListWidgetItem *)));
    connect(m_mainCtrl, SIGNAL(getFriendInformationSuccess(TUserInformation)),
            this, SLOT(showFriendInformation(TUserInformation)));

    connect(m_labelHead, SIGNAL(clicked()),
            this, SLOT(getMyInformation()));

    connect(m_mainCtrl, SIGNAL(getMyInformationSuccess(TUserInformation)),
            this, SLOT(showMyInformation(TUserInformation)));

    connect(m_mainCtrl, SIGNAL(changeMyInformationSuccess(TUserInformation)),
            this, SLOT(setMyInformation(TUserInformation)));

    connect(m_cbStatus, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeMyStatus(int)));
    connect(m_mainCtrl, SIGNAL(changeFriendRemarkSuccess(TTempStrings)),
            this, SLOT(setFriendRemark(TTempStrings)));
    connect(m_mainCtrl, SIGNAL(deleteFriendSuccess(QString, QString, QString)),
            this, SLOT(deletedFriend(QString, QString, QString)));

    connect(m_mainCtrl, SIGNAL(getFriendRequest(TTalkMessage, TUserInformation)),
            this, SLOT(receiveFriendRequest(TTalkMessage, TUserInformation)));


    connect(m_mainCtrl, SIGNAL(getFlockRequest(TTalkMessage, TFlockMember, TFlockInformation)),
            this, SLOT(receiveFlockRequest(TTalkMessage, TFlockMember, TFlockInformation)));
    connect(m_mainCtrl, SIGNAL(dropFlockSuccess(QString)),
            this, SLOT(dropedFlock(QString)));
    connect(m_mainCtrl, SIGNAL(leaveFlockSuccess(QString,QString,QString)),
            this, SLOT(leavedFlock(QString,QString,QString)));
    connect(m_mainCtrl, SIGNAL(haveMemberleavedFlock(QString,QString,QString)),
            this, SLOT(leavedFlock(QString,QString,QString)));
    connect(m_mainCtrl, SIGNAL(changeFlockSuccess(TFlockInformation)),
            this, SLOT(setFlockInformation(TFlockInformation)));
    connect(m_mainCtrl, SIGNAL(changeFlockRemarkSuccess(TTempStrings)),
            this, SLOT(setFlockMemberRemark(TTempStrings)));
    connect(m_mainCtrl, SIGNAL(createFlockSuccess(TFlockInformation)),
            this, SLOT(createFlockSuccess(TFlockInformation)));


    connect(m_mainCtrl, SIGNAL(getDiscussionRequest(TTalkMessage,TDiscussionMember,TDiscussionInformation)),
            this, SLOT(receiveDiscussionRequest(TTalkMessage,TDiscussionMember,TDiscussionInformation)));
    connect(m_mainCtrl, SIGNAL(dropDiscussionSuccess(QString)),
            this, SLOT(dropedDiscussion(QString)));
    connect(m_mainCtrl, SIGNAL(leaveDiscussionSuccess(QString,QString)),
            this, SLOT(leavedDiscussion(QString,QString)));
    connect(m_mainCtrl, SIGNAL(changeDiscussionSuccess(TDiscussionInformation)),
            this, SLOT(setDiscussionInformation(TDiscussionInformation)));
    connect(m_mainCtrl, SIGNAL(createDiscussionSuccess(TDiscussionInformation)),
            this, SLOT(createDiscussionSuccess(TDiscussionInformation)));

    connect(m_mainCtrl, SIGNAL(closeWindowSignal()),
            this, SLOT(closeWindow()));

    connect(m_leSearch, SIGNAL(textChanged(QString)),
            this, SLOT(searchFriend(QString)));
    connect(m_toolBtnSearch, SIGNAL(clicked()),
           this, SLOT(onClickSearchFriend()));

    connect(m_toolBtnAddFriend, SIGNAL(clicked()),
            this, SLOT(onClickAddFriend()));
    connect(m_toolBtnAddFlock, SIGNAL(clicked()),
            this, SLOT(onClickAddFlock()));
//    connect(m_toolBtnAddDiscussion, SIGNAL(clicked()),
//            this, SLOT(addDiscussion()));
    connect(m_toolBtnMail, SIGNAL(clicked()),
            this, SLOT(showMailWidget()));
    connect(m_toolBtnMesBox, SIGNAL(clicked()),
            this, SLOT(showMessageWidget()));
    connect(m_toolBtnConfig, SIGNAL(clicked()),
            this, SLOT(onClickRightButton()));
    connect(m_toolBtnNewMes, SIGNAL(clicked()),
            this, SLOT(showLatestMessageWidget()));

    connect(m_mainCtrl, SIGNAL(renameBoxSuccess(QString,QString)),
            this, SLOT(renameBoxSuccess(QString, QString)));
    connect(m_mainCtrl, SIGNAL(moveFriendToBoxSuccess(QString,QString,QString)),
            this, SLOT(moveFriendToBoxSuccess(QString,QString,QString)));

    connect(m_mainCtrl, SIGNAL(getNetworkHistoryMessagesSuccess(QString,QDate,QVector<TTalkMessage>)),
            this, SLOT(setNetworkMessageWidget(QString,QDate,QVector<TTalkMessage>)));

    connect(m_messageListWidget, SIGNAL(timerStatus(bool)),
            this, SLOT(setTimerStatus(bool)));
}


/*************************************************
Function Name： clearContainers
Description:  清理容器
*************************************************/
void TMainWidget::clearContainers()
{
    m_friendsGroupList.clear();

    m_indexFriendsGroupMap.clear();

    qDeleteAll(m_friendMap.begin(), m_friendMap.end());
    m_friendMap.clear();

    qDeleteAll(m_toolItemsFriendsVec.begin(), m_toolItemsFriendsVec.end());
    m_toolItemsFriendsVec.clear();


    qDeleteAll(m_flockMap.begin(), m_flockMap.end());
    m_flockMap.clear();

    qDeleteAll(m_discussionMap.begin(), m_discussionMap.end());
    m_discussionMap.clear();

    qDeleteAll(m_linkMap.begin(), m_linkMap.end());
    m_linkMap.clear();

}

/*************************************************
Function Name： creatMenu
Description: 创建右键菜单
*************************************************/
void TMainWidget:: creatMenu()
{
    m_menu = new QMenu(this);
    QAction *addFriend = new QAction(tr("添加好友"), m_menu);
    QAction *addFlock = new QAction(tr("添加群"), m_menu);
    QAction *changePwd = new QAction(tr("修改密码"), m_menu);
    QAction *information = new QAction(tr("个人信息"), m_menu);
    QAction *manMes = new QAction(tr("消息管理"), m_menu);
    QAction *mail = new QAction(tr("邮件管理"), m_menu);

    connect(addFriend, SIGNAL(triggered()),
            this, SLOT(onClickAddFriend()));
    connect(addFlock, SIGNAL(triggered()),
            this, SLOT(onClickAddFlock()));
    connect(changePwd, SIGNAL(triggered()),
            this, SLOT(onClickChangePwd()));
    connect(information, SIGNAL(triggered()),
            this, SLOT(getMyInformation()));
    connect(manMes, SIGNAL(triggered()),
            this, SLOT(showMessageWidget()));
    connect(mail, SIGNAL(triggered()),
            this, SLOT(showMailWidget()));


    m_menu->addAction(addFlock);
    m_menu->addAction(addFriend);
    m_menu->addAction(changePwd);
    m_menu->addAction(information);
    m_menu->addAction(manMes);
    m_menu->addAction(mail);
}


/*************************************************
Function Name： refreshFriendButtonMenu
Description: 刷新好友按钮右键菜单
*************************************************/
void TMainWidget::refreshFriendButtonMenu()
{
    QMapIterator<QString, TFriendButton *> iter(m_friendMap);
     while (iter.hasNext()) {
         iter.next();
         iter.value()->refreshMoveMenu();
     }
}


/*************************************************
Function Name： refreshFriendButtonMenu
Description: 刷新好友按钮右键菜单
*************************************************/
bool TMainWidget::continueToClose()
{
    if (m_flag == 1)
    {
        return true;
    }
    //如果还有聊天窗口没有关闭，就弹出信息窗口
    if (m_chatRoomMap.size() != 0
            || m_flockRoomMap.size() != 0
            || m_discussionRoomMap.size() != 0)
    {
        QString temp = tr("还有聊天窗口没有关闭，您确定想要退出程序？");
        QMessageBox::StandardButton rb;
        rb = QMessageBox::question(this, tr("退出程序"), temp,
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(rb == QMessageBox::Yes)
                return true;
        else
            return false;
     }
    return true;
}

#ifndef IMDISCUSSIONBUTTON_H
#define IMDISCUSSIONBUTTON_H

#include "stable.h"
#include "TConstant.h"

class QMenu;
class QLabel;
class TMainWidget;
class TDiscussionChatWidget;
class IMMainCtrl;

class TDiscussionButton : public QPushButton
{
    Q_OBJECT
public:
    TDiscussionButton(const TDiscussionInformation &info, TMainWidget *mainWidget = 0,
                                QWidget *parent = 0);
    ~TDiscussionButton();

    bool isRoomOpen() const
    {
        return m_isOpen;
    }
    bool isRoomShow() const
    {
        return m_isShow;
    }
    TDiscussionChatWidget *getRoomPoint() const
    {
        return m_discussionRoom;
    }
    TDiscussionInformation &getInformation()
    {	return m_discussionInfo;	}
    TDiscussionInformation getInformation() const
    {	return m_discussionInfo;	}
    void setChangedInformation(const TDiscussionInformation & discussion)
    {
        if (m_discussionInfo.m_discussionID != discussion.m_discussionID)
            return;

        m_discussionInfo.m_creatorID = discussion.m_creatorID;
        m_discussionInfo.m_createDate = discussion.m_createDate;
        m_discussionInfo.m_discussionName = discussion.m_discussionName;
        m_discussionInfo.m_theme = discussion.m_theme;
        setButtonStatus();
    }
    // 打开聊天窗口
    void openChatRoom();
    // 显示聊天窗口
    void showChatRoom();
    // 关闭聊天窗口
    void closeChatRoom();

    // 设置button显示信息
    void setButtonStatus();

    // 删除成员
    void deleteDiscussionMember(const QString & memberID);
    // 添加成员
    void addDiscussionMember(const QString & memberID);

signals:
    void leaveDiscussionSignal(const QString &);

public slots:
    // 左键 双击
    void onDoubleClickDiscussionButton();
    // 右键点击按键
    void onClickRightButton();
    // 发送讨论组消息
    void onClickSendMessage();
    // 查看消息记录
    void onClickShowRecord();
    // 解散讨论组
    void onClickDropDiscussion();
    // 退出讨论组
    void onClickLeaveDiscussion();
    // 添加讨论组成员
    void onClickAddMember();
    // 显示详细信息
    void onClickShowInformation();
    //聊天窗口关闭了
    void chatRoomQuit();

    // 设置成员列表
    void setDiscussionMemberList(const QVector<TDiscussionMember> &);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    // 创建右键菜单
    void creatMenu(/*const QList<QString> *groutList*/);

private:
    TDiscussionInformation m_discussionInfo;

    // 聊天窗口是否打开了
    bool m_isOpen;
    // 聊天窗口是否显示
    bool m_isShow;
    // 右键菜单
    QMenu *m_menu;
    TDiscussionChatWidget *m_discussionRoom;
    TMainWidget *m_mainWidget;
    IMMainCtrl *m_mainCtrl;
    QVector<TDiscussionMember> m_discussionMembersVec;

    QLabel *m_labelHead;
//    QIcon *m_iconHead;
    QLabel *m_labelMes;
};

#endif // IMDISCUSSIONBUTTON_H

#ifndef IMFLOCKBUTTON_H
#define IMFLOCKBUTTON_H

#include <QPushButton>
#include "TConstant.h"
#include "stable.h"

class QMenu;
class QLabel;
class TMainWidget;
class TFlockChatWidget;
class IMMainCtrl;

class TFlockButton : public QPushButton
{
    Q_OBJECT
public:
    TFlockButton(const TFlockInformation &info, TMainWidget *mainWidget,
                  QWidget *parent = 0);
    ~TFlockButton();
    
    bool isRoomOpen() const
    {
        return m_isOpen;
    }
    bool isRoomShow() const
    {
        return m_isShow;
    }
    TFlockChatWidget *getRoomPoint() const
    {
        return m_flockRoom;
    }
    TFlockInformation &getInformation()
    {	return m_flockInfo;	}
    TFlockInformation getInformation() const
    {	return m_flockInfo;	}
    void setChangedInformation(const TFlockInformation & flock)
    {
        if (m_flockInfo.m_flockID != flock.m_flockID)
            return;

//        m_flockInfo.m_creatorID = flock.m_creatorID;
//        m_flockInfo.m_createDate = flock.m_createDate;
        m_flockInfo.m_flockName = flock.m_flockName;
        m_flockInfo.m_theme = flock.m_theme;
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

    // 删除群成员
    void deleteFlockMember(const QString & memberID);
    // 添加群成员
    void addFlockMember(const QString & memberID);

signals:
    void leaveFlockSignal(const QString &);

public slots:
    // 左键 双击
    void onDoubleClickFlockButton();
    // 右键点击按键
    void onClickRightButton();
    // 发送群消息
    void onClickSendMessage();
    // 查看消息记录
    void onClickShowRecord();
    // 显示群详细信息
    void onClickShowInformation();
    // 解散群
    void onClickDropFlock();
    // 退出群
    void onClickLeaveFlock();
    // 添加群成员
    void onClickAddMemberFlock();
    // 进入空间
    void onClickEnterSpace();
    //聊天窗口关闭了
    void chatRoomQuit();

    // 设置成员列表
    void setFlockMemberList(const QVector<TFlockMember> &);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    // 创建右键菜单
    void creatMenu(/*const QList<QString> *groutList*/);

private:
    TFlockInformation m_flockInfo;

    // 聊天窗口是否打开了
    bool m_isOpen;
    // 聊天窗口是否显示
    bool m_isShow;
    // 右键菜单
    QMenu *m_menu;
//    TFlockChatWidget *m_chatRoom;
    TMainWidget *m_mainWidget;
    IMMainCtrl *m_mainCtrl;

    QLabel *m_labelHead;
//    QIcon *m_iconHead;
    QLabel *m_labelMes;
    TFlockChatWidget *m_flockRoom;
    QVector<TFlockMember> m_flockMembersVec;
};

#endif // IMFLOCKBUTTON_H

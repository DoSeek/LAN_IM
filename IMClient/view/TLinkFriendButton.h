#ifndef IMLINKFRIENDBUTTON_H
#define IMLINKFRIENDBUTTON_H


#include "TLinkButton.h"
#include "TConstant.h"

class QLabel;
class TFriendButton;

class IMLinkFriendButton : public TLinkButton
{
    Q_OBJECT
public:
    IMLinkFriendButton(TFriendButton * btn, QWidget *parent = 0);
    ~IMLinkFriendButton();


    // 设置button显示信息
    void setButtonStatus();

public slots:
    // 左键 双击
    void onDoubleClickButton();
    // 忽略消息
    void ignoreMessage();


private:
    TFriendButton * m_friendBtn;
    TFriendInformation m_info;

};

#endif // IMLinkFriendButton_H

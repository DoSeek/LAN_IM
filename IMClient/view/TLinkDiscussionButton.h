#ifndef IMLINKDISCUSSIONBUTTON_H
#define IMLINKDISCUSSIONBUTTON_H

#include "TLinkButton.h"
#include "TConstant.h"

class QLabel;
class TDiscussionButton;

class IMLinkDiscussionButton : public TLinkButton
{
    Q_OBJECT
public:
    IMLinkDiscussionButton(TDiscussionButton * btn, QWidget *parent = 0);
    ~IMLinkDiscussionButton();

    // 设置button显示信息
    void setButtonStatus();

signals:
    
public slots:
    // 左键 双击
    void onDoubleClickButton();
    // 忽略消息
    void ignoreMessage();

private:
    TDiscussionButton *m_discussionBtn;
    TDiscussionInformation m_info;
    
};

#endif // IMLINKDISCUSSIONBUTTON_H

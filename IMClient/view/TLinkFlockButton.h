#ifndef IMLINKFLOCKBUTTON_H
#define IMLINKFLOCKBUTTON_H

#include "stable.h"
#include "TLinkButton.h"
#include "TConstant.h"

class TFlockButton;

class IMLinkFlockButton : public TLinkButton
{
    Q_OBJECT
public:
    IMLinkFlockButton(TFlockButton * btn, QWidget *parent = 0);
    ~IMLinkFlockButton();

    // 设置button显示信息
    void setButtonStatus();

signals:
    
public slots:
    // 左键 双击
    void onDoubleClickButton();
    // 忽略消息
    void ignoreMessage();

private:
    TFlockButton *m_flockBtn;
    TFlockInformation m_info;

};

#endif // IMLINKFLOCKBUTTON_H

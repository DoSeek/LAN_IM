#ifndef IMMAILBUTTON_H
#define IMMAILBUTTON_H

#include <QPushButton>
#include "TConstant.h"

class QCheckBox;
class QLabel;
class TMailWidget;
class IMMailCtrl;

class IMMailButton : public QPushButton
{
    Q_OBJECT
public:
    IMMailButton(const TMailInformation &, TMailWidget *, QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);

signals:
    
public slots:

private slots:
    // 点击删除邮件
    void onClickDeleteMail();
    // 左键 双击
    void onDoubleClickDiscussionButton();
    // 右键点击按键
    void onClickRightButton();

private:
    // 设置button显示信息
    void setButtonStatus();
    // 创建右键菜单
    void creatMenu();

private:
//    QCheckBox m_cbSelected;
    QLabel *m_labelStatus;
    QLabel *m_labelSender;
    QLabel *m_labelTheme;
    QLabel *m_labelTime;
    QMenu *m_menu;
    TMailWidget *m_mailWidget;
    IMMailCtrl *m_mailCtrl;
    TMailInformation m_mail;
};

#endif // IMMAILBUTTON_H

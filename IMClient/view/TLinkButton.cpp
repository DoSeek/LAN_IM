#include "TLinkButton.h"


TLinkButton::TLinkButton(QWidget *parent) :
    QPushButton(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_labelHead = new QLabel;
    m_labelHead->setFixedSize(HEAD_SMALL_SIZE, HEAD_SMALL_SIZE);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setContentsMargins(0,0,0,0);
    m_labelInfo = new QLabel;
    m_labelMes = new QLabel;
    rightLayout->addWidget(m_labelInfo);
    rightLayout->addWidget(m_labelMes);

    mainLayout->addWidget(m_labelHead);
    mainLayout->addLayout(rightLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
    setMinimumSize(100,40);

    m_menu = nullptr;

    setStyleSheet("QPushButton{border: 0;}");
}

/*************************************************
Function Name： setLatestMessage()
Description:  设置最新消息
*************************************************/
void TLinkButton::setLatestMessage(const TTalkMessage & mes)
{
    m_labelMes->setText(mes.m_text);
}


/*************************************************
Function Name： onClickRightButton()
Description: 右键 单击
*************************************************/
void TLinkButton::onClickRightButton(/*const QPoint &*/)
{
    if (m_menu == nullptr)
    {
        //创建右键菜单
        creatMenu();
    }
    m_menu->exec(QCursor::pos());
}


//remark:protected:----------------------------------------

/*************************************************
Function Name： mousePressEvent
Description: 鼠标单击事件
*************************************************/
void TLinkButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        onClickRightButton();
        return;
    }
    QPushButton::mousePressEvent(event);
}

/*************************************************
Function Name： mouseDoubleClickEvent
Description: 鼠标双击事件
*************************************************/
void TLinkButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
//        emit leftDoubleClickSignal();
        onDoubleClickButton();
        return;
    }
    QPushButton::mouseDoubleClickEvent(event);
}



//remark:private:----------------------------------------

/*************************************************
Function Name： getStatusFromInt()
Description: 创建右键菜单
*************************************************/
void TLinkButton::creatMenu(/*const QList<QString> *groutList*/)
{
    m_menu = new QMenu(this);
    QAction *showMessage = new QAction(tr("查看消息"), m_menu);
    QAction *ignoreMessage = new QAction(tr("忽略消息"), m_menu);
    connect(showMessage, SIGNAL(triggered()),
            this, SLOT(onDoubleClickButton()));
    connect(ignoreMessage, SIGNAL(triggered()),
            this, SLOT(ignoreMessage()));

    m_menu->addAction(showMessage);
    m_menu->addAction(ignoreMessage);
}

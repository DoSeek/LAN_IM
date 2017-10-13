#include "stable.h"
#include "TMessageManageWidget.h"

#include "TLocalMessageWidget.h"
#include "TNetWorkMessageWidget.h"

TMessageManageWidget::TMessageManageWidget(TMainWidget *mainWidget,
                                             QWidget *parent) :
    QWidget(parent), m_mainWidget(mainWidget)
{
    if (m_mainWidget == nullptr)
    {
        return;
    }
    m_tabWidget= new QTabWidget(this);
    m_localWidget = new IMLocalMessageWidget(m_mainWidget, this);
    m_networkWidget = new IMNetWorkMessageWidget(m_mainWidget, this);
    setWindowTitle(tr("消息管理"));
    m_tabWidget->addTab(m_localWidget, tr("本地消息"));
    m_tabWidget->addTab(m_networkWidget, tr("网络消息"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

//    connect(m_localWidget, SIGNAL(currentIndexClicked(int)),
//            this, SLOT());
//    connect(m_networkWidget, SIGNAL(currentIndexClicked(int)),
//            this, SLOT());
}


TMessageManageWidget::~TMessageManageWidget()
{
}


void TMessageManageWidget::setLocalMessageWidget(const QVector<TTalkMessage> & mesVec)
{
    m_localWidget->setMessageShow(mesVec);
}

void TMessageManageWidget::setNetworkMessageWidget(const QVector<TTalkMessage> & mesVec)
{
    m_networkWidget->setMessageShow(mesVec);
}


void TMessageManageWidget::setListModelGroup()
{
    m_localWidget->setListModelGroup();
    m_networkWidget->setListModelGroup();
}

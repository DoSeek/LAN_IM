#ifndef IMMESSAGEMANAGEWIDGET_H
#define IMMESSAGEMANAGEWIDGET_H

#include <QWidget>
#include "TConstant.h"
class TMainWidget;
class QTabWidget;
class IMLocalMessageWidget;
class IMNetWorkMessageWidget;

class TMessageManageWidget : public QWidget
{
    Q_OBJECT
public:
    TMessageManageWidget(TMainWidget *, QWidget *parent = 0);
    ~TMessageManageWidget();

    void setLocalMessageWidget(const QVector<TTalkMessage> &mesVec);
    void setNetworkMessageWidget(const QVector<TTalkMessage> &mesVec);

    void setListModelGroup();

signals:
    
public slots:

private:
    TMainWidget * m_mainWidget;
    QTabWidget *m_tabWidget;
    IMLocalMessageWidget *m_localWidget;
    IMNetWorkMessageWidget *m_networkWidget;

};

#endif // IMMESSAGEMANAGEWIDGET_H

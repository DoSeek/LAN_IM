/*********************************************
File Name： TServerMessageCtrl.h
Author： jet.F.R
Date： 2014.3.10
Description： 控制服务器类
Changes：
********************************************/

#ifndef TSERVERMESSAGECTRL_H
#define TSERVERMESSAGECTRL_H

#include <QObject>
#include "TDatabaseCtrl.h"

class TClientSocketCtrl;
class TTcpServer;

class QStringListModel;
class QListView;

/*************************************************
Class Name： TServerMessageCtrl
Description: 控制服务器
*************************************************/
class TServerMessageCtrl : public QObject
{
    Q_OBJECT
public:
    TServerMessageCtrl(QObject *parent = 0);
    ~TServerMessageCtrl();
    void runServer(); // 用于启动服务器
    QListView *getIDListView();
    QListView *getAddressListView();

signals:

public slots:

private slots:
    void showConnectedMedssage();  //暂时没用，本来是用于显示链接信息 mark： Not used
    void readMessage(TClientSocketCtrl *); //暂时没用，本来是用于读取信息 mark： Not used

    void insertIDData(const QString &id);
    void deleteIDData(const QString &id);

    void insertAddressData(const QString &address);
    void deleteAddressData(const QString &address);

private:

private:
    TTcpServer *mTcpServer;
    quint16 mnBlockSize;  //暂时没用
    IMDatabaseCtrl mDatabaseCtrl;

    QStringList mstrlist;
    QStringListModel *mstrlistmodeID;
    QListView *mlistviewID;

    QStringList m_listAddress;
    QStringListModel *m_modelAddress;
    QListView *m_listViewAddress;
};

#endif // TSERVERMESSAGECTRL_H

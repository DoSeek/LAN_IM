/*********************************************
File Name： TServerMessageCtrl.cpp
Author： jet.F.R
Date： 2014.3.10
Description： 控制服务器类
Changes：
********************************************/

#include "TServerMessageCtrl.h"
#include "TTcpServer.h"
#include "TClientSocketCtrl.h"
#include <QDateTime>
#include <QStringListModel>
#include <QStringList>
TServerMessageCtrl::TServerMessageCtrl(QObject *parent) :
    QObject(parent)
{

}

TServerMessageCtrl::~TServerMessageCtrl()
{

}

/*************************************************
Function Name： runServer()
Description: 用于启动服务器，并监听1234端口
Input： NULL
Output：NULL
Changes： NULL
*************************************************/
void TServerMessageCtrl::runServer()
{
   mTcpServer = new TTcpServer(this);
   connect(mTcpServer, SIGNAL(haveNewID(QString)),
           this, SLOT(insertIDData(QString)));
   connect(mTcpServer, SIGNAL(haveNewAddress(QString)),
           this, SLOT(insertAddressData(QString)));

   connect(mTcpServer, SIGNAL(haveLeftID(QString)),
           this, SLOT(deleteIDData(QString)));
   connect(mTcpServer, SIGNAL(haveLeftAddress(QString)),
           this, SLOT(deleteAddressData(QString)));


   if (!mTcpServer->listen(QHostAddress::Any, 1234))
   {
       qDebug("run fail");

       exit(EXIT_FAILURE);
   }
   mDatabaseCtrl.initDatabase();

   //   qDebug()<<tr("The server is running on port %1.").arg(mTcpServer->serverPort());

   //connect(mTcpServer, SIGNAL(newConnection()), this, SLOT(showConnectedMedssage()));
   //connect(mTcpServer, SIGNAL(newClientSocket(IMClientSocket *)), this, SLOT(readMessage(IMClientSocket *)));

   // QMessageBox::information(NULL, tr("IM Client"),
   //                          tr("Runing to Server..."));
   //     if (mTcpServer->isListening())
   //     {
   //         QMessageBox::information(NULL, tr("IM Client"),
   //                                  tr("The server is running on port %1.").arg(mTcpServer->serverPort()));
   //     }
}


/*************************************************
Function Name： getIDListView()
Description: 获取listiew
*************************************************/
QListView * TServerMessageCtrl::getIDListView()
{
    return mlistviewID;
}

/*************************************************
Function Name： getAddressListView()
Description: 获取listiew
*************************************************/
QListView * TServerMessageCtrl::getAddressListView()
{
    return m_listViewAddress;
}

// mark: private slots---------------------------------------------------

/*************************************************
Function Name： showConnectedMedssage()
Description: Not used
Input： NULL
Output：NULL
Changes： NULL
*************************************************/
void TServerMessageCtrl::showConnectedMedssage()
{
//     QTcpSocket *newClientSocket = mTcpServer->nextPendingConnection();
//     QHostAddress newClientAddress = newClientSocket->peerAddress();
//     qDebug() << newClientAddress;
//     delete newClientSocket
}

/*************************************************
Function Name： readMessage()
Description: Not used
Input： TClientSocketCtrl* socket：
Output：NULL
Changes： NULL
*************************************************/
void TServerMessageCtrl::readMessage(TClientSocketCtrl* socket)
{
    delete socket;

  //IMClientSocket *newClientSocket = socket;
//     if (!newClientSocket->waitForReadyRead(30000))
//     {
//         return;
//     }


//     QDataStream in(newClientSocket);
//     in.setVersion(QDataStream::Qt_4_8);
//     if (m_blockSize == 0)
//     {
//         qDebug()<<"size"<<newClientSocket->bytesAvailable();
//         if (newClientSocket->bytesAvailable() < sizeof(quint16))
//             return;
//         in >> m_blockSize;
//     }

//     if (newClientSocket->bytesAvailable() < m_blockSize)
//     {
//         qDebug("here-43");
//         return;
//     }
//     int type;
//     QString id;
//     QString pwd;
//     QDateTime dateTime;

//     in >> type;

//     switch (type)
//     {
//     case LOGIN:
//         in >> id >> pwd >>dateTime;
//         break;
//     default:
//         break;
//     }
//     qDebug() << m_blockSize << type << id << pwd << dateTime;
}


/*************************************************
Function Name： insertIDData()
Description: 插入数据
*************************************************/
void TServerMessageCtrl::insertIDData(const QString & id)
{
//    int row = mstrlistmodeID->rowCount();
//    mstrlistmodeID->insertRows(row, 1);
//    QModelIndex index = mstrlistmodeID->index(row);
//    mstrlistmodeID->setData(index, id);
//    m_listViewID->setCurrentIndex(index);
    mstrlist.append(id);
    mstrlistmodeID->setStringList(mstrlist);
}

/*************************************************
Function Name： deleteIDData()
Description: 删除数据
*************************************************/
void TServerMessageCtrl::deleteIDData(const QString &id)
{
//    if(mstrlistmodeID->rowCount() > 1)
//    {
//        mstrlistmodeID->removeRows(m_listViewID->currentIndex().row(), 1);
//    }
    int index = mstrlist.indexOf(id);
    mstrlist.removeAt(index);
    mstrlistmodeID->removeRow(index);
}

/*************************************************
Function Name： insertIDData()
Description: 插入数据
*************************************************/
void TServerMessageCtrl::insertAddressData(const QString &address)
{
//    int row = m_modelAddress->rowCount();
//    m_modelAddress->insertRows(row, 1);
//    QModelIndex index = m_modelAddress->index(row);
//    m_modelAddress->setData(index, address);
//    m_listViewAddress->setCurrentIndex(index);

    m_listAddress.append(address);
    m_modelAddress->setStringList(m_listAddress);

}

/*************************************************
Function Name： deleteIDData()
Description: 删除数据
*************************************************/
void TServerMessageCtrl::deleteAddressData(const QString & address)
{
    int index = m_listAddress.indexOf(address);
    m_listAddress.removeAt(index);
    m_modelAddress->removeRow(index);
}

 // mark: private----------------------------------------------------

#include <QCoreApplication>

#include "stable.h"
#include "TTcpServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 启动服务核心
    TTcpServer *server = new TTcpServer();

// 供带GUI模式的服务程序用
//    connect(server, SIGNAL(haveNewID(QString)),\
//            this, SLOT(insertIDData(QString)));
//    connect(server, SIGNAL(haveNewAddress(QString)),\
//            this, SLOT(insertAddressData(QString)));

//    connect(server, SIGNAL(haveLeftID(QString)),\
//            this, SLOT(deleteIDData(QString)));
//    connect(server, SIGNAL(haveLeftAddress(QString)),\
//            this, SLOT(deleteAddressData(QString)));

    if (!server->listen(QHostAddress::Any, 1234))
    {
        QString strErr(QObject::tr("critical: Satrting server listen fail, error string:: "));
        strErr += server->errorString();
        qDebug(strErr.toStdString().c_str());
        exit(EXIT_FAILURE);
    }

    //m_databaseCtrl.initDatabase();


    int nRet = a.exec();
    delete server;

    return nRet;
}

#ifndef IMMAILCTRL_H
#define IMMAILCTRL_H

#include <QObject>
#include "TConstant.h"
#include <QVector>


class IMTcpSocket;
class QString;
class QTreeView;
class QModelIndex;
class QStandardItem;
class QStandardItemModel;


class IMMailCtrl : public QObject
{
    Q_OBJECT
public:
    IMMailCtrl(const QString & id, QObject *parent = 0);
    ~IMMailCtrl();

signals:
    void getInMailsSuccess(const QVector<TMailInformation> &);
    void getOutMailsSuccess(const QVector<TMailInformation> &);
    void deleteMailSuccess(const int &);
    void getOutMail(const TMailInformation &);
    void getInMail(const TMailInformation &);

public slots:
    // 请求发送邮件
    void requestSendMail(const TMailInformation &);

    // 请求获取收件箱邮件
    void requestGetInMails();

    // 请求获取发件箱邮件
    void requestGetOutMails();

    // 请求删除邮件
    void requestDeleteMail(const TMailInformation & mail);

    // 请求修改邮件为已读
    void requestSetMailRead(const TMailInformation & mail);

    // 读取信息
    void readMessage();

private:

private:
    QString m_myID;
    IMTcpSocket *m_tcpSocket;
    quint16 m_blockSize;

    QVector<TMailInformation> m_inMailsVec;
    QVector<TMailInformation> m_outMailsVec;
    TMailInformation m_mail;
    QString m_peerID;
    int m_mailID;
};

#endif // IMMAILCTRL_H

#ifndef IMMAILWIDGET_H
#define IMMAILWIDGET_H

#include <QWidget>
#include <QMap>
#include "TConstant.h"

class QPushButton;
class QTabWidget;
class IMMailButton;
class IMMailOutWidget;
class IMMailInWidget;
class IMMailWriteWidget;
class QLabel;
class IMMailCtrl;
class IMMailInformationWidget;

class TMailWidget : public QWidget
{
    Q_OBJECT
public:
    TMailWidget(const QString & myID, QWidget *parent = 0);
    ~TMailWidget();

    IMMailCtrl* getMailCtrl();
    QString getMyID();
    void showMailInformationWidget(const TMailInformation & mail);
    void showMailInWidget();
    void showMailWriteWidget(const QString & address);

signals:

public slots:

private slots:
    // 设置收件箱
    void setInMailsWidget(const QVector<TMailInformation> &);
    // 设置发件箱
    void setOutMailsWidget(const QVector<TMailInformation> &);

    // 添加收件箱按钮
    bool addInMailButton(const TMailInformation &);
    // 添加发件箱按钮
    bool addOutMailButton(const TMailInformation &);
    // 删除邮件按钮
    void deleteMail(const int &);

    // 添加新邮件到发件箱
    bool addNewOutMailButton(const TMailInformation &);
    // 添加新邮件到收件箱
    bool addNewInMailButton(const TMailInformation &);

private:
    void initIMMailWidget();
    void linkSignalWithSlot();
    void clearContainers();

private:
//    QPushButton *m_btnWrite;        // 写信
//    QPushButton *m_btnIn;           // 收件箱
//    QPushButton *m_btnOut;          // 已发送
//    QPushButton *m_btnDrafts;       // 草稿箱
//    QPushButton *m_btnTrash;        // 垃圾箱

    QString m_myID;

    QLabel *m_labelInformation;
    QTabWidget *m_tabWidget;
    IMMailOutWidget *m_outWidget;
    IMMailInWidget *m_inWidget;
    IMMailWriteWidget *m_writeWidget;
    IMMailInformationWidget *m_informationWidget;

    IMMailCtrl *m_mailCtrl;

    QMap<int, IMMailButton*> m_inMailMap;
    QMap<int, IMMailButton*> m_outMailMap;
};

#endif // IMMAILWIDGET_H

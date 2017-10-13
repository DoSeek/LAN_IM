#ifndef IMDISCUSSIONINFORMATIONWIDGET_H
#define IMDISCUSSIONINFORMATIONWIDGET_H

#include <QWidget>
#include "TConstant.h"
#include "stable.h"

class QPushButton;
class QLabel;
class QLineEdit;
class TMainWidget;

class TDiscussionInformationWidget : public QWidget
{
    Q_OBJECT
public:
    TDiscussionInformationWidget(TMainWidget *mainWidget, QWidget *parent = 0);
    ~TDiscussionInformationWidget();

    void setReadOnly(bool isReadOnly, bool isEdit);
    void setDiscussionInformation(const TDiscussionInformation &);
    TDiscussionInformation getInformation();

signals:

protected:
    void closeEvent(QCloseEvent *);
public slots:
    void onClickCancelButton();
    void onClickOKButton();
private:
    void initIMLoginWidget();

     QLabel *m_labelDiscussionHead;
     QLabel *m_labelDiscussionName;
     QLabel *m_labelDiscussionID;
     QLabel *m_labelCreatorID;
     QLabel *m_labelTheme;
     QLabel *m_labelCreateDate;

     QLineEdit *m_leDiscussionName;
     QLineEdit *m_leDiscussionID;
     QLineEdit *m_leCreatorID;
     QLineEdit *m_leTheme;
     QLineEdit *m_leCreateDate;

     QPushButton *m_btnOk;// "注册" 按钮
     QPushButton *m_btnCancel;  // "取消" 按钮

     bool m_isReadOnly;
     bool m_isEdit;

     TDiscussionInformation m_discussionInf;
     TMainWidget *m_mainWidget;
    
};

#endif // IMDISCUSSIONINFORMATIONWIDGET_H

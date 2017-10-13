#ifndef IMFRIENDLISTWIDGET_H
#define IMFRIENDLISTWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QScrollArea;
class QMenu;
class TMainWidget;

class TFriendListWidget : public QWidget
{
    Q_OBJECT
public:
    TFriendListWidget(TMainWidget *mainWidget, QWidget *parent = 0);
    void addItem(QWidget *item);

    //创建右键菜单
    void creatMenu();

signals:
    

public slots:
    void showMenu();

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_friendsScrollArea;

    //右键菜单
    QMenu *m_menu;

    TMainWidget *m_mainWidget;
};

#endif // IMFRIENDLISTWIDGET_H

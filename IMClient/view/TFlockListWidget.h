#ifndef IMFLOCKLISTWIDGET_H
#define IMFLOCKLISTWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QScrollArea;
class QMenu;
class TMainWidget;


class TFlockListWidget : public QWidget
{
    Q_OBJECT
public:
    TFlockListWidget(TMainWidget *mainWidget, QWidget *parent = 0);
    void addItem(QWidget *item);
    void removeItem(QWidget *item);

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
    QScrollArea *m_flocksScrollArea;

    //右键菜单
    QMenu *m_menu;

    TMainWidget *m_mainWidget;
};

#endif // IMFLOCKLISTWIDGET_H

#ifndef IMSEARCHLISTWIDGET_H
#define IMSEARCHLISTWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QScrollArea;
class QMenu;
class TMainWidget;

class TSearchListWidget : public QWidget
{
    Q_OBJECT
public:
    TSearchListWidget(QWidget *parent = 0);
    void addItem(QWidget *item);
    void removeItem(QWidget *item);
    int size();
    void clearAll();
    
signals:
    
public slots:

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_scrollArea;
    TMainWidget *m_mainWidget;

    int m_mesCounter;
};

#endif // IMSEARCHLISTWIDGET_H

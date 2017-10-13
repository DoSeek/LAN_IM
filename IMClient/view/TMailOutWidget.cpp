#include "TMailOutWidget.h"

IMMailOutWidget::IMMailOutWidget(IMMailCtrl * ctrl, QWidget *parent) :
    m_mailCtrl(ctrl), IMMailListWidget(parent)
{
}

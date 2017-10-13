#include "TMailInWidget.h"

IMMailInWidget::IMMailInWidget(IMMailCtrl * ctrl, QWidget *parent) :
    m_mailCtrl(ctrl), IMMailListWidget(parent)
{
}

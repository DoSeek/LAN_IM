#include <QApplication>
#include "TLoginWidget.h"
#include <QTextCodec>
#include <QDebug>

#include "TInformationWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 以下部分解决中文乱码
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    // 以上部分解决中文乱码

    IMLoginWidget w;
    w.show();
    if (true == w.getIsAutoLogin())
    {
        qDebug() << "click";
        w.onClickBtnLogin();
    }

//    TInformationWidget w;
//    w.setReadOnly(true, true);
//    w.show();
    
    return a.exec();
}

//192.168.50.133

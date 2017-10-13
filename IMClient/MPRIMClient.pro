#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T14:10:37
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = MPRClient

CONFIG   += c++11

PRECOMPILED_HEADER = common/stable.h

# Release model control
CONFIG(debug, debug|release){
    DESTDIR = ./bin/Debug/
}else{
    DESTDIR = ./bin/Release/
}

DEPENDPATH += . \
        control \
        network \
        view \
        resource/image
        common \

INCLUDEPATH += ./ \
    view \
    network \
    control \
    common \

# Input
HEADERS += \
            control/TDatabaseCtrl.h \
            common/TEncryption.h \
            view/TCalendarDialog.h \
            view/TCustomCalendar.h \
            view/TCustomComboBox.h \
            view/TCustomLineEdit.h \
            view/TChatWidget.h \
            view/TClickLabel.h \
            view/TDiscussionButton.h \
            view/TDiscussionChatWidget.h \
            view/TDiscussionInformationWidget.h \
            view/TDiscussionListWidget.h \
            view/TDiscussionMemberButton.h \
            view/TDiscussionMemberListWidget.h \
            view/TFileReceiverWidget.h \
            view/TFileSenderWidget.h \
            view/TFlockButton.h \
            view/TFlockChatWidget.h \
            view/TFlockInformationWidget.h \
            view/TFlockListWidget.h \
            view/TFlockMemberButton.h \
            view/TFlockMemberListWidget.h \
            view/TForgotPwdWidget.h \
            view/TFriendButton.h \
            view/TFriendListWidget.h \
            view/TInformationWidget.h \
            view/TInputTextEdit.h \
            view/TLatestMessageListWidget.h \
            view/TLinkButton.h \
            view/TLinkDiscussionButton.h \
            view/TLinkFlockButton.h \
            view/TLinkFriendButton.h \
            view/TLittleWidget.h \
            view/TLocalMessageWidget.h \
            view/TLoginWidget.h \
            view/TMailButton.h \
            #view/TMailInformationWidget.h \
            #view/TMailInWidget.h \
            #view/TMailListWidget.h \
            #view/TMailOutWidget.h \
            #view/TMailWidget.h \
            #view/TMailWriteWidget.h \
            view/TMainWidget.h \
            view/TMessageManageWidget.h \
            view/TMessageWidget.h \
            view/TNetWorkMessageWidget.h \
            view/TRegisterWidget.h \
            view/TSearchListWidget.h \
            view/TToolBox.h \
            common/stable.h \
            common/TConstant.h \
            control/TClientFileCtrl.h \
            control/TRegisterCtrl.h \
            control/TMainCtrl.h \
            control/TMailCtrl.h \
            control/TLoginCtrl.h \
            control/TClientMessageCtrl.h \
            network/TUser.h \
            network/TTcpSocket.h

SOURCES += main.cpp \
           control/TClientFileCtrl.cpp \
           control/TClientMessageCtrl.cpp \
           control/TDatabaseCtrl.cpp \
           control/TLoginCtrl.cpp \
           control/TMailCtrl.cpp \
           control/TMainCtrl.cpp \
           control/TRegisterCtrl.cpp \
           common/TEncryption.cpp \
           network/TTcpSocket.cpp \
           network/TUser.cpp \
           view/TCalendarDialog.cpp \
           view/TCustomCalendar.cpp \
           view/TCustomComboBox.cpp \
           view/TCustomLineEdit.cpp \
           view/TChatWidget.cpp \
           view/TClickLabel.cpp \
           view/TDiscussionButton.cpp \
           view/TDiscussionChatWidget.cpp \
           view/TDiscussionInformationWidget.cpp \
           view/TDiscussionListWidget.cpp \
           view/TDiscussionMemberButton.cpp \
           view/TDiscussionMemberListWidget.cpp \
           view/TFileReceiverWidget.cpp \
           view/TFileSenderWidget.cpp \
           view/TFlockButton.cpp \
           view/TFlockChatWidget.cpp \
           view/TFlockInformationWidget.cpp \
           view/TFlockListWidget.cpp \
           view/TFlockMemberButton.cpp \
           view/TFlockMemberListWidget.cpp \
           view/TForgotPwdWidget.cpp \
           view/TFriendButton.cpp \
           view/TFriendListWidget.cpp \
           view/TInformationWidget.cpp \
           view/TInputTextEdit.cpp \
           view/TLatestMessageListWidget.cpp \
           view/TLinkButton.cpp \
           view/TLinkDiscussionButton.cpp \
           view/TLinkFlockButton.cpp \
           view/TLinkFriendButton.cpp \
           view/TLittleWidget.cpp \
           view/TLocalMessageWidget.cpp \
           view/TLoginWidget.cpp \
           view/TMailButton.cpp \
           view/TMailInformationWidget.cpp \
           view/TMailInWidget.cpp \
           view/TMailListWidget.cpp \
           view/TMailOutWidget.cpp \
           view/TMailWidget.cpp \
           view/TMailWriteWidget.cpp \
           view/TMainWidget.cpp \
           view/TMessageManageWidget.cpp \
           view/TMessageWidget.cpp \
           view/TNetWorkMessageWidget.cpp \
           view/TRegisterWidget.cpp \
           view/TSearchListWidget.cpp \
           view/TToolBox.cpp
  
RESOURCES += resource/image/imgRes.qrc


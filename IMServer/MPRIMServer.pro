#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T14:12:37
#
#-------------------------------------------------

QT += core network sql
#QT -= gui

CONFIG += c++11

TARGET = IMServer
CONFIG += console precompile_header
CONFIG -= app_bundle

TEMPLATE = app

# Release model control
CONFIG(debug, debug|release){
    DESTDIR = ./bin/Debug/
    CONFIG += TEST_CODE
}else{
    DESTDIR = ./bin/Release/
}

# include path
INCLUDEPATH +=./src/server \
    ./src/common \
    ./src/network \
    ./src/database \

SOURCES += \
    main.cpp\
#    src/server/TServerMessageCtrl.cpp \
    src/server/TTcpServer.cpp \
    src/database/TDatabaseCtrl.cpp \
    src/network/TClientSocketCtrl.cpp

HEADERS  += \
#    src/server/TServerMessageCtrl.h \
    src/server/TTcpServer.h \
    src/database/TDatabaseCtrl.h \
    src/common/TConstant.h \
    src/common/stable.h \
    src/network/TClientSocketCtrl.h


# precompile
# Use Precompiled headers (PCH)
precompile_header: {
    PRECOMPILED_HEADER  = src/common/stable.h
}

precompile_header:!isEmpty(PRECOMPILED_HEADER) {
    DEFINES += USING_PCH
}

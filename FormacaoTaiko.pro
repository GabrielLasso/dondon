#-------------------------------------------------
#
# Project created by QtCreator 2018-08-16T17:17:34
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = FormacaoTaiko
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    src/commandadd.cpp \
    src/commanddelete.cpp \
    src/commandmove.cpp \
    src/commandrotate.cpp \
    src/instrumento.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mapa.cpp \
    src/mapascene.cpp \
    src/mapaview.cpp \
    src/mapawidget.cpp \
    src/qgraphicstaikoitem.cpp

HEADERS += \
    src/commandadd.h \
    src/commanddelete.h \
    src/commandmove.h \
    src/commandrotate.h \
    src/instrumento.h \
    src/mainwindow.h \
    src/mapa.h \
    src/mapascene.h \
    src/mapaview.h \
    src/mapawidget.h \
    src/qgraphicstaikoitem.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/mapawidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/resources/resources.qrc

RC_ICONS += icon.ico

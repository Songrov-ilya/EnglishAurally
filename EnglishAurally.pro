#-------------------------------------------------
#
# Project created by QtCreator 2018-03-04T14:55:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += multimedia

TARGET = English' 'Aurally
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MakeTrack.cpp

HEADERS  += mainwindow.h \
    MakeTrack.h \

FORMS    += mainwindow.ui

DISTFILES += \
        icoEnglishAurally.rc

        RC_FILE += icoEnglishAurally.rc


RESOURCES += \
    resources_file.qrc

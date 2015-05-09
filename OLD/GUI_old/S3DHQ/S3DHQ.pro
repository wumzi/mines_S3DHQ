#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T19:16:11
#
#-------------------------------------------------

QT       += serialport core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = S3DHQ
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        camera.cpp \
        serialconnection.cpp \

HEADERS  += mainwindow.h \
        camera.h \
        serialconnection.h

FORMS    += mainwindow.ui

INCLUDEPATH += "C:\Users\Maxime Ernoult\Documents\Mecatro\Mécatro\S3DHQ08_versionFinale\S3DHQ"

LIBS += -lEDSDK

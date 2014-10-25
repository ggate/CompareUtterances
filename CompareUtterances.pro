#-------------------------------------------------
#
# Project created by QtCreator 2014-10-15T14:18:47
#
#-------------------------------------------------

QT       += core

QT       += gui
QT       += widgets

TARGET = CompareUtterances
CONFIG   += console
CONFIG   -= app_bundle

######### To be modified to match boost installation paths ###########
INCLUDEPATH += /usr/local/Cellar/boost/1.56.0/include
LIBS += /usr/local/Cellar/boost/1.56.0/lib/libboost_system.dylib
LIBS += /usr/local/Cellar/boost/1.56.0/lib/libboost_filesystem.dylib
######################################################################

TEMPLATE = app


SOURCES += main.cpp \
    utterancecomparator.cpp \
    dtwplot.cpp \
    mfccplot.cpp \
    mainwindow.cpp

HEADERS += \
    utterancecomparator.h \
    dtwplot.h \
    mfccplot.h \
    mainwindow.h

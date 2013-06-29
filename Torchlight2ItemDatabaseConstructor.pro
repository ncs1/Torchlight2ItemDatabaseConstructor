#-------------------------------------------------
#
# Project created by QtCreator 2013-04-07T19:53:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

TARGET = Torchlight2ItemDatabaseConstructor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sqlitetable.cpp \
    effectstable.cpp \
    effectsdatfileparser.cpp \
    itemdatfileparser.cpp \
    itemdetailstable.cpp \
    itembaserequirementstable.cpp \
    itemlevelrequirementsparser.cpp \
    graphfileparser.cpp \
    baseweapondamagetable.cpp \
    baseweapondamageparser.cpp \
    skillsdatfileparser.cpp \
    skillstable.cpp \
    affixtable.cpp \
    affixdatfileparser.cpp

HEADERS  += mainwindow.h \
    sqlitetable.h \
    effectstable.h \
    effectsdatfileparser.h \
    itemdatfileparser.h \
    itemdetailstable.h \
    itembaserequirementstable.h \
    itemlevelrequirementsparser.h \
    graphfileparser.h \
    baseweapondamagetable.h \
    baseweapondamageparser.h \
    skillsdatfileparser.h \
    skillstable.h \
    affixtable.h \
    affixdatfileparser.h

FORMS    += mainwindow.ui

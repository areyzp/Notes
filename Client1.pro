#-------------------------------------------------
#
# Project created by QtCreator 2015-06-28T21:06:14
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        note.cpp \
    category.cpp \
    notesform.cpp \
    tag.cpp \
    parser.cpp \
    categorycheckbox.cpp \
    notewidget.cpp \
    addcategory.cpp \
    noteview.cpp \
    noteedit.cpp \
    tagmanager.cpp

HEADERS  += mainwindow.h \
            note.h \
    category.h \
    notesform.h \
    tag.h \
    parser.h \
    categorycheckbox.h \
    notewidget.h \
    addcategory.h \
    noteview.h \
    noteedit.h \
    tagmanager.h

FORMS    += mainwindow.ui \
            notesform.ui \
    notewidget.ui \
    noteview.ui \
    noteedit.ui

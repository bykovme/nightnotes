#-------------------------------------------------
#
# Project created by QtCreator 2013-09-19T15:04:58
#
#-------------------------------------------------

QT       += core gui

#macx:LIBS +=  -lmarkdown++ -lmarkdown

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = NightNotes
TEMPLATE = app

macx:ICON = nightnotes.icns
win32:RC_FILE = nightnotes.rc

SOURCES += main.cpp\
        nightwindow.cpp \
    nightnoteprefs.cpp \
    nightabout.cpp \
    nightmessage.cpp \
    nightsavefile.cpp \
    nightfilesoperations.cpp \
    nightopenfile.cpp \
    nighttree.cpp \
    nightnewfile.cpp

HEADERS  += nightwindow.h \
    themes.h \
    const.h \
    nightnoteprefs.h \
    nightabout.h \
    nightmessage.h \
    nightnewfile.h \
    discount/discount-wrapper.hpp \
    discount/mkdio.h

FORMS    += nightwindow.ui \
    nightnoteprefs.ui \
    nightabout.ui \
    nightmessage.ui \
    nightsearchdialog.ui \
    nightnewfile.ui

RESOURCES += \
    NightNoteRes.qrc

OTHER_FILES += \
    nightnotes.icns


INCLUDEPATH += $$PWD/discount
DEPENDPATH += $$PWD/discount

macx: LIBS += -L$$PWD/discount/ -lmarkdown++
macx: LIBS += -L$$PWD/discount/ -lmarkdown

macx: PRE_TARGETDEPS += $$PWD/discount/libmarkdown.a
macx: PRE_TARGETDEPS += $$PWD/discount/libmarkdown++.a

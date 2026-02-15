#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T00:31:55
#
#-------------------------------------------------

QT       += core gui

TARGET = MPcccView
TEMPLATE = app


CODECFORTR = UTF-8
CODECFORSRC = UTF-8

QMAKE_CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8

unix:!macx {
    # Путь к библиотекам относительно бинарника
    QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN\'
}

SOURCES += main.cpp\
        mainwindow.cpp \
    I8080.cpp \
    LineEditBool.cpp \
    lineedithex.cpp \
    INPUT.cpp \
    GLOBAL.cpp \
    output.cpp \
    mem.cpp \
    mem_cell.cpp \
    in08.cpp \
    addressdelegate.cpp \
    instrdelegate.cpp \
    codedelegate.cpp \
    ascii.cpp \
    aboutvs.cpp

HEADERS  += mainwindow.h \
    I8080.h \
    LineEditBool.h \
    lineedithex.h \
    GLOBAL.h \
    INPUT.h \
    problem.h \
    output.h \
    mem.h \
    mem_cell.h \
    in08.h \
    addressdelegate.h \
    instrdelegate.h \
    codedelegate.h \
    ascii.h \
    aboutvs.h

FORMS    += mainwindow.ui \
    in08.ui \
    ascii.ui \
    aboutvs.ui

RESOURCES += \
    res.qrc

RC_FILE += file.rc

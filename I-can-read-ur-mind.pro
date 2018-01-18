#-------------------------------------------------
#
# Project created by QtCreator 2018-01-04T05:00:56
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = I-can-read-ur-mind
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


SOURCES += \
    src/main.cpp \
    src/view/main-window.cpp \
    src/additional_lib.cpp \
    src/model/eye-tracker.cpp \
    src/view/menu.cpp \
    src/view/calibration.cpp \
    src/model.cpp \
    src/view.cpp \
    src/controller.cpp \
    src/model/tests-controller.cpp

HEADERS += \
    src/view/main-window.h \
    src/additional_header.h \
    src/model/eye-tracker.h \
    src/view/menu.h \
    src/view/calibration.h \
    src/model.h \
    src/view.h \
    src/controller.h \
    src/model/tests-controller.h

#FORMS +=


win32:LIBS += -L$$PWD/3rdparty/tobii_research/lib/ -ltobii_research

INCLUDEPATH += $$PWD/3rdparty/tobii_research/include
DEPENDPATH += $$PWD/3rdparty/tobii_research/include

RESOURCES += \
    res.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2018-01-04T05:00:56
#
#-------------------------------------------------

QT       += core gui

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
    src/main-window.cpp \
    src/additional_lib.cpp \
    src/BCIs/eye-tracker/eye-tracker.cpp \
    src/widgets/menu/menu.cpp \
    src/BCIs/bci.cpp \
    src/widgets/eyeTracker/calibration/calibration.cpp

HEADERS += \
    src/main-window.h \
    src/additional_header.h \
    src/BCIs/eye-tracker/eye-tracker.h \
    src/widgets/menu/menu.h \
    src/BCIs/bci.h \
    src/widgets/eyeTracker/calibration/calibration.h

#FORMS +=


win32:LIBS += -L$$PWD/3rdparty/tobii_research/lib/ -ltobii_research

INCLUDEPATH += $$PWD/3rdparty/tobii_research/include
DEPENDPATH += $$PWD/3rdparty/tobii_research/include

RESOURCES += \
    res.qrc \
    src/resources.qrc

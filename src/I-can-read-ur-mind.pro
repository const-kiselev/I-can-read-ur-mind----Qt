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
    main.cpp \
    main-window.cpp \
    additional_lib.cpp \
    BCIs/eye-tracker/eye-tracker.cpp \
    widgets/menu/menu.cpp \
    BCIs/bci.cpp \
    widgets/eyeTracker/calibration/calibration.cpp

HEADERS += \
    main-window.h \
    additional_header.h \
    BCIs/eye-tracker/eye-tracker.h \
    widgets/menu/menu.h \
    BCIs/bci.h \
    widgets/eyeTracker/calibration/calibration.h

#FORMS +=

#LIBS += /usr/local/lib/tobii_research
#macx:LIBS += -ltobii_research -L/usr/local/lib/tobii_research
LIBS += /I"Z:\Documents\Develop\I-can-read-ur-mind----Qt\TobiiPro.SDK.C_Binding.Windows_1.2.1.55_64\include" "Z:\Documents\Develop\I-can-read-ur-mind----Qt\TobiiPro.SDK.C_Binding.Windows_1.2.1.55_64\lib\tobii_research.lib"


#INCLUDEPATH += /usr/local/include
macx:INCLUDEPATH += /usr/local/include
win32:INCLUDEPATH += ..\TobiiPro.SDK.C_Binding.Windows_1.2.1.55_64\include



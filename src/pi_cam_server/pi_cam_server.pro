QT += core gui network

CONFIG += c++11

TARGET = pi_cam_server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QT_SYSROOT = /home/amndan/workspace/embedded/rpi3/sysroot/

SOURCES += main.cpp \
    ../utils/CommunicationTCPPiCam.cpp \
    ../utils/ConnectionSingleTCP.cpp \
    ServerPiCam.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -L/usr/local/lib -lraspicam

INCLUDEPATH += $$QT_SYSROOT/usr/local/include

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

target.path = /tmp
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../utils/CommunicationTCPPiCam.h \
    ../utils/ConnectionSingleTCP.h \
    $$QT_SYSROOT/usr/local/include/raspicam/raspicam.h \
    $$QT_SYSROOT/usr/local/include/raspicam/raspicamtypes.h \
    ServerPiCam.h

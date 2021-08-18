QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle
QT += network
QT += websockets
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        client.cpp \
        consoleinput.cpp \
        main.cpp \
        parser.cpp \
        weather.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    client.h \
    consoleinput.h \
    parser.h \
    weather.h

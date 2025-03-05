QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += header

SOURCES += \
    src/custombutton.cpp \
    src/gamewindow.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/startwindow.cpp \
    src/timerwidget.cpp

HEADERS += \
    header/custombutton.h \
    header/gamewindow.h \
    header/mainwindow.h \
    header/startwindow.h \
    header/timerwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

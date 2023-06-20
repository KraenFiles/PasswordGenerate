QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/filerw.cpp \
    src/generate.cpp \
    src/generatepassword.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/passwordbutton.cpp \
    src/passwordlist.cpp

HEADERS += \
    src/filerw.h \
    src/generate.h \
    src/generatepassword.h \
    src/mainwindow.h \
    src/passwordbutton.h \
    src/passwordlist.h

FORMS += \
    src/generate.ui \
    src/mainwindow.ui \
    src/passwordlist.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
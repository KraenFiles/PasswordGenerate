QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#greaterThan(QT_MAJOR_VERSION,5): QT += core5compat

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/encryptionengine.cpp \
    src/filerw.cpp \
    src/generate.cpp \
    src/generatepassword.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/passwordbutton.cpp \
    src/passwordlist.cpp \
    src/synchroniser.cpp \
    src/applink.c

HEADERS += \
    src/encryptionengine.h \
    src/filerw.h \
    src/generate.h \
    src/generatepassword.h \
    src/mainwindow.h \
    src/passwordbutton.h \
    src/passwordlist.h \
    src/synchroniser.h

FORMS += \
    src/generate.ui \
    src/mainwindow.ui \
    src/passwordlist.ui

unix:{
INCLUDEPATH += /usr/local/include
LIBS += -lssl -lcrypto
}
else: win32: {
INCLUDEPATH += Q:/Qt/Tools/OpenSSLv3/Win_x64/include
LIBS += -LQ:/Qt/Tools/OpenSSLv3/Win_x64/lib -lssl -lcrypto
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(ANDROID_TARGET_ARCH,x86_64) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

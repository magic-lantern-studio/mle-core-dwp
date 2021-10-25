QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += MLE_NOT_DLL DEBUG

INCLUDEPATH += $$PWD/../../common/include $$PWD/../../linux/include /opt/MagicLantern/include

SOURCES += \
        $$PWD/../../common/test/dwpCheckerTest.cpp

unix:!macx {
    LIBS += -L/opt/MagicLantern/lib -lDWP -lmlmath -lmlutil -Wl,--no-as-needed -ldl
}

macx {
    LIBS += -L/opt/MagicLantern/lib -lDWP -lmlmath -lmlutil
}

# Default rules for deployment.
unix {
    target.path = /opt/MagicLantern/bin
    headers.path = /opt/MagicLantern/include/mle
    headers.files = $$HEADERS
    INSTALLS += target headers
}
!isEmpty(target.path): INSTALLS += target

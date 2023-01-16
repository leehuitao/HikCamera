DEFINES += QT_DEPRECATED_WARNINGS


contains(QT_ARCH, i386){

}
else{
LIBS += $$PWD/Depends/x64/vs2013shared/MVSDKmd.lib\
        $$PWD/Depends/x64/vs2013shared/VideoRender.lib\
        $$PWD/Depends/x64/vs2013shared/ImageConvert.lib
}

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/Include

HEADERS += \
    $$PWD/dahuacamera.h

SOURCES += \
    $$PWD/dahuacamera.cpp


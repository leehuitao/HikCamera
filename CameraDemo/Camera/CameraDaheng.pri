DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += .
INCLUDEPATH += "$$(DahengRoot)/MER-Series/Samples/C++ SDK/inc"

LIBS += "$$(DahengRoot)/MER-Series/Samples/C++ SDK/lib/x86/GxIAPICPP.lib"

HEADERS += \
$$PWD/CameraBase.h \
$$PWD/CameraDaheng.h

SOURCES += \
$$PWD/CameraBase.cpp \
$$PWD/CameraDaheng.cpp

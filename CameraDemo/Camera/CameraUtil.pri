DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += .
INCLUDEPATH += "$$(MVCAM_COMMON_RUNENV)/Includes"
INCLUDEPATH += "$$(MVCAM_COMMON_RUNENV)/Includes/GenICam"
INCLUDEPATH += "$$(DahengRoot)/MER-Series/Samples/C++ SDK/inc"

LIBS += "$$(MVCAM_COMMON_RUNENV)/libraries/win32/MvCameraControl.lib"
LIBS += "$$(DahengRoot)/MER-Series/Samples/C++ SDK/lib/x86/GxIAPICPP.lib"

HEADERS += \
$$PWD/CameraBase.h \
$$PWD/CameraDaheng.h \
$$PWD/CameraHik.h

SOURCES += \
$$PWD/CameraBase.cpp \
$$PWD/CameraDaheng.cpp \
$$PWD/CameraHik.cpp

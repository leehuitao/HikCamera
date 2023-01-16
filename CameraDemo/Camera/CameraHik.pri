DEFINES += QT_DEPRECATED_WARNINGS


win32: LIBS += -L$$PWD/../lib/ -lMvCameraControl

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
INCLUDEPATH += $$PWD/../include/GenICam
DEPENDPATH += $$PWD/../include/GenICam
LIBS += $$PWD/../lib/MvCameraControl.lib

HEADERS += \
$$PWD/CameraBase.h \
$$PWD/CameraHik.h

SOURCES += \
$$PWD/CameraBase.cpp \
$$PWD/CameraHik.cpp

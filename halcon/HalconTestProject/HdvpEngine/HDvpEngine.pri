INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/hdvpengine.h

SOURCES += \
    $$PWD/hdvpengine.cpp


INCLUDEPATH += $$PWD/Halcon18/include
INCLUDEPATH += $$PWD/Halcon18/include/halconcpp

LIBS += $$PWD/Halcon18/lib/x64-win64/halconcpp.lib \
        $$PWD/Halcon18/lib/x64-win64/halcon.lib \
        $$PWD/Halcon18/lib/x64-win64/hdevenginecpp.lib \






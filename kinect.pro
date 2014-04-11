greaterThan(QT_MAJOR_VERSION, 4): QT += quick

TARGET = kinect
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += include

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv libfreenect
}

win32 {
    INCLUDEPATH += C:\opencv\build\include \
                   C:\GDCM\gdcm\include

    !contains(QMAKE_HOST.arch, x86_64) {
            QMAKE_LFLAGS *= /MACHINE:X86
            LIBS += -L"C:\opencv\build\x86\vc12\lib"
    }
    else {
        contains(QMAKE_HOST.arch, x86_64):{
            QMAKE_LFLAGS *= /MACHINE:X64
            LIBS += -L"C:\opencv\build\x64\vc12\lib"
        }
    }

    LIBS += -lopencv_core248 \
            -lopencv_highgui248 \
            -lopencv_imgproc248 \
            -lopencv_ocl248
}

SOURCES += src/main.cpp \
    src/freenectdev.cpp

HEADERS += include/freenectdev.h \
    include/libfreenect.hpp \
    include/libfreenect.h


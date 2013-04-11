# -------------------------------------------------
# Project created by QtCreator 2012-12-09T18:12:19
# -------------------------------------------------
QT += opengl
QT += core \
    	gui \
    	xml
QMAKE_CXX=clang++ -fdiagnostics-fixit-info

TARGET=Deferred-Renderer
DESTDIR=./
OBJECTS_DIR=obj
# this is where we want to put the intermediate build files ( .o)
OBJECTS_DIR=./obj/
MOC_DIR=./moc/
SOURCES += src/main.cpp \
           src/MainWindow.cpp \
           src/GLWindow.cpp \
    src/Spotlight.cpp \
    src/SphereLightVolume.cpp \
    src/ObjectLoader.cpp \
    src/LightManager.cpp \
    src/DeferredSpotLight.cpp \
    src/DeferredShading.cpp \
    src/DeferredPointLight.cpp \
    src/DeferredDirectionalLight.cpp \
    src/ConeLightVolume.cpp
INCLUDEPATH+=./include
HEADERS += include/MainWindow.h \
           include/GLWindow.h \
    include/SpotLight.h \
    include/SphereLightVolume.h \
    include/ObjectLoader.h \
    include/DeferredShading.h \
    include/ConeLightVolume.h \
    include/ngl/Light.h
OTHER_FILES+= shaders/Phong.fs \
              shaders/Phong.vs
CONFIG += console
CONFIG -= app_bundle
macx:INCLUDEPATH+=$$(HOME)/boost_1_47_0
DEFINES+=GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
DEFINES+=ADDLARGEMODELS

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native
# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG

LIBS += -L/usr/local/lib
# add the ngl lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-g++:linux-g++-64 {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
linux-clang {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN
# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/

win32: {
        DEFINES+=USING_GLEW
        INCLUDEPATH+=-I c:/boost_1_44_0
        INCLUDEPATH+=-I c:/boost

        INCLUDEPATH+= -I C:/NGL/Support/glew
        LIBS+= -L C:/NGL/lib
        LIBS+= -lmingw32
        DEFINES += WIN32
        DEFINES += USING_GLEW
        DEFINES +=GLEW_STATIC
        DEFINES+=_WIN32
        SOURCES+=C:/NGL/Support/glew/glew.c
        INCLUDEPATH+=C:/NGL/Support/glew/
}


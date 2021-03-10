TEMPLATE = app
TARGET = test
INCLUDEPATH += .

SOURCES += ../glad/egl.c test.cpp
INCLUDEPATH += ../glad

QT += gui
QT += widgets
QT += opengl

LIBS += -lEGL -ldl

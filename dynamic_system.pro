#-------------------------------------------------
#
# Project created by QtCreator 2023-09-08T14:43:58
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dynamic_system
TEMPLATE = app


SOURCES += src/main.cpp\
           src/system/dynamic_system.cpp \
           src/matrix/matrix.cpp \
           src/widget/gwidget.cpp

HEADERS  += src/system/dynamic_system.h \
            src/matrix/matrix.h \
            src/widget/gwidget.h

QT += core
QT -= gui

CONFIG += c++11

TARGET = sm4
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    sm4.cpp

HEADERS += \
    sm4.h

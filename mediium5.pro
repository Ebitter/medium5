QT += core
QT -= gui

CONFIG += c++11

TARGET = mediium5
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    CppSQLite3.cpp \
    sqlite3.c

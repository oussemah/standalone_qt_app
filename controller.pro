#-------------------------------------------------
#
# Project created by QtCreator 2016-03-26T11:35:58
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): cache

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = controller
TEMPLATE = app

CONFIG += release

target.path = /usr/bin
INSTALLS += target

INCLUDEPATH += GeneratedFiles

MOC_DIR = GeneratedFiles
OBJECTS_DIR = Objects
UI_DIR = GeneratedFiles
RCC_DIR = GeneratedFiles

SOURCES += main.cpp\
        mainwindow.cpp \
    menu_gpio.cpp \
    gpio_settings.cpp \
    network_setting.cpp \
    qgpiohandler.cpp

HEADERS  += mainwindow.h \
    menu_gpio.h \
    gpio_settings.h \
    network_setting.h \
    qgpiohandler.h

FORMS    += \
    menu_gpio.ui \
    gpio_settings.ui \
    mainwindow.ui \
    network_setting.ui

RESOURCES += \
    controller.qrc

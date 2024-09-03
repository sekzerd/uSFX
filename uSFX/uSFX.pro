#QT += core gui widgets
#CONFIG += c++11
#QT += core
QT -= gui
CONFIG += c++11 console

SOURCES += main.cpp
#    MainWindow.cpp

HEADERS += AppUtils.h
#    MainWindow.h

#FORMS += \
#    MainWindow.ui

INCLUDEPATH += $$PWD/../quazip
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib
include($$PWD/../quazip/quazip.pri)
DEFINES += QT_DDEPRECATED_WARNINGS QUAZIP_STATIC


CONFIG(debug, release|debug){
    DESTDIR = $$PWD/../debug
#    UI_DIR += $$PWD/../debug/usfx/ui
    OBJECTS_DIR += $$PWD/../debug/usfx/obj
    MOC_DIR += $$PWD/../debug/usfx/moc
    RCC_DIR += $$PWD/../debug/usfx/rcc
}else{
    DESTDIR = $$PWD/../release
#    UI_DIR += $$PWD/../release/usfx/ui
    OBJECTS_DIR += $$PWD/../release/usfx/obj
    MOC_DIR += $$PWD/../release/usfx/moc
    RCC_DIR += $$PWD/../release/usfx/rcc
}

QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += main.cpp

INCLUDEPATH += $$PWD/../quazip

INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtZlib
include($$PWD/../quazip/quazip.pri)
DEFINES += QT_DDEPRECATED_WARNINGS QUAZIP_STATIC


CONFIG(debug, release|debug){
    DESTDIR = $$PWD/../debug
    UI_DIR += $$PWD/../debug/upack/ui
    OBJECTS_DIR += $$PWD/../debug/upack/obj
    MOC_DIR += $$PWD/../debug/upack/moc
    RCC_DIR += $$PWD/../debug/upack/rcc

}else{
    DESTDIR = $$PWD/../release
    UI_DIR += $$PWD/../release/upack/ui
    OBJECTS_DIR += $$PWD/../release/upack/obj
    MOC_DIR += $$PWD/../release/upack/moc
    RCC_DIR += $$PWD/../release/upack/rcc

}

@echo off
SetLocal EnableDelayedExpansion
(set PATH=E:\SDK\qt-everywhere-src-5.14.2\build\qtbase\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=E:\SDK\qt-everywhere-src-5.14.2\build\qtbase\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=E:\SDK\qt-everywhere-src-5.14.2\build\qtbase\plugins
)
E:\SDK\qt-everywhere-src-5.14.2\build\qtbase\bin\uic.exe %*
EndLocal

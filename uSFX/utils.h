#ifndef APPUTILS_H
#define APPUTILS_H

#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QString>
#include <QTextStream>
#include <QtDebug>

void app_debug_handler(QtMsgType type, const QMessageLogContext &context,
                       const QString &msg) {
#ifdef QT_DEBUG
    static QMutex mutex;
    mutex.lock();
    static QFile logFile("log.txt");
    if (!logFile.isOpen()) {
        logFile.open(QIODevice::WriteOnly | QIODevice::Append |
                     QIODevice::Text);
    }
    QTextStream out(&logFile);
    QString buffer;
    switch (type) {
    case QtDebugMsg:
        buffer += "[Debug]: ";
        break;
    case QtInfoMsg:
        buffer += "[Info]: ";
        break;
    case QtWarningMsg:
        buffer += "[Warning]: ";
        break;
    case QtCriticalMsg:
        buffer += "[Critical]: ";
        break;
    case QtFatalMsg:
        buffer += "[Fatal]: ";
        break;
    }

    buffer += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    buffer += msg;

    buffer += " (";
    buffer += QString(context.file);
    buffer += ":";
    buffer += QString::number(context.line);
    buffer += ") ";

    out << buffer << endl;
    qDebug() << buffer;
    mutex.unlock();
#endif
}

#include <QUuid>
static QString random_string() {
    QUuid uuid = QUuid::createUuid();
    return uuid.toString().remove('{').remove('}');
}

#include "quazip/JlCompress.h"
static QStringList decompress_file(const QString &input_file,
                                   const QString &output_dir) {
    return JlCompress::extractDir(input_file, output_dir);
}

#include <QStandardPaths>
static QString temp_dir(QString suffix) {
    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString t = tempDir + "/uSFX_" + suffix;
    return t;
}

static bool init_dir(QString suffix) {
    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString t = tempDir + "/uSFX_" + suffix;
    return QDir().mkpath(t);
}

static bool clean_dir(QString suffix) {
    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString t = tempDir + "/uSFX_" + suffix;
    QDir dir(t);
    return dir.removeRecursively();
}

#include <QCoreApplication>
QByteArray extract_self() { //    MainWindow w;
    //    w.show();
    QString appPath = QCoreApplication::applicationFilePath();
    //    qDebug() << "App Path:" << appPath;
    QFile app(appPath);
    if (!app.exists()) {
        qDebug() << "!app.exists()";
        return "";
    }
    if (!app.open(QFile::OpenMode::enum_type::ReadOnly)) {
        qDebug() << "!app.open(QFile::OpenMode::enum_type::ReadOnly)";
        return "";
    }
    return app.readAll();
}

bool write_bin_to_file(const QString &filePath, const QByteArray &data) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return false;
    }
    file.write(data);

    //    QDataStream out(&file);
    //    out << data;
    if (!file.flush()) {
        qWarning() << "Failed to flush data to file:" << file.errorString();
        return false;
    }
    file.close();
    return true;
}

#endif // APPUTILS_H

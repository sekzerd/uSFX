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
#ifdef QT_DEBUG
    buffer += " (";
    buffer += QString(context.file);
    buffer += ":";
    buffer += QString::number(context.line);
    buffer += ") ";
#endif
    out << buffer << endl;
    qDebug() << buffer;
    mutex.unlock();
}
#endif // APPUTILS_H

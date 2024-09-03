#include "AppUtils.h"
#include "MainWindow.h"
#include "quazip/JlCompress.h"

#include <QBuffer>
#include <QCoreApplication>
#include <QFile>
#include <QIODevice>
// #include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QtDebug>

static QString SPLIT_FLAG_START = "#!%__uSFX_SPLIT_START_dd5de4df__%!#";
static QString SPLIT_FLAG_END = "#!%__uSFX_SPLIT_END_dd5de4df__%!#";

static QStringList decompress_file(const QString &input_file,
                                   const QString &output_dir) {
    //    JlCompress compress;
    return JlCompress::extractDir(input_file, output_dir);
}
static bool init_dir() {
    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString t = tempDir + "/uSFX";
    return QDir().mkpath(t);
}

static bool clean_dir() {
    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString t = tempDir + "/uSFX";
    QDir dir(t);
    return dir.removeRecursively();
}
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

int main(int argc, char *argv[]) {
    //    QCoreApplication a(argc, argv);
    qInstallMessageHandler(app_debug_handler);
    //    MainWindow w;
    //    w.show();
    init_dir();
    QByteArray data = extract_self();
    if (data.isEmpty()) {
        //        QMessageBox::critical(nullptr, "error", "load data faied");
        return 0;
    }
    QString prefix = SPLIT_FLAG_START;
    prefix.push_back(0x0d);
    prefix.push_back(0x0a);
    prefix.append("[app]");

    auto start = data.indexOf(prefix.toLocal8Bit());
    auto end = data.indexOf(SPLIT_FLAG_END.toLocal8Bit(), start);
    if (start == -1 || end == -1 || start == end) {
        qDebug() << "flag error start:" << start << "\tend:" << end;
        //        QMessageBox::critical(nullptr, "error", "load data failed");
        return 0;
    }
    QTemporaryFile tempFile;
    if (!tempFile.open()) {
        qDebug() << "Failed to open temporary file.";
        return 0;
    }
    //    qDebug() << tempFile.fileName();
    QByteArray data_ini;
    for (int i = start; i < end; ++i) {
        data_ini.append(data[i]);
    }
    //    qDebug() << QString::fromLocal8Bit(data_ini);
    if (tempFile.write(data_ini) == -1) {
        qDebug() << "Failed to write data to temporary file.";
        return 0;
    }

    if (!tempFile.flush()) {
        qDebug() << "Failed to flush temporary file.";
        return 0;
    }

    QSettings settings(tempFile.fileName(), QSettings::IniFormat);
    //    qDebug() << settings.value("app/entry").toString();

    QByteArray zip;
    int s = end + SPLIT_FLAG_END.size();
    for (int i = s; i < data.size(); ++i) {
        zip.append(data[i]);
    }

    // for test
    //    auto f = write_bin_to_file("test.zip", zip);
    //    qDebug() << "write file test.zip:" << f;
    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString zip_file_name = tempDir + "/uSFX/" + "test.zip";
    qDebug() << "output path:" << zip_file_name;
    auto f = write_bin_to_file(zip_file_name, zip);
    qDebug() << "write_bin_to_file:" << f;
    QString zip_file_dir = tempDir + "/uSFX";

    auto files = decompress_file(zip_file_name, zip_file_dir);
    qDebug() << "decompress_file:" << files;
    QProcess process;

    QString programPath =
        zip_file_dir + "/" + settings.value("app/entry").toString();
    QStringList arguments;
    process.setWorkingDirectory(zip_file_dir);
    process.start(programPath, arguments);

    if (!process.waitForStarted()) {
        qDebug() << "Failed to start program:" << programPath;
    }

    if (!process.waitForFinished()) {
        qDebug() << "Program did not finish within the specified time limit.";
    }

    QByteArray standard_output = process.readAllStandardOutput();
    QByteArray error_output = process.readAllStandardError();

    qDebug() << "standard_output:" << standard_output;
    qDebug() << "error_output:" << error_output;
    clean_dir();
    return 0;
    //    return a.exec();
}

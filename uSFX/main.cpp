#include "utils.h"
// #include "MainWindow.h"

// #include <QApplication>
// #include <QMessageBox>

#include <QBuffer>
#include <QCoreApplication>
#include <QFile>
#include <QIODevice>
#include <QProcess>
#include <QSettings>
#include <QTemporaryFile>
#include <QtDebug>
#include <QtGlobal>

static QString SPLIT_FLAG_START = "#!%__uSFX_SPLIT_START_dd5de4df__%!#";
static QString SPLIT_FLAG_END = "#!%__uSFX_SPLIT_END_dd5de4df__%!#";
static QByteArray decrypt(const QByteArray& data) {
    QByteArray ret;
    for (auto i = 0; i < data.length(); i++) {
        ret.push_back((unsigned char)~data[i]);
    }
    return ret;
}
int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    //    QApplication a(argc, argv);

    qInstallMessageHandler(app_debug_handler);
    //    MainWindow w;
    //    w.show();
    auto random_suffix = random_string();
    init_dir(random_suffix);
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

    QString zip_file_name = temp_dir(random_suffix) + "/temp.zip";

    qDebug() << "output path:" << zip_file_name;
    auto decryto_data = decrypt(zip);
    qDebug() << "data len:" << zip.length();
    qDebug() << "decryto len:" << decryto_data.length();

    auto f = write_bin_to_file(zip_file_name, decryto_data);
    qDebug() << "write_bin_to_file:" << f;
    QString zip_file_dir = temp_dir(random_suffix);

    auto files = decompress_file(zip_file_name, zip_file_dir);
    qDebug() << "decompress_file:" << files;
    if (files.length() != 0) {
        QFile::remove(zip_file_name);
    }

    QProcess process;
    QString programPath = zip_file_dir + "/" + settings.value("app/entry").toString();
    QStringList arguments;
    process.setWorkingDirectory(zip_file_dir);
    process.start(programPath, arguments);

    if (!process.waitForStarted()) {
        qDebug() << "Failed to start program:" << programPath;
    }

    if (!process.waitForFinished(-1)) {
        qDebug() << "Program did not finish within the specified time limit.";
    }

    QByteArray standard_output = process.readAllStandardOutput();
    QByteArray error_output = process.readAllStandardError();

    qDebug() << "standard_output:" << standard_output;
    qDebug() << "error_output:" << error_output;
    clean_dir(random_suffix);
    return 0;
    //    return a.exec();
}

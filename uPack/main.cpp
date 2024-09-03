#include "quazip/JlCompress.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QStringList>

static bool compress_file(const QString &output, const QString &file) {
    //    JlCompress compress;
    return JlCompress::compressFile(output, file);
}
static bool compress_dir(const QString &output, const QString &file_dir) {
    //    JlCompress compress;
    return JlCompress::compressDir(output, file_dir);
}

int main(int argc, char *argv[]) {
    //    QCoreApplication a(argc, argv);
    QStringList args;
    if (argc < 3) {
        qDebug() << "usage:";
        qDebug() << "\tuPack output.zip input.dir";
        qDebug() << "\tuPack output.zip input.file";
        qDebug() << "\tdon't use uPack output.zip .";

        return 0;
    }
    for (auto i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    QString path = args[2];
    //    QFile src(path);
    //    if (!src.exists()) {
    //        qDebug() << "can't open file:" << path;
    //        return 0;
    //    }
    QFileInfo f(path);
    if (f.isDir()) {
        if (!compress_dir(args[1], args[2])) {
            qDebug() << "compress failed";
            return 0;
        }
    }
    if (f.isFile()) {
        if (!compress_file(args[1], args[2])) {
            qDebug() << "compress failed";
            return 0;
        }
    }
    qDebug() << "compress ok";
    return 0;
    //    return a.exec();
}

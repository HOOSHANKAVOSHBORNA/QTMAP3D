#include <QDebug>
#include <QDir>
#include <iostream>

#include "logManager.h"

LogManager::LogManager(QObject *parent)
    : QObject(parent)
{
    // directory existence checking
    QDir dir;
    if (!dir.exists(getSavingFolderPath())) {
        dir.mkpath(getSavingFolderPath());
    }

    // TODO: purpose that no log file deleted in the middle!!
    int i = 1;
    while (true) {
        QFile logFile(getSavingFilePath() + QString::number(i).rightJustified(3, '0'));

        if (!logFile.exists()) {
            break;
        }

        i++;
    }

    savedFileName = savedFileName + QString::number(i - 1).rightJustified(3, '0');

    QFile logFile(getSavingFilePath());

    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        std::cout << "here: " << getSavingFilePath().toStdString() << std::endl;
        std::cout << "Cannot open log file!!!" << std::endl;
        return;
    }

    QTextStream out(&logFile);

    out << "\n";
    out << "=================== App Started at: "
        << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ")
        << "===================";
    out << "\n";

    out.flush();

    logFile.close();
}

LogManager::~LogManager() {}

QString LogManager::getSavingFolderPath()
{
    return appDir + "/" + appName + "/" + savedDir;
}

QString LogManager::getSavingFilePath()
{
    // my system path: home/.local/share/LoggingExample/logs/mainLogs.txt
    return getSavingFolderPath() + "/" + savedFileName;
}

void LogManager::messageHandler(QtMsgType type,
                                const QMessageLogContext &context,
                                const QString &msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        if (std::strcmp(context.category, "qml") == 0) { // category is qml
            txt += "qml: ";
        } else {
            txt += "cpp: ";
        }
        txt += QString("%1").arg(msg);
        std::cout << txt.toStdString() << std::endl;
        return;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    case QtInfoMsg:
        txt = QString("Info: %1").arg(msg);
        break;
    }

    txt += QDateTime::currentDateTime().toString(" yyyy-MM-dd hh:mm:ss");

    writeLogToFile(txt);
}

bool LogManager::writeLogToFile(QString textToWrite)
{
    QFile *outFile = new QFile(getSavingFilePath());

    if (outFile->size() > maxSize) {
        QString newName = savedFileName;
        int newIndex = newName.right(3).toInt() + 1;
        newName.remove(newName.size() - 3, newName.size() - 1);
        newName += QString::number(newIndex).rightJustified(3, '0');
        savedFileName = newName;
    }

    if (!outFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
        std::cout << "Cannot open log file!!!";
        return false;
    }

    QTextStream ts(outFile);
    ts << textToWrite << '\n';
    ts.flush();

    outFile->close();
    delete outFile;

    return true;
}

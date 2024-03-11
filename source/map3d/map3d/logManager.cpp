#include <QDebug>
#include <QDir>
#include <iostream>

#include "logManager.h"

LogManager::LogManager(QObject *parent)
    : QObject(parent)
{
    QString toWrite = QString("\n\n=================== App Started at:"
                              + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                              + "===================");
    writeLogToFile(toWrite);
}

LogManager::~LogManager() {}

void LogManager::setNewSavedFileName()
{
    savedFileName = QString::number(QDateTime::currentSecsSinceEpoch());
}

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
    if (std::strcmp(context.category, "qml") == 0) { // category is qml
        txt += "qml: ";
    } else {
        txt += "cpp: ";
    }

    switch (type) {
    case QtDebugMsg:
        return;
    case QtWarningMsg:
        txt += "Warning: %1";
        break;
    case QtCriticalMsg:
        txt += "Critical: %1";
        break;
    case QtFatalMsg:
        txt += "Fatal: %1";
        abort();
    case QtInfoMsg:
        txt += "Info: %1";
        break;
    }

    txt += QString("%1").arg(msg);
    std::cout << txt.toStdString() << std::endl;

    txt += QDateTime::currentDateTime().toString(" yyyy-MM-dd hh:mm:ss");
    writeLogToFile(txt);
}

bool LogManager::writeLogToFile(QString textToWrite)
{
    QDir dir(getSavingFolderPath());

    if (!dir.exists()) {
        dir.mkpath(getSavingFolderPath());
    }

    dir.setSorting(QDir::Name);
    savedFileName = dir.entryList().last();

    if (".." == savedFileName) {
        setNewSavedFileName();
    }

    QFile *outFile = new QFile(getSavingFilePath());

    if (outFile->size() > maxSize) {
        delete outFile;
        setNewSavedFileName();
        outFile = new QFile(getSavingFilePath());
    }

    if (!outFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
        std::cout << "Cannot open log file!!!" << std::endl;
        return false;
    }

    QTextStream ts(outFile);
    ts << textToWrite << '\n';
    ts.flush();

    outFile->close();
    delete outFile;

    return true;
}

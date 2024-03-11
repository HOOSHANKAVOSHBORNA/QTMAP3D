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

    savedFileName = findFileName();
    writeLogToFile(toWrite);
}

LogManager::~LogManager() {}

void LogManager::setNewSavedFileName()
{
    savedFileName = QString::number(QDateTime::currentSecsSinceEpoch());
}

QString LogManager::findFileName()
{
    QString founded = "";

    QDir dir(getSavingFolderPath());

    if (!dir.exists()) {
        dir.mkpath(getSavingFolderPath());
        return founded;
    }

    int maxNum = -1;
    int num = 0;
    bool isInt = false;
    for (int i = 0; i < dir.entryList().size(); ++i) {
        num = dir.entryList().at(i).toInt(&isInt);
        if (!isInt) {
            continue;
        }

        if (num > maxNum) {
            maxNum = num;
        }
    }

    if (maxNum != -1) {
        founded = QString::number(maxNum);
    }

    return founded;
}

QString LogManager::getSavingFolderPath()
{
    return appDir + "/" + appName + "/" + savedDir;
}

QString LogManager::getSavingFilePath()
{
    // my system path: home/.local/share/LoggingExample/logs/234234343.txt
    return getSavingFolderPath() + "/" + savedFileName;
}

void LogManager::messageHandler(QtMsgType type,
                                const QMessageLogContext &context,
                                const QString &msg)
{
    QString txt;
    if (std::strcmp(context.category, "qml") == 0) { // category is qml
        txt += "qml-";
    } else {
        txt += "cpp-";
    }

    switch (type) {
    case QtDebugMsg:
        return;
    case QtWarningMsg:
        txt += "Warning: ";
        break;
    case QtCriticalMsg:
        txt += "Critical: ";
        break;
    case QtFatalMsg:
        txt += "Fatal: ";
        abort();
    case QtInfoMsg:
        txt += "Info: ";
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

    if (savedFileName == "") {
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

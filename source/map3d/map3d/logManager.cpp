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

    QFile logFile(getSavingFilePath());

    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        std::cout << "Cannot open log file!!!";
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
    // DEBUG
    //    qDebug() << context.category << ", " << context.file << ", " << context.function << ", "
    //             << context.line << ", " << context.version;

    if (type == QtDebugMsg) {
        if (std::strcmp(context.category, "qml") == 0) { // cateogry is "qml"
            qDebug() << "debug qml: " << msg.toStdString();
        } else {
            qDebug() << "debug cpp: " << msg.toStdString();
        }

        return;
    }

    QFile logFile(getSavingFilePath());

    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        std::cout << "Cannot open log file!!!";
        return;
    }

    QTextStream out(&logFile);

    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type) {
    case QtInfoMsg:
        out << "INF ";
        break;
    case QtDebugMsg:
        out << "DBG ";
        break;
    case QtWarningMsg:
        out << "WRN ";
        break;
    case QtCriticalMsg:
        out << "CRT ";
        break;
    case QtFatalMsg:
        out << "FTL ";
        break;
    }

    out << context.category << ": " << msg << "\n";
    out.flush();

    logFile.close();
}

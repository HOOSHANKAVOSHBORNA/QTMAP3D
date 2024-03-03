#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QObject>
#include <QStandardPaths>

class LogManager : public QObject
{
    Q_OBJECT

public:
    LogManager(QObject *parent = nullptr);
    ~LogManager();

    static QString getSavingFolderPath();
    static QString getSavingFilePath();
    static void messageHandler(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &msg);

private:
    static const inline QString appDir = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    static inline const QString appName = QString("Map3d");
    static inline const QString savedDir = QString("logs");
    static inline const QString savedFileName = QString("mainLogs.txt");
};

#endif // LOGMANAGER_H

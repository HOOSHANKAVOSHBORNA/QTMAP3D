#ifndef CONNECTIONCONFIGURATION_H
#define CONNECTIONCONFIGURATION_H

#include <QObject>
#include <QQuickItem>
#include <QSettings>
//#include <QJsonObject>
//#include <QJsonDocument>
//#include <QStandardPaths>
//#include <QDir>

class ConnectionConfiguration:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ip READ getIp  WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(QString port READ getPort  WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString username READ getUsername  WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ getPassword  WRITE setPassword NOTIFY passwordChanged)

public:
   explicit ConnectionConfiguration(QObject *parent = nullptr);

    Q_INVOKABLE QString getIp() const;
    Q_INVOKABLE void setIp(const QString &newIp);

    Q_INVOKABLE QString getPort() const;
    Q_INVOKABLE void setPort(const QString &newPort);

    Q_INVOKABLE QString getUsername() const;
    Q_INVOKABLE void setUsername(const QString &newUsername);

    Q_INVOKABLE QString getPassword() const;
    Q_INVOKABLE void setPassword(const QString &newPassword);
    Q_INVOKABLE void saveSettings();

//    QJsonObject toJson();
    bool writeToFile();

signals:
    void ipChanged();
    void portChanged();
    void usernameChanged();
    void passwordChanged();

private:
    QString mIp;
    QString mPort;
    QString mUsername;
    QString mPassword;
    QSettings* mSettings;
//    QString appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//    const QString savedDir = QString("saved");
//    const QString savedFileName = QString("settings.json");
};

class ConnectionConfigurationManager:public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    static ConnectionConfigurationManager *createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);
    ~ConnectionConfigurationManager();
    Q_INVOKABLE ConnectionConfiguration *getConnectionConfiguration();
protected:
    ConnectionConfigurationManager(QObject *parent = nullptr); // -------------- protected constructor for singelton
private:
    static inline ConnectionConfigurationManager* mInstance{nullptr};
    ConnectionConfiguration *mConnectionConfiguration;
};

#endif // CONNECTIONCONFIGURATION_H

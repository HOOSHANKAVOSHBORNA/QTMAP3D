#ifndef CONNECTIONCONFIGURATION_H
#define CONNECTIONCONFIGURATION_H

#include "networkManager.h"
#include <QObject>
#include <QQuickItem>
#include <QSettings>

class ConnectionConfiguration:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ip READ getIp  WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(int port READ getPort  WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString username READ getUsername  WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ getPassword  WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(bool isConnected READ isConnected WRITE setIsConnected NOTIFY isConnectedChanged)

public:
   explicit ConnectionConfiguration(NetworkManager *networkManager, QObject *parent = nullptr);

    Q_INVOKABLE QString getIp() const;
    Q_INVOKABLE void setIp(const QString &newIp);

    Q_INVOKABLE int getPort() const;
    Q_INVOKABLE void setPort(int newPort);

    Q_INVOKABLE QString getUsername() const;
    Q_INVOKABLE void setUsername(const QString &newUsername);

    Q_INVOKABLE QString getPassword() const;
    Q_INVOKABLE void setPassword(const QString &newPassword);
    Q_INVOKABLE void saveSettings();

    bool isConnected() const;
    Q_INVOKABLE void setIsConnected(bool newIsConnected);

    Q_INVOKABLE void testConnection();

signals:
    void ipChanged();
    void portChanged();
    void usernameChanged();
    void passwordChanged();
    void isConnectedChanged();

private:
    QString mIp;
    int mPort;
    QString mUsername;
    QString mPassword;
    QSettings* mSettings;
    NetworkManager *mNetworkManager{nullptr};
    bool mIsConnected;
};

#endif // CONNECTIONCONFIGURATION_H

#include "connectionConfiguration.h"

ConnectionConfiguration::ConnectionConfiguration(NetworkManager *networkManager, QObject *parent)
    :QObject(parent),
    mNetworkManager(networkManager)
{
    mSettings = new QSettings("MySettings","Configuration");

    QString data = mSettings->value("configs/ip", "").toString();
    mIp = data;

    data = mSettings->value("configs/port", "").toString();
    mPort = data.toInt();

    data = mSettings->value("configs/username", "").toString();
    mUsername = data;

    data = mSettings->value("configs/password", "").toString();
    mPassword = data;

    setIsConnected(false);
    if(mNetworkManager){
        connect(mNetworkManager, &NetworkManager::connected, this, &ConnectionConfiguration::setIsConnected);
        mNetworkManager->setConfig(mIp, mPort, mUsername, mPassword);
    }
}

QString ConnectionConfiguration::getIp() const
{
    return mIp;
}

void ConnectionConfiguration::setIp(const QString &newIp)
{
    if (mIp == newIp)
        return;
    mIp = newIp;
    emit ipChanged();
}

int ConnectionConfiguration::getPort() const
{
    return mPort;
}

void ConnectionConfiguration::setPort(int newPort)
{
    if (mPort == newPort)
        return;
    mPort = newPort;
    emit portChanged();
}

QString ConnectionConfiguration::getUsername() const
{
    return mUsername;
}

void ConnectionConfiguration::setUsername(const QString &newUsername)
{
    if (mUsername == newUsername)
        return;
    mUsername = newUsername;
    emit usernameChanged();
}

QString ConnectionConfiguration::getPassword() const
{
    return mPassword;
}

void ConnectionConfiguration::setPassword(const QString &newPassword)
{
    if (mPassword == newPassword)
        return;
    mPassword = newPassword;
    emit passwordChanged();
}

void ConnectionConfiguration::saveSettings()
{
    mSettings->setValue("configs/ip", mIp);
    mSettings->setValue("configs/port", mPort);
    mSettings->setValue("configs/username", mUsername);
    mSettings->setValue("configs/password", mPassword);

    if(mNetworkManager){
        mNetworkManager->setConfig(mIp, mPort, mUsername, mPassword);
    }
    else
        setIsConnected(false);
}

void ConnectionConfiguration::clearUnsavedUiData()
{
    setIp(mSettings->value("configs/ip").toString());
    setPort(mSettings->value("configs/port").toInt());
    setUsername(mSettings->value("configs/username").toString());
    setPassword(mSettings->value("configs/password").toString());
}


bool ConnectionConfiguration::isConnected() const
{
    return mIsConnected;
}

void ConnectionConfiguration::setIsConnected(bool newIsConnected)
{
    mIsConnected = newIsConnected;
    emit isConnectedChanged();
}

void ConnectionConfiguration::testConnection()
{
    if(mNetworkManager){
        mNetworkManager->setConfig(mIp, mPort, mUsername, mPassword);
    }
    else
        setIsConnected(false);
}

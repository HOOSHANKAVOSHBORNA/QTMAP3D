#include "connectionConfiguration.h"

ConnectionConfiguration::ConnectionConfiguration(QObject *parent):QObject(parent)
{
    mSettings = new QSettings("MySettings","Configuration");

    QString data = mSettings->value("configs/ip", "").toString();
    mIp = data;

    data = mSettings->value("configs/port", "").toString();
    mPort = data;

    data = mSettings->value("configs/username", "").toString();
    mUsername = data;

    data = mSettings->value("configs/password", "").toString();
    mPassword = data;
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

QString ConnectionConfiguration::getPort() const
{
    return mPort;
}

void ConnectionConfiguration::setPort(const QString &newPort)
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
}

//QJsonObject Settings::toJson()
//{
//    QJsonObject json;

//    json["ip"] = mIp;
//    json["port"] = mPort;
//    json["username"] = mUsername;
//    json["password"] = mPassword;

//    return json;
//}

bool ConnectionConfiguration::writeToFile()
{


//    QDir dir;
//    if (!dir.exists(appDir + "/" + savedDir)) {
//        dir.mkpath(savedDir);
//    }

//    QFile settingsFile(appDir + "/" + savedDir + "/" + savedFileName);

//    if (!settingsFile.open(QIODevice::WriteOnly)) {
//        return false;
//    }

//    QJsonObject jsonObject = this->toJson();
//    settingsFile.write(QJsonDocument(jsonObject).toJson());

   //    return true;
}


ConnectionConfigurationManager *ConnectionConfigurationManager::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(mInstance == nullptr){ mInstance = new ConnectionConfigurationManager(); }
    return mInstance;
}

ConnectionConfigurationManager::~ConnectionConfigurationManager()
{
    delete mConnectionConfiguration;
}

ConnectionConfiguration* ConnectionConfigurationManager::getConnectionConfiguration()
{
    return mConnectionConfiguration;
}

ConnectionConfigurationManager::ConnectionConfigurationManager(QObject *parent):QObject(parent)
{
    mConnectionConfiguration = new ConnectionConfiguration();
}


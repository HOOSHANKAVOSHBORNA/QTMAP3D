#include "settings.h"

Settings::Settings(QObject *parent):QObject(parent)
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

QString Settings::getIp() const
{
    return mIp;
}

void Settings::setIp(const QString &newIp)
{
    if (mIp == newIp)
        return;
    mIp = newIp;
    emit ipChanged();
}

QString Settings::getPort() const
{
    return mPort;
}

void Settings::setPort(const QString &newPort)
{
    if (mPort == newPort)
        return;
    mPort = newPort;
    emit portChanged();
}

QString Settings::getUsername() const
{
    return mUsername;
}

void Settings::setUsername(const QString &newUsername)
{
    if (mUsername == newUsername)
        return;
    mUsername = newUsername;
    emit usernameChanged();
}

QString Settings::getPassword() const
{
    return mPassword;
}

void Settings::setPassword(const QString &newPassword)
{
    if (mPassword == newPassword)
        return;
    mPassword = newPassword;
    emit passwordChanged();
}

void Settings::saveSettings()
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

bool Settings::writeToFile()
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


SettingsManager *SettingsManager::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(mInstance == nullptr){ mInstance = new SettingsManager(); }
    return mInstance;
}

SettingsManager::~SettingsManager()
{
    delete mSettings;
}

Settings* SettingsManager::getSettings()
{
    return mSettings;
}

SettingsManager::SettingsManager(QObject *parent):QObject(parent)
{
    mSettings = new Settings();
}


#include "settings.h"

Settings::Settings(QObject *parent):QObject(parent)
{

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
    emit settingsChanged();
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
    emit settingsChanged();
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
    emit settingsChanged();
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
    emit settingsChanged();
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




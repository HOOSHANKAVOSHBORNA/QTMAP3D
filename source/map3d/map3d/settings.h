#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QQuickItem>


class Settings:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString ip READ getIp  WRITE setIp NOTIFY settingsChanged)
    Q_PROPERTY(QString port READ getPort  WRITE setPort NOTIFY settingsChanged)
    Q_PROPERTY(QString username READ getUsername  WRITE setUsername NOTIFY settingsChanged)
    Q_PROPERTY(QString password READ getPassword  WRITE setPassword NOTIFY settingsChanged)

public:
   explicit Settings(QObject *parent = nullptr);

    Q_INVOKABLE QString getIp() const;
    Q_INVOKABLE void setIp(const QString &newIp);

    Q_INVOKABLE QString getPort() const;
    Q_INVOKABLE void setPort(const QString &newPort);

    Q_INVOKABLE QString getUsername() const;
    Q_INVOKABLE void setUsername(const QString &newUsername);

    Q_INVOKABLE QString getPassword() const;
    Q_INVOKABLE void setPassword(const QString &newPassword);

signals:
    void settingsChanged();

private:
    QString mIp;
    QString mPort;
    QString mUsername;
    QString mPassword;
};

class SettingsManager:public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    static SettingsManager *createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);
    ~SettingsManager();
    Q_INVOKABLE Settings *getSettings();
protected:
    SettingsManager(QObject *parent = nullptr); // -------------- protected constructor for singelton
private:
    static inline SettingsManager* mInstance{nullptr};
    Settings *mSettings;
};

#endif // SETTINGS_H

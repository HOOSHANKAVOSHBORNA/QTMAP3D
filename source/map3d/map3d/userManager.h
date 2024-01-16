#ifndef USERMANAGER_H
#define USERMANAGER_H
#include "qqmlapplicationengine.h"
#include "serviceManager.h"

#include <QQuickWindow>

class QQmlEngine;
class LoginPage : public QQuickWindow
{
    Q_OBJECT
public:
    LoginPage(QWindow *parent = nullptr);

    void setServiceManager(ServiceManager *newServiceManager);

public slots:
    void signIn(const QString username, const QString password);
//    void signUp(const QString username, const QString password);

signals:
    void signedIn();

protected:
    void closeEvent(QCloseEvent *) override;
private:
    void onUserDataReceived(const UserData &userData);
private:
    QQmlEngine* mQmlEngine{nullptr};

    ServiceManager* mServiceManager{nullptr};
    UserData mLoginUserData;

};

class UserManager: public QObject
{
    Q_OBJECT

public:
    UserManager(ServiceManager *serviceManger,QQmlApplicationEngine *qmlEngine, QObject *parent = nullptr);

signals:
    void signedIn();

private:
    void onQmlObjectCreated(QObject *obj, const QUrl &objUrl);

private:
    ServiceManager *mServiceManager{nullptr};
    QQmlApplicationEngine *mQmlEngine{nullptr};
    LoginPage *mLoginPage{nullptr};

};

class Profile:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName  WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString username READ getUsername  WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ getPassword  WRITE setPassword NOTIFY passwordChanged)

public:
    explicit Profile(QObject *parent = nullptr){}

   Q_INVOKABLE QString getName() const;
   Q_INVOKABLE void setName(const QString &newName);
   Q_INVOKABLE QString getUsername() const;
   Q_INVOKABLE void setUsername(const QString &newUsername);
   Q_INVOKABLE QString getPassword() const;
   Q_INVOKABLE void setPassword(const QString &newPassword);

signals:
    void nameChanged();

    void usernameChanged();

    void passwordChanged();

private:
    QString mName;
    QString mUsername;
    QString mPassword;
};

class ProfileManager:public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    static ProfileManager *createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);
    ~ProfileManager();
    Q_INVOKABLE Profile *getProfile();
protected:
    ProfileManager(QObject *parent = nullptr); // -------------- protected constructor for singelton
private:
    static inline ProfileManager* mInstance{nullptr};
    Profile *mProfile;
};

#endif // USERMANAGER_H

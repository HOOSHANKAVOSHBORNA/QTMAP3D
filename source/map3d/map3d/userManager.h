#ifndef USERMANAGER_H
#define USERMANAGER_H
#include "qqmlapplicationengine.h"
#include "serviceManager.h"
#include "loadingInfo.h"
#include <QQuickItem>
#include <QQuickWindow>

class QQmlEngine;
class LoginPage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool windowHidden READ windowHidden WRITE setWindowHidden NOTIFY windowHiddenChanged)

public:
    LoginPage(ServiceManager *serviceManager,QQmlApplicationEngine *qmlEngine, QObject *parent = nullptr);

    void setServiceManager(ServiceManager *newServiceManager);

   Q_INVOKABLE void signIn(const QString username, const QString password);

   bool windowHidden() const;
   void setWindowHidden(bool newWindowHidden);

   signals:
    void signedIn();
    void windowHiddenChanged();

   protected:
   Q_INVOKABLE  void onWindowClosed();
private:
    void onUserDataReceived(const UserData &userData);
private:
    QQmlEngine* mQmlEngine{nullptr};
    ServiceManager* mServiceManager{nullptr};
    UserData mLoginUserData;
    QQuickWindow* mSettingsWindow;

    bool mWindowHidden{false};
};

class Profile:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName  WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString username READ getUsername  WRITE setUsername NOTIFY usernameChanged)

public:


    explicit Profile(ServiceManager *serviceManager, QObject *parent = nullptr);

    QString getName() const;
    void setName(const QString &newName);
    QString getUsername() const;
    void setUsername(const QString &newUsername);
    Q_INVOKABLE void logOut();


signals:
    void nameChanged();
    void usernameChanged();

private:
    ServiceManager* mServiceManager{nullptr};
    QString mName{"Alireza Nabati"};
    QString mUsername{"Alirez98"};
};




class UserManager: public QObject
{
    Q_OBJECT

public:
    UserManager(ServiceManager *serviceManager,QQmlApplicationEngine *qmlEngine, QObject *parent = nullptr);

signals:
    void signedIn();


private:
    ServiceManager *mServiceManager{nullptr};
    QQmlApplicationEngine *mQmlEngine{nullptr};
    LoginPage *mLoginPage{nullptr};
    Profile *mProfile;
    LoadingInfo *mLoadingInfo;
};



#endif // USERMANAGER_H

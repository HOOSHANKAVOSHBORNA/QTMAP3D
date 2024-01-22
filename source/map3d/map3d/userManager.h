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
    UserManager(ServiceManager *serviceManger,QQmlApplicationEngine *qmlEngine, QObject *parent = nullptr);

signals:
    void signedIn();

private:
    void onQmlObjectCreated(QObject *obj, const QUrl &objUrl);

private:
    ServiceManager *mServiceManager{nullptr};
    QQmlApplicationEngine *mQmlEngine{nullptr};
    LoginPage *mLoginPage{nullptr};
    Profile *mProfile;
};



#endif // USERMANAGER_H

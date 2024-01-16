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

#endif // USERMANAGER_H

#ifndef USERMANAGER_H
#define USERMANAGER_H
#include "qqmlapplicationengine.h"
#include "serviceManager.h"
#include "loadingPage.h"
#include "connectionConfiguration.h"
#include <QQuickItem>
#include <QQuickWindow>

class QQmlEngine;

class RoleSelectionModel: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit RoleSelectionModel(ServiceManager *serviceManager, QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void signIn(const QString username, const QString password);
    Q_INVOKABLE int getSelectedRoleIndex(int index);

signals:
    void signedIn();

private:
    std::vector<QString> mRoleNames;
    ServiceManager* mServiceManager{nullptr};
};

class LoginPage : public QObject
{
    Q_OBJECT

public:
    LoginPage(ServiceManager *serviceManager,QQmlApplicationEngine *qmlEngine, QObject *parent = nullptr);

    void setServiceManager(ServiceManager *newServiceManager);
    Q_INVOKABLE void signIn(const QString username, const QString password);

   signals:
    void signedIn();

   protected:
   Q_INVOKABLE  void onWindowClosed();
private:
    void onUserDataReceived(const UserData &userData);
private:
    QQmlEngine* mQmlEngine{nullptr};
    ServiceManager* mServiceManager{nullptr};
    UserData mLoginUserData;
    RoleSelectionModel* mRoleSelectionModel;
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
    LoadingPage *mLoadingInfo;
};



#endif // USERMANAGER_H

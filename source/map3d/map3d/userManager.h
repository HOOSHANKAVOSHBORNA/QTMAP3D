#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "qabstractitemmodel.h"
#include "serviceManager.h"

class QQmlEngine;

class RoleSelectionModel: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit RoleSelectionModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    void setRolse(QVector<QString> roles);
    void clear();

private:
    QVector<QString> mRoles;
};

class UserManager : public QObject
{
    Q_OBJECT

public:
    UserManager(ServiceManager *serviceManager, QObject *parent = nullptr);

    void setServiceManager(ServiceManager *newServiceManager);

    Q_INVOKABLE void signIn(const QString username, const QString password);
    Q_INVOKABLE void signIn(int selectRoleIndex);

    Q_INVOKABLE UserData userData() const;

    Q_INVOKABLE RoleSelectionModel *roleSelectionModel() const;

signals:
    void signedIn();
    void selectRole();

private slots:
    void onUserDataReceived(const UserData &userData);

private:
    ServiceManager* mServiceManager{nullptr};
    UserData mUserData;
    RoleSelectionModel* mRoleSelectionModel;
};

//class Profile:public QObject
//{
//    Q_OBJECT

//    Q_PROPERTY(QString name READ getName  WRITE setName NOTIFY nameChanged)
//    Q_PROPERTY(QString username READ getUsername  WRITE setUsername NOTIFY usernameChanged)

//public:


//    explicit Profile(ServiceManager *serviceManager, QObject *parent = nullptr);

//    QString getName() const;
//    void setName(const QString &newName);
//    QString getUsername() const;
//    void setUsername(const QString &newUsername);
//    Q_INVOKABLE void logOut();


//signals:
//    void nameChanged();
//    void usernameChanged();

//private:
//    ServiceManager* mServiceManager{nullptr};
//    QString mName{"Alireza Nabati"};
//    QString mUsername{"Alirez98"};
//};


//class UserManager: public QObject
//{
//    Q_OBJECT

//public:
//    UserManager(ServiceManager *serviceManager,QQmlApplicationEngine *qmlEngine, QObject *parent = nullptr);

//signals:
//    void signedIn();


//private:
//    ServiceManager *mServiceManager{nullptr};
//    QQmlApplicationEngine *mQmlEngine{nullptr};
//    LoginPage *mLoginPage{nullptr};
//    Profile *mProfile;
//    LoadingPage *mLoadingInfo;
//};



#endif // USERMANAGER_H

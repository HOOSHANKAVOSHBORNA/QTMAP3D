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

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString loginMessage READ loginMessage WRITE setLoginMessage NOTIFY loginMessageChanged)
    Q_PROPERTY(QString roleMessage READ roleMessage WRITE setRoleMessage NOTIFY roleMessageChanged)
    Q_PROPERTY(bool rolePageVisible READ rolePageVisible WRITE setRolePageVisible NOTIFY selectRole)
    Q_PROPERTY(bool isConnected READ isConnected WRITE setIsConnected NOTIFY isConnectedChanged)


public:
    UserManager(ServiceManager *serviceManager, QObject *parent = nullptr);

    void setServiceManager(ServiceManager *newServiceManager);

    Q_INVOKABLE void signIn(const QString username, const QString password);
    Q_INVOKABLE void signIn(int selectRoleIndex);

    Q_INVOKABLE UserData userData() const;

    Q_INVOKABLE RoleSelectionModel *roleSelectionModel() const;


    QString name() const;
    void setName(const QString &newName);

    QString userName() const;
    void setUserName(const QString &newUserName);

    QString loginMessage() const;
    void setLoginMessage(const QString &newMessage);

    bool rolePageVisible() const;
    Q_INVOKABLE void setRolePageVisible(bool newRolePageVisible);

    QString roleMessage() const;
    void setRoleMessage(const QString &newRoleMessage);

    bool isConnected() const;
    void setIsConnected(bool newIsConnected);

signals:
    void signedIn();
    void selectRole();

    void userDataChanged();

    void nameChanged();

    void userNameChanged();

    void loginMessageChanged();

    void roleMessageChanged();

    void isConnectedChanged();

private slots:
    void onUserDataReceived(const UserData &userData);

private:
    ServiceManager* mServiceManager{nullptr};
    UserData mUserData;
    RoleSelectionModel* mRoleSelectionModel;
    QString mName;
    QString mUserName;
    QString mLoginMessage;
    bool mRolePageVisible;
    QString mRoleMessage;
    bool mIsConnected;
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

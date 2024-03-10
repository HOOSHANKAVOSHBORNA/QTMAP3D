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
    Q_PROPERTY(QString roleName READ roleName WRITE setRoleName NOTIFY roleNameChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)

private:
    explicit UserManager();

public:
    static UserManager *instance();
    void initialize(ServiceManager *serviceManager, QObject *parent = nullptr);

    void setServiceManager(ServiceManager *newServiceManager);

    Q_INVOKABLE void signIn(const QString username, const QString password);
    Q_INVOKABLE void signIn(int selectRoleIndex);

    Q_INVOKABLE UserData userData() const;

    Q_INVOKABLE RoleSelectionModel *roleSelectionModel() const;

    QString name() const;
    void setName(const QString &newName);

    QString userName() const;
    void setUserName(const QString &newUserName);

    QString message() const;
    Q_INVOKABLE void setMessage(const QString &newMessage);

    void setIsConnected(bool newIsConnected);

    QString roleName() const;
    void setRoleName(const QString &newRoleName);

signals:
    void signedIn();
    void authenticationFailed();
    void signedOut();
    void selectRole();
    void signInFailed();

    void userDataChanged();

    void nameChanged();
    void userNameChanged();
    void messageChanged();

    void roleNameChanged();

private slots:
    void onUserDataReceived(const UserData &userData);

private:
    static inline UserManager *mUserManager = nullptr;
    ServiceManager *mServiceManager = nullptr;
    UserData mUserData;
    RoleSelectionModel *mRoleSelectionModel;
    QString mName;
    QString mUserName;
    QString mMessage;
    bool mRolePageVisible;

    QString mRoleName;
};

#endif // USERMANAGER_H

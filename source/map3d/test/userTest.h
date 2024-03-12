#ifndef USERTEST_H
#define USERTEST_H

#include "serviceManager.h"

class UserTest: public QObject
{
    Q_OBJECT
public:
    UserTest(ServiceManager *serviceManager);

private slots:
    void onUserDataReceived(const UserData& userData);

signals:
    void login();
    void logout();

private:
    ServiceManager *mServiceManager;
    QVector<UserData> mUserDatas;
    // UserData mUserData1;
    // UserData mUserData2;
    bool mIsLogin{false};
};

#endif // USERTEST_H

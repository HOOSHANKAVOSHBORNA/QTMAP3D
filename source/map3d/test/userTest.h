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

private:
    ServiceManager *mServiceManager;
    UserData mUserData;
};

#endif // USERTEST_H

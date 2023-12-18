#ifndef LOGINTEST_H
#define LOGINTEST_H
#include "networkManager.h"

class LoginTest
{
public:
    LoginTest(NetworkManager *networkManager);

private:
    NetworkManager *mNetworkManager{nullptr};
};

#endif // LOGINTEST_H

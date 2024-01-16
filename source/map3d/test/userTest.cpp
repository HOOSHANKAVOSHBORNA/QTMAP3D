#include "userTest.h"


UserTest::UserTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    mUserData.name = "Test Name";
    mUserData.userName = "testUser";
    mUserData.password = "1234";
    QObject::connect(mServiceManager, &ServiceManager::userDataReceived, this, &UserTest::onUserDataReceived);
}

void UserTest::onUserDataReceived(const UserData &userData)
{

    if(userData.userName == mUserData.userName
        && userData.password == mUserData.password){
        mUserData.response.status = Response::Status::Success;
        mUserData.response.message = "";
    }
    else{
        mUserData.response.status = Response::Status::Failed;
        mUserData.response.message = "User name or password is wrong.";
    }
    mServiceManager->sendUser(mUserData);
}

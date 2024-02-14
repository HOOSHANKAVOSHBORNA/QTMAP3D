#include "userTest.h"


UserTest::UserTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    mUserData.name = "Test Name";
    mUserData.userName = "test1";
    mUserData.password = "1234";

    QObject::connect(mServiceManager, &ServiceManager::userDataReceived, this, &UserTest::onUserDataReceived);
}

void UserTest::onUserDataReceived(const UserData &userData)
{
    qDebug()<<"test received user: "<<userData.toJson();
    if(userData.userName == mUserData.userName
        && userData.password == mUserData.password){
        mUserData.response.status = Response::Status::Success;
        mUserData.response.message = "";
        if(userData.command == UserData::UserCommand::Login){
            mUserData.roles.append("Admin");
            mUserData.roles.append("User");
            mUserData.roles.append("Reviewer");
        }
        else if(userData.command == UserData::UserCommand::Login){
            mUserData.selectRoleIndex = userData.selectRoleIndex;
        }
    }
    else{
        mUserData.response.status = Response::Status::Failed;
        mUserData.response.message = "User name or password is wrong.";
    }
    mUserData.command = userData.command;
    mServiceManager->sendUser(mUserData);
}

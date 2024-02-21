#include "userTest.h"


UserTest::UserTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    mUserData1.name = "Test1 Name";
    mUserData1.userName = "test1";
    mUserData1.password = "1234";
    mUserData1.roles.append("Admin");
    mUserData1.roles.append("User");
    mUserData1.roles.append("Reviewer");

    mUserData2.name = "Test2 Name";
    mUserData2.userName = "test2";
    mUserData2.password = "1234";

    QObject::connect(mServiceManager, &ServiceManager::userDataReceived, this, &UserTest::onUserDataReceived);
}

void UserTest::onUserDataReceived(const UserData &userData)
{
    qDebug()<<"test received user: "<<userData.toJson();
    mIsLogin = false;
    UserData responsUserData = userData;

    if(userData.userName == mUserData1.userName){
        if(userData.password == mUserData1.password){
            responsUserData.response.status = Response::Status::Success;
            responsUserData.response.message = "";
            if(userData.command == UserData::UserCommand::Login){
                responsUserData.roles.clear();
                responsUserData.roles = mUserData1.roles;
            }
            else if(userData.command == UserData::UserCommand::SelectRole){
                mUserData1.selectRoleIndex = userData.selectRoleIndex;
                mIsLogin = true;
                emit login();
            }
            else if(userData.command == UserData::UserCommand::Logout)
                emit logout();
        }
        else{
            responsUserData.response.status = Response::Status::Failed;
            responsUserData.response.message = "Password is wrong.";
        }
    }
    else if(userData.userName == mUserData2.userName){
        if(userData.password == mUserData2.password){
            responsUserData.response.status = Response::Status::Success;
            responsUserData.response.message = "";
            if(userData.command == UserData::UserCommand::Login){
                responsUserData.roles.clear();
                responsUserData.roles = mUserData2.roles;
                mIsLogin = true;
                emit login();
            }
            else if(userData.command == UserData::UserCommand::SelectRole){
                mUserData2.selectRoleIndex = userData.selectRoleIndex;
            }
            else if(userData.command == UserData::UserCommand::Logout)
                emit logout();
        }
        else{
            responsUserData.response.status = Response::Status::Failed;
            responsUserData.response.message = "Password is wrong.";
        }
    }
    else{
        responsUserData.response.status = Response::Status::Failed;
        responsUserData.response.message = "User name is wrong.";
    }

    mServiceManager->sendUser(responsUserData);
}

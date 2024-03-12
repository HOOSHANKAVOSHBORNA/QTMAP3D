#include "userTest.h"


UserTest::UserTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    //----------- user data 1 ----------------
    UserData userData1;
    userData1.name = "Test1 Name";
    userData1.userName = "test1";
    userData1.password = "1234";
    userData1.roles.append("Admin");
    userData1.roles.append("User");
    userData1.roles.append("Reviewer");

    //----------- user data 2 ----------------
    UserData userData2;
    userData2.name = "Test2 Name";
    userData2.userName = "test2";
    userData2.password = "1234";

    mUserDatas.append(userData1);
    mUserDatas.append(userData2);

    QObject::connect(mServiceManager, &ServiceManager::userDataReceived, this, &UserTest::onUserDataReceived);
}

void UserTest::onUserDataReceived(const UserData &userData)
{
    qDebug()<<"test received user: "<<userData.toJson();
    mIsLogin = false;
    UserData responsUserData = userData;

    if(userData.command == UserData::UserCommand::Logout) {
        emit logout();
    }
    // find user data
    auto it = std::find(mUserDatas.begin(), mUserDatas.end(), userData);
    if(it != mUserDatas.end()){
        if(userData.password == it->password){
            responsUserData.name = it->name;
            responsUserData.response.status = Response::Status::Success;
            responsUserData.response.message = "";
            if(userData.command == UserData::UserCommand::Login){
                responsUserData.roles.clear();
                responsUserData.roles = it->roles;
            }
            else if(userData.command == UserData::UserCommand::SelectRole){
                it->selectRoleIndex = userData.selectRoleIndex;
                mIsLogin = true;
                emit login();
            }
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

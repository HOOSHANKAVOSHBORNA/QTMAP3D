#include "userManager.h"

RoleSelectionModel::RoleSelectionModel(QObject *parent):
    QAbstractListModel(parent)
{
}

int RoleSelectionModel::rowCount(const QModelIndex &parent) const
{
    return mRoles.size();
}

QVariant RoleSelectionModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();

    switch(role) {
    case Qt::DisplayRole:
        return mRoles[row];
    default:
        break;
    }

    return QVariant(false);
}

void RoleSelectionModel::setRolse(QVector<QString> roles)
{
    beginResetModel();
    mRoles = roles;
    endResetModel();
}

void RoleSelectionModel::clear()
{
    beginResetModel();
    mRoles.clear();
    endResetModel();
}

//--user manager----------------------------------------------------------------------
UserManager::UserManager(ServiceManager* serviceManager, QObject *parent):
    QObject(parent), mServiceManager{serviceManager}
{
    mRoleSelectionModel = new RoleSelectionModel(serviceManager);
    connect(mServiceManager, &ServiceManager::userDataReceived, this, &UserManager::onUserDataReceived);
}

void UserManager::signIn(const QString username, const QString password)
{
    qDebug()<<"signIn LoginPage";
    UserData userData;
    userData.userName = username;
    userData.password = password;
    userData.command = UserData::UserCommand::Login;
    mServiceManager->sendUser(userData);

    setUserName(username);
    emit selectRole();
    setMessage("Wrong username or password");

    //--test------
    QVector<QString> testvec;
    testvec.append("Admin");
    testvec.append("User");
    mRoleSelectionModel->setRolse(testvec);
}

void UserManager::signIn(int selectRoleIndex)
{
    qDebug()<<"signIn Select Role: "<<selectRoleIndex;
    mUserData.selectRoleIndex = selectRoleIndex;
    mUserData.command = UserData::UserCommand::SelectRole;
    mServiceManager->sendUser(mUserData);

}

UserData UserManager::userData() const
{
    return mUserData;
}


void UserManager::setServiceManager(ServiceManager *newServiceManager)
{
    if(mServiceManager)
        disconnect(mServiceManager, nullptr, this, nullptr);
    mServiceManager = newServiceManager;
    connect(mServiceManager, &ServiceManager::userDataReceived, this, &UserManager::onUserDataReceived);
}

void UserManager::onUserDataReceived(const UserData &userData)
{
    qDebug()<<"map3d received user: "<<userData.toJson();

    if(userData.response.status == Response::Status::Success){
        mUserData = userData;
        if(userData.command == UserData::UserCommand::Login){
            if(mUserData.roles.isEmpty())
                emit signedIn();
            else{
                mRoleSelectionModel->setRolse(mUserData.roles);
                emit selectRole();
            }
        }
        else if(userData.command == UserData::UserCommand::SelectRole){
            emit signedIn();
        }
    }
}

RoleSelectionModel *UserManager::roleSelectionModel() const
{
    return mRoleSelectionModel;
}

QString UserManager::name() const
{
    return mName;
}

void UserManager::setName(const QString &newName)
{
    if (mName == newName)
        return;
    mName = newName;
    emit nameChanged();
}

QString UserManager::userName() const
{
    return mUserName;
}

void UserManager::setUserName(const QString &newUserName)
{
    if (mUserName == newUserName)
        return;
    mUserName = newUserName;
    emit userNameChanged();
}

QString UserManager::message() const
{
    return mMessage;
}

void UserManager::setMessage(const QString &newMessage)
{
    if (mMessage == newMessage)
        return;
    mMessage = newMessage;
    emit messageChanged();
}


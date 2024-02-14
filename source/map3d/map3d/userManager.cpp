#include "userManager.h"
#include "connectionConfiguration.h"
//#include "qcryptographichash.h"
#include <QQmlContext>
#include <QApplication>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QJsonObject>

LoginPage::LoginPage(ServiceManager* serviceManager, QQmlApplicationEngine *qmlEngine, QObject *parent):
    QObject(parent), mServiceManager{serviceManager},mQmlEngine{qmlEngine}
{
    mRoleSelectionModel = new RoleSelectionModel(serviceManager);
    mQmlEngine->rootContext()->setContextProperty("roleSelectionModel",mRoleSelectionModel);
}

void LoginPage::signIn(const QString username, const QString password)
{
    qDebug()<<"signIn";
    UserData userData;
    userData.userName = username;
    userData.password = password;
    userData.command = UserData::UserCommand::Login;
    mServiceManager->sendUser(userData);

//    QByteArray pswNsalt (password.toStdString().c_str()) ;
//    pswNsalt = QCryptographicHash::hash(pswNsalt, QCryptographicHash::Md5).toHex();

}

void LoginPage::onWindowClosed()
{
    QCoreApplication::exit(-1);
}

void LoginPage::onUserDataReceived(const UserData &userData)
{
    qDebug()<<userData.response.message;
    if(userData.response.status == Response::Status::Success){
        mLoginUserData = userData;
        emit signedIn();
    }
}

void LoginPage::setServiceManager(ServiceManager *newServiceManager)
{
    if(mServiceManager)
        disconnect(mServiceManager, nullptr, this, nullptr);
    mServiceManager = newServiceManager;
    connect(mServiceManager, &ServiceManager::userDataReceived, this, &LoginPage::onUserDataReceived);
}

UserManager::UserManager(ServiceManager *serviceManager, QQmlApplicationEngine *qmlEngine, QObject *parent)
    : QObject(parent),
    mServiceManager(serviceManager),
    mQmlEngine(qmlEngine)
{
    mProfile = new Profile(serviceManager);
    qmlEngine->rootContext()->setContextProperty("UserInfo", mProfile);
    mLoginPage = new LoginPage(serviceManager,qmlEngine);
    qmlEngine->rootContext()->setContextProperty("loginPage", mLoginPage);
    mLoadingInfo = new LoadingPage();
    qmlEngine->rootContext()->setContextProperty("loadingInfo", mLoadingInfo);
}


Profile::Profile(ServiceManager *serviceManager, QObject *parent)
    : QObject(parent) , mServiceManager{serviceManager}
{

}

QString Profile::getName() const
{
    return mName;
}

void Profile::setName(const QString &newName)
{
    if (mName == newName)
        return;
    mName = newName;
    emit nameChanged();
}

QString Profile::getUsername() const
{
    return mUsername;
}

void Profile::setUsername(const QString &newUsername)
{
    if (mUsername == newUsername)
        return;
    mUsername = newUsername;
    emit usernameChanged();
}

void Profile::logOut()
{
  UserData userData;

  userData.name = mName;
  userData.userName = mUsername;
  userData.command = UserData::UserCommand::Logout;


  mServiceManager->sendUser(userData);
}

RoleSelectionModel::RoleSelectionModel(ServiceManager *serviceManager, QObject *parent):
QAbstractListModel(parent), mServiceManager{serviceManager}
{
  mRoleNames.push_back("Admin");
  mRoleNames.push_back("Administrator");
  mRoleNames.push_back("Reviewer");
}

int RoleSelectionModel::rowCount(const QModelIndex &parent) const
{
  return mRoleNames.size();
}

QVariant RoleSelectionModel::data(const QModelIndex &index, int role) const
{
  const int row = index.row();

  switch(role) {
  case Qt::DisplayRole:
        return mRoleNames[row];
  default:
        break;
  }

  return QVariant(false);
}



void RoleSelectionModel::signIn(const QString username, const QString password)
{
  qDebug()<<"signIn";
  UserData userData;
  userData.userName = username;
  userData.password = password;
  userData.command = UserData::UserCommand::Login;
  mServiceManager->sendUser(userData);

  //    QByteArray pswNsalt (password.toStdString().c_str()) ;
  //    pswNsalt = QCryptographicHash::hash(pswNsalt, QCryptographicHash::Md5).toHex();

  emit signedIn();
}

int RoleSelectionModel::getSelectedRoleIndex(int index)
{
  return index;
}

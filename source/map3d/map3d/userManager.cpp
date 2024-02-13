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

    setWindowHidden(true);
    emit signedIn();
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
        setWindowHidden(true);
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
    qmlRegisterSingletonType<ConnectionConfigurationManager>("Crystal", 1, 0,
                                   "ConnectionConfigurationInstance", ConnectionConfigurationManager::createSingletonInstance);
    mProfile = new Profile(serviceManager);
    qmlEngine->rootContext()->setContextProperty("UserInfo", mProfile);
    mLoginPage = new LoginPage(serviceManager,qmlEngine);
    qmlEngine->rootContext()->setContextProperty("loginPage", mLoginPage);
    mLoadingInfo = new LoadingInfo();
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



bool LoginPage::windowHidden() const
{
  return mWindowHidden;
}

void LoginPage::setWindowHidden(bool newWindowHidden)
{
  if (mWindowHidden == newWindowHidden)
        return;
  mWindowHidden = newWindowHidden;
  emit windowHiddenChanged();
}

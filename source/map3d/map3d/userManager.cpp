#include "userManager.h"
//#include "qcryptographichash.h"
#include <QQmlContext>
#include <QApplication>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QJsonObject>

LoginPage::LoginPage(QWindow *parent):
    QQuickWindow(parent)
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

    hide();
    emit signedIn();
}

void LoginPage::closeEvent(QCloseEvent *)
{
    QCoreApplication::exit(-1);
}

void LoginPage::onUserDataReceived(const UserData &userData)
{
    qDebug()<<userData.response.message;
    if(userData.response.status == Response::Status::Success){
        mLoginUserData = userData;
        hide();
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

UserManager::UserManager(ServiceManager *serviceManger,QQmlApplicationEngine *qmlEngine, QObject *parent)
    : QObject(parent),
    mServiceManager(serviceManger),
    mQmlEngine(qmlEngine)
{
    qmlRegisterType<LoginPage>("Crystal", 1, 0, "LoginPage");
    QObject::connect(mQmlEngine, &QQmlApplicationEngine::objectCreated,
                     this, &UserManager::onQmlObjectCreated,
                     Qt::DirectConnection);
    mQmlEngine->load(QStringLiteral("qrc:///LoginPage.qml"));
    mProfile = new Profile(serviceManger);
    qmlEngine->rootContext()->setContextProperty("UserInfo", mProfile);
//    qmlEngine->rootContext()->setContextProperty("LoginPage", mLoginPage);
}

void UserManager::onQmlObjectCreated(QObject *obj, const QUrl &objUrl)
{
    if(!obj){
        qDebug()<<"Can not create: "<< objUrl.toString();
        QCoreApplication::exit(-1);
        return;
    }


    LoginPage *loginPage = qobject_cast<LoginPage*>(obj);

    if (loginPage){
        qDebug()<<"Load: "<< objUrl.toString();
        mLoginPage = loginPage;

        connect(mLoginPage, &LoginPage::signedIn, this, &UserManager::signedIn);
        mLoginPage->setServiceManager(mServiceManager);
        mLoginPage->show();
    }
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


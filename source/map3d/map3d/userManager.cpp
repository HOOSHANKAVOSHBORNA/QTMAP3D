#include "userManager.h"
//#include "qcryptographichash.h"

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
//    QJsonDocument jsonDoc;
//    QJsonObject jsonObject;
//    jsonObject.insert("Type", "SignIn");
//    QJsonObject jsonDataObject;
//    QByteArray pswNsalt (password.toStdString().c_str()) ;
//    pswNsalt = QCryptographicHash::hash(pswNsalt, QCryptographicHash::Md5).toHex();
//    jsonDataObject.insert("Username", username);
//    jsonDataObject.insert("Password", pswNsalt.data());

//    jsonObject.insert("Data", jsonDataObject);
//    jsonDoc.setObject(jsonObject);
//    mServiceManager->sendAction(jsonDoc.toJson(QJsonDocument::Indented));
    //    connect(mServiceManager, &ServiceManager::signInResponseReceived, this, &Authenticator::signedIn);
    hide();
    emit signedIn();
}

void LoginPage::closeEvent(QCloseEvent *)
{
    QCoreApplication::exit(-1);
}

//void LoginPage::signUp(const QString username, const QString password)
//{
//    QJsonDocument jsonDoc;
//    QJsonObject jsonObject;
//    jsonObject.insert("Type", "SignUp");
//    QJsonObject jsonDataObject;
//    QByteArray pswNsalt (password.toStdString().c_str()) ;
//    pswNsalt = QCryptographicHash::hash(pswNsalt, QCryptographicHash::Md5).toHex();
//    jsonDataObject.insert("Username", username);
//    jsonDataObject.insert("Password", pswNsalt.data());

//    jsonObject.insert("Data", jsonDataObject);
//    jsonDoc.setObject(jsonObject);
////    mServiceManager->sendAction(jsonDoc.toJson(QJsonDocument::Indented));
////    connect(mServiceManager, &ServiceManager::signUpResponseReceived, this, &Authenticator::signedUp);
//}

void LoginPage::setServiceManager(ServiceManager *newServiceManager)
{
    mServiceManager = newServiceManager;
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

QString Profile::getPassword() const
{
    return mPassword;
}

void Profile::setPassword(const QString &newPassword)
{
    if (mPassword == newPassword)
        return;
    mPassword = newPassword;
    emit passwordChanged();
}

ProfileManager *ProfileManager::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(mInstance == nullptr){ mInstance = new ProfileManager(); }
    return mInstance;
}

ProfileManager::~ProfileManager()
{
    delete mProfile;
}

Profile *ProfileManager::getProfile()
{
    return mProfile;
}

ProfileManager::ProfileManager(QObject *parent)
{
    mProfile = new Profile();
}

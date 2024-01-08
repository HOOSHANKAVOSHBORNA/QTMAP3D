#include "authenticator.h"
#include "qcryptographichash.h"

#include <QQmlComponent>
#include <QQuickWindow>
#include <QJsonObject>

Authenticator::Authenticator(QWindow *parent):
    QQuickWindow(parent)
{

}

void Authenticator::signIn(const QString username, const QString password)
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObject;
    jsonObject.insert("Type", "SignIn");
    QJsonObject jsonDataObject;
    QByteArray pswNsalt (password.toStdString().c_str()) ;
    pswNsalt = QCryptographicHash::hash(pswNsalt, QCryptographicHash::Md5).toHex();
    jsonDataObject.insert("Username", username);
    jsonDataObject.insert("Password", pswNsalt.data());

    jsonObject.insert("Data", jsonDataObject);
    jsonDoc.setObject(jsonObject);
//    mServiceManager->sendAction(jsonDoc.toJson(QJsonDocument::Indented));
//    connect(mServiceManager, &ServiceManager::signInResponseReceived, this, &Authenticator::signedIn);
}

void Authenticator::signUp(const QString username, const QString password)
{
    QJsonDocument jsonDoc;
    QJsonObject jsonObject;
    jsonObject.insert("Type", "SignUp");
    QJsonObject jsonDataObject;
    QByteArray pswNsalt (password.toStdString().c_str()) ;
    pswNsalt = QCryptographicHash::hash(pswNsalt, QCryptographicHash::Md5).toHex();
    jsonDataObject.insert("Username", username);
    jsonDataObject.insert("Password", pswNsalt.data());

    jsonObject.insert("Data", jsonDataObject);
    jsonDoc.setObject(jsonObject);
//    mServiceManager->sendAction(jsonDoc.toJson(QJsonDocument::Indented));
//    connect(mServiceManager, &ServiceManager::signUpResponseReceived, this, &Authenticator::signedUp);
}

void Authenticator::setServiceManager(ServiceManager *newServiceManager)
{
    mServiceManager = newServiceManager;
}

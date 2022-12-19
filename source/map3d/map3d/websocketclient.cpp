#include "websocketclient.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QSslKey>
#include <QWebSocket>

WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent)
{
    mWebSocket = new QWebSocket("Map3d Web Socket Client");
    mWebSocket->setParent(this);
    connect(mWebSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(mWebSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &WebSocketClient::onSslErrors);
    connect(mWebSocket, &QWebSocket::disconnected, [=](){
        qDebug() <<tr( "%1 can not connected to %2").arg(mWebSocket->origin()).arg(mWebSocket->requestUrl().toString());
    });

    QSslConfiguration sslConfiguration;
//    QFile certFile(QStringLiteral(":/Resources/websocketey/localhost.cert"));
    QFile keyFile(QStringLiteral(":/Resources/websocketkey/localhost.key"));
//    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
//    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
//    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
//    sslConfiguration.setLocalCertificate(certificate);
   sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
    mWebSocket->setSslConfiguration(sslConfiguration);

//    mWebSocket->open(QUrl(QStringLiteral("wss://localhost:1234")));
    mWebSocket->open(QUrl(QStringLiteral("wss://192.168.10.115:1234")));
}

void WebSocketClient::sendMessage(const QJsonDocument &message)
{
    mWebSocket->sendTextMessage(QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
}

//! [onConnected]
void WebSocketClient::onConnected()
{
    qDebug() <<tr( "%1 connected to %2").arg(mWebSocket->origin()).arg(mWebSocket->requestUrl().toString());
    connect(mWebSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::onTextMessageReceived);
}
//! [onConnected]

//! [onTextMessageReceived]
void WebSocketClient::onTextMessageReceived(QString message)
{
    //qDebug()<< " Message received:" << message;
    QJsonDocument messageDocument = QJsonDocument::fromJson(message.toUtf8());
    if(messageDocument.isObject())
        emit messageReceived(messageDocument);
    else
        qDebug() << "received messages is not a JSON object";

}

void WebSocketClient::onSslErrors(const QList<QSslError> &errors)
{
    //Q_UNUSED(errors)
    qDebug()<<tr("%1 errors:").arg(mWebSocket->origin());
    for(auto error: errors)
    {
        qDebug()<<error.errorString();
    }
    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.
    //    qDebug() << "mWebSocket error:" << mWebSocket->errorString();
    //mWebSocket->ignoreSslErrors();
}
//! [onTextMessageReceived]

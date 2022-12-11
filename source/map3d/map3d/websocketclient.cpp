#include "websocketclient.h"

#include <QDebug>
#include <QJsonDocument>
#include <QWebSocket>

WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent)
{
    mWebSocket = new QWebSocket();
    mWebSocket->setParent(this);
    connect(mWebSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(mWebSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &WebSocketClient::onSslErrors);

    mWebSocket->open(QUrl(QStringLiteral("wss://localhost:1234")));
}

void WebSocketClient::sendMessage(const QJsonDocument &message)
{
    mWebSocket->sendTextMessage(QString::fromUtf8(message.toJson(QJsonDocument::Compact)));
}

//! [onConnected]
void WebSocketClient::onConnected()
{
    qDebug() << "WebSocket connected: ";
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
        messageReceived(messageDocument);
    else
        qDebug() << "received is not a JSON object";

}

void WebSocketClient::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors)

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.
    //    qDebug() << "mWebSocket error:" << mWebSocket->errorString();
    mWebSocket->ignoreSslErrors();
}
//! [onTextMessageReceived]

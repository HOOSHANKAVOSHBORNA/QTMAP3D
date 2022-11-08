#include "websocketclient.h"

#include <QDebug>
#include <QWebSocket>

WebSocketClient::WebSocketClient(QString name, QObject *parent) : QObject(parent)
{
    mName = name;
    mWebSocket = new QWebSocket(mName);
    mWebSocket->setParent(this);
    connect(mWebSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(mWebSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &WebSocketClient::onSslErrors);

    mWebSocket->open(QUrl(QStringLiteral("wss://localhost:1234")));
}

void WebSocketClient::sendTextMessage(const QString &message)
{
    mWebSocket->sendTextMessage(message);
}

//! [onConnected]
void WebSocketClient::onConnected()
{
    qDebug() << "WebSocket connected: "<< mName;
    connect(mWebSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::onTextMessageReceived);
}
//! [onConnected]

//! [onTextMessageReceived]
void WebSocketClient::onTextMessageReceived(QString message)
{
    qDebug() << mName << " Message received:" << message;
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

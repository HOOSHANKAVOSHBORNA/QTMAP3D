#include "networkManager.h"
#include "qamqpexchange.h"
#include "qamqpqueue.h"

NetworkManager::NetworkManager(QObject *parent):
    QObject(parent)
{
    connect(&mClient, &QAmqpClient::connected, this, &NetworkManager::clientConnected);
    connect(&mClient, &QAmqpClient::disconnected, this, &NetworkManager::clientDisconnected);
    connect(&mClient, qOverload<QAMQP::Error >(&QAmqpClient::error), this, &NetworkManager::clientError);
    connect(&mClient, qOverload<QAbstractSocket::SocketError >(&QAmqpClient::socketErrorOccurred), this, &NetworkManager::clientSocketError);
    connect(&mClient, qOverload<const QList<QSslError>& >(&QAmqpClient::sslErrors), this, &NetworkManager::clientSslErrors);

    mClient.setAutoReconnect(true);
}

//void NetworkManager::start()
//{
//    mClient.connectToHost();
//}

void NetworkManager::setConfig(QString ip, int port, QString username, QString pass)
{
//    if(ip != mClient.host()
//        || port != mClient.port()
//        || username != mClient.username()
//        || pass != mClient.password()){

        mClient.disconnectFromHost();

        mClient.setHost(ip);
        mClient.setPort(port);
        mClient.setUsername(username);
        mClient.setPassword(pass);
        mClient.setVirtualHost("/");

        mClient.connectToHost();

//    }
}

void NetworkManager::sendMessage(const QString &message)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(message, "Map3dClient");
    //    qDebug() << "Sent message: "<<message;
}

bool NetworkManager::isConnected()
{
    return mClient.isConnected();
}

bool NetworkManager::isConsuming()
{
    qDebug()<<"isConsuming: "<<mMap3dQueue->isConsuming();
    qDebug()<<"Consumer count: "<<mMap3dQueue->consumerCount();
    qDebug()<<"messageCount count: "<<mMap3dQueue->messageCount();
    if(mMap3dQueue)
        return mMap3dQueue->isConsuming();
    return false;
}

void NetworkManager::clientConnected()
{
    emit connected(true);
    qDebug() << "Client connected to: "<<mClient.host()<<": "<<mClient.port();
    mMap3dQueue = mClient.createQueue("Map3d");
    QAmqpQueue *map3dClientQueue = mClient.createQueue("Map3dClient");
    disconnect(mMap3dQueue, 0, 0, 0); // in case this is a reconnect
    disconnect(map3dClientQueue, 0, 0, 0);

    //--data -------------------------------------------
    connect(mMap3dQueue, &QAmqpQueue::declared, this, &NetworkManager::onMap3dQueueDeclare);
    connect(map3dClientQueue, &QAmqpQueue::declared, this, &NetworkManager::onMap3dClientQueueDeclare);
    mMap3dQueue->declare();
    map3dClientQueue->declare();
}

void NetworkManager::clientDisconnected()
{
    qDebug() << "Client diconnected from: "<<mClient.host()<<": "<<mClient.port();
    emit connected(false);
}

void NetworkManager::clientError(QAMQP::Error error)
{
    emit connected(mClient.isConnected());
    qDebug()<<"error: "<<error<<": "<< mClient.errorString();
}

void NetworkManager::clientSocketError(QAbstractSocket::SocketError error)
{
    emit connected(mClient.isConnected());
    qDebug()<<"SocketError: "<<error<<": "<< mClient.errorString();
}

void NetworkManager::clientSslErrors(const QList<QSslError> &errors)
{
    emit connected(mClient.isConnected());
    for(const auto &error:errors)
        qDebug()<<"SslError: "<<error.error()<<": "<< error.errorString();
}

void NetworkManager::onMap3dQueueDeclare()
{
    qDebug()<<"Map3d queue declared.";
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;
    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::onMessageReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << "Waiting for messages from Map3d queue.";
}

void NetworkManager::onMap3dClientQueueDeclare()
{
    qDebug() << "Map3dClient queue declared.";
    emit map3dClientQueueDeclared();
}

void NetworkManager::onMessageReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
    QString messageString = message.payload();

    emit messageReceived(messageString);
}

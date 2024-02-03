#include "networkManager.h"
#include "qamqpexchange.h"
#include "qamqpqueue.h"

NetworkManager::NetworkManager(QObject *parent): QObject(parent)
{
    mClient.setAutoReconnect(true);
}

void NetworkManager::start()
{
    connect(&mClient, &QAmqpClient::connected, this, &NetworkManager::clientConnected);
    connect(&mClient, qOverload<QAMQP::Error >(&QAmqpClient::error), this, &NetworkManager::clientError);
    //    mClient.setHost("192.168.0.110");
    //    mClient.setPort(5672);
    //    mClient.setUsername("guest");
    //    mClient.setPassword("guest");
    //    mClient.setVirtualHost("/");
    mClient.connectToHost();
}

void NetworkManager::sendMessage(const QString &message)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(message, "Map3dClient");
//    qDebug() << "Sent message: "<<message;
}

void NetworkManager::clientConnected()
{
    qDebug() << "Client connected.";
    QAmqpQueue *map3dQueue = mClient.createQueue("Map3d");
    QAmqpQueue *map3dClientQueue = mClient.createQueue("Map3dClient");
    disconnect(map3dQueue, 0, 0, 0); // in case this is a reconnect
    disconnect(map3dClientQueue, 0, 0, 0);

    //--data -------------------------------------------
    connect(map3dQueue, &QAmqpQueue::declared, this, &NetworkManager::onMap3dQueueDeclare);
    connect(map3dClientQueue, &QAmqpQueue::declared, this, &NetworkManager::onMap3dClientQueueDeclare);
    map3dQueue->declare();
    map3dClientQueue->declare();
}

void NetworkManager::clientError(QAMQP::Error error)
{
    qDebug()<<"error: "<<error<<": "<< mClient.errorString();
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

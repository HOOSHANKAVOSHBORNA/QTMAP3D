#include "networkManager.h"
#include "qamqpexchange.h"
#include "qamqpqueue.h"

NetworkManager::NetworkManager(ServiceManager *serviceManger, QObject *parent):
    QObject(parent), mServiceManager(serviceManger)
{
    mClient.setAutoReconnect(true);
}

void NetworkManager::start()
{
    connect(&mClient, &QAmqpClient::connected, this, &NetworkManager::clientConnected);
    mClient.connectToHost();
}

void NetworkManager::dataQueueDeclared()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::dataMessageReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << "Waiting for messages.";
}

void NetworkManager::dataMessageReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
   qDebug() << "message: " << message.payload();
    mServiceManager->messageData(message.payload());
}

void NetworkManager::clientConnected()
{
    qDebug() << "Client connected.";
    QAmqpQueue *dataQueue = mClient.createQueue("data");
    QAmqpQueue *actionQueue = mClient.createQueue("action");
    disconnect(dataQueue, 0, 0, 0); // in case this is a reconnect
    disconnect(actionQueue, 0, 0, 0);
    connect(dataQueue, &QAmqpQueue::declared, this, &NetworkManager::dataQueueDeclared);
    connect(actionQueue, &QAmqpQueue::declared, this, &NetworkManager::sendDataQueueDeclared);
    dataQueue->declare();
    actionQueue->declared();
}

void NetworkManager::sendDataQueueDeclared()
{
    connect(mServiceManager, &ServiceManager::actionSent, this, &NetworkManager::sendData);
}

void NetworkManager::sendData(const QString &action)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(action, "action");
    qDebug() << "Sent action: "<<action;
}

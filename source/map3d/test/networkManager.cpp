#include "networkManager.h"
#include "qamqpexchange.h"
#include "qamqpqueue.h"

NetworkManager::NetworkManager(QObject *parent): QObject(parent)
{
    mClient.setAutoReconnect(true);
}

void NetworkManager::sendData(const QString &data)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(data, "data");
    // qDebug() << "Sent data: "<<data;
}

void NetworkManager::start()
{
    connect(&mClient, &QAmqpClient::connected, this, &NetworkManager::clientConnected);
    connect(&mClient, qOverload<QAMQP::Error >(&QAmqpClient::error), this, &NetworkManager::clientError);
    mClient.connectToHost();
}

void NetworkManager::clientConnected()
{
    qDebug() << "Client connected.";
    QAmqpQueue *dataQueue = mClient.createQueue("data");
    QAmqpQueue *actionQueue = mClient.createQueue("action");
    disconnect(dataQueue, 0, 0, 0); // in case this is a reconnect
    disconnect(actionQueue, 0, 0, 0);

    //--data -------------------------------------------
    connect(dataQueue, &QAmqpQueue::declared, this, &NetworkManager::dataQueueDeclared);
    connect(actionQueue, &QAmqpQueue::declared, this, &NetworkManager::actionQueueDeclared);
    dataQueue->declare();
    actionQueue->declare();
}

void NetworkManager::clientError(QAMQP::Error error)
{
    qDebug()<<"error: "<<error<<": "<< mClient.errorString();
}

void NetworkManager::actionQueueReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
    qDebug() << "message: " << message.payload();
}

void NetworkManager::actionQueueDeclared()
{
    qDebug() << "queue dec";
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;
    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::actionQueueReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << "Waiting for messages.";

}

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
    mClient.connectToHost();
}

void NetworkManager::clientConnected()
{
    qDebug() << "Client connected.";
    //--flyable queue-------------------------------------------
    QAmqpQueue *flyableQueue = mClient.createQueue("flyable");
    disconnect(flyableQueue, 0, 0, 0); // in case this is a reconnect
    connect(flyableQueue, &QAmqpQueue::declared, this, &NetworkManager::flyableQueueDeclared);
    flyableQueue->declare();

    //--layer queue-------------------------------------------
    QAmqpQueue *layerQueue = mClient.createQueue("layer");
    disconnect(layerQueue, 0, 0, 0); // in case this is a reconnect
    connect(layerQueue, &QAmqpQueue::declared, this, &NetworkManager::layerQueueDeclared);
    layerQueue->declare();
}

void NetworkManager::flyableQueueDeclared()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::flyableMessageReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << "Waiting for flyable messages.";
}

void NetworkManager::flyableMessageReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
    qDebug() << "Flyable message: " << message.payload();
}

void NetworkManager::layerQueueDeclared()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::layerMessageReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << "Waiting for layer messages.";
}

void NetworkManager::layerMessageReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
    qDebug() << "Layer message: " << message.payload();
}

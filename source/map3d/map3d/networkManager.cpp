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

void NetworkManager::clientConnected()
{
    qDebug() << "Client connected.";
    //--layer queue-------------------------------------------
    QAmqpQueue *layerQueue = mClient.createQueue("layer");
    disconnect(layerQueue, 0, 0, 0); // in case this is a reconnect
    connect(layerQueue, &QAmqpQueue::declared, this, &NetworkManager::layerQueueDeclared);
    layerQueue->declare();
    //--flyable queue-------------------------------------------
    QAmqpQueue *flyableQueue = mClient.createQueue("flyable");
    disconnect(flyableQueue, 0, 0, 0); // in case this is a reconnect
    connect(flyableQueue, &QAmqpQueue::declared, this, &NetworkManager::flyableQueueDeclared);
    flyableQueue->declare();
    //--status queue--------------------------------------------
    QAmqpQueue *statusQueue = mClient.createQueue("status");
    disconnect(statusQueue, 0, 0, 0); // in case this is a reconnect
    connect(statusQueue, &QAmqpQueue::declared, this, &NetworkManager::statusQueueDeclared);
    statusQueue->declare();

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
    mServiceManager->flyableNodeData(message.payload().toStdString());
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

    mServiceManager->layersData(message.payload().toStdString());
}

void NetworkManager::statusQueueDeclared()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::statusMessageReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << "Waiting for status messages.";
}

void NetworkManager::statusMessageReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
    qDebug() << "status message: " << message.payload();

    mServiceManager->statusNodeData(message.payload().toStdString());
}

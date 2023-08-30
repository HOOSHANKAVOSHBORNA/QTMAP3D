#include "networkManager.h"

NetworkManager::NetworkManager(QObject *parent): QObject(parent)
{
    mClient.setAutoReconnect(true);
}

void NetworkManager::start()
{
    connect(&mClient, &QAmqpClient::connected, this, &NetworkManager::clientConnected);
    mClient.connectToHost();
}

void NetworkManager::sendData()
{
    QAmqpExchange *defaultExchange = mClient.createExchange();
    defaultExchange->publish("Hello World!", "hello");
    qDebug() << " [x] Sent 'Hello World!'";
}

void NetworkManager::clientConnected()
{
    QAmqpQueue *queue = mClient.createQueue("flyable");
    disconnect(queue, 0, 0, 0); // in case this is a reconnect
    connect(queue, &QAmqpQueue::declared, this, &NetworkManager::flyableQueueDeclared);
    queue->declare();
}

void NetworkManager::flyableQueueDeclared()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::flyableMessageReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << "Waiting for flyable messages.";

    emit ready();
}

void NetworkManager::flyableMessageReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
    qDebug() << "Flyable message: " << message.payload();
}

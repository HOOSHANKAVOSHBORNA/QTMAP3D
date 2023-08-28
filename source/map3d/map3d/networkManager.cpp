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
    QAmqpQueue *queue = mClient.createQueue("hello");
    disconnect(queue, 0, 0, 0); // in case this is a reconnect
    connect(queue, &QAmqpQueue::declared, this, &NetworkManager::queueDeclared);
    queue->declare();
}

void NetworkManager::queueDeclared()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::messageReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << " [*] Waiting for messages. To exit press CTRL+C";

    emit ready();
}

void NetworkManager::messageReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
    qDebug() << " [x] Received " << message.payload();
}

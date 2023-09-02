#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

#include "qamqpclient.h"
#include "qamqpexchange.h"
#include "qamqpqueue.h"

class NetworkManager: public QObject
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = nullptr);
    void sendFlyableData(const QString &data);

    void start();
private slots:
    void clientConnected();
    void clientError(QAMQP::Error error);
signals:
    void flyableQueueDeclared();
private:
    QAmqpClient mClient;
};

#endif // NETWORKMANAGER_H

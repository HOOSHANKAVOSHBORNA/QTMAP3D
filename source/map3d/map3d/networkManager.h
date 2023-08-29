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
    void start();

    void sendData();
private slots:
    void clientConnected();
    void queueDeclared();
    void messageReceived();
signals:
    void ready();
private:
    QAmqpClient mClient;
};

#endif // NETWORKMANAGER_H

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

#include "qamqpclient.h"

class NetworkManager: public QObject
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = nullptr);
    void sendData(const QString &data);

    void start();
private slots:
    void clientConnected();
    void clientError(QAMQP::Error error);
    void actionQueueReceived();
    void actionQueueDeclared();
signals:
    void dataQueueDeclared();
private:
    QAmqpClient mClient;
    QString mUsername{"Admin"};
    QString mPassword{"e3afed0047b08059d0fada10f400c1e5"};
};

#endif // NETWORKMANAGER_H

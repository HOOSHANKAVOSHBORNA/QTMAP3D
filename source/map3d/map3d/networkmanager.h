#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

class WebSocketClient;

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

public:
    inline WebSocketClient *webSocketClient() const { return mWebSocketClient;}

signals:

public slots:

private:
    WebSocketClient *mWebSocketClient = nullptr;
};

#endif // NETWORKMANAGER_H

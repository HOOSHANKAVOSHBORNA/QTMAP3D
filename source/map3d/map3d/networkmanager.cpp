
#include "networkmanager.h"
#include "websocketclient.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    mWebSocketClient = new WebSocketClient;
}

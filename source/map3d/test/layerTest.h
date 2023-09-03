#ifndef LAYERTEST_H
#define LAYERTEST_H

#include "networkManager.h"

#include <QJsonDocument>

class LayerTest
{
public:
    LayerTest(NetworkManager *networkManager);

private:
    QJsonDocument createLayers();
private:
    NetworkManager *mNetworkManager;
};

#endif // LAYERTEST_H

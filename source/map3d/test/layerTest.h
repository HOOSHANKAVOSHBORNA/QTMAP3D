#ifndef LAYERTEST_H
#define LAYERTEST_H

#include "serviceManager.h"

class LayerTest
{
public:
    LayerTest(ServiceManager *serviceManager);

private:
    void createLayers();
    QJsonDocument updateLayers();

private:
    ServiceManager *mServiceManager;
    std::vector<LayerData> LayerDataList;
};

#endif // LAYERTEST_H

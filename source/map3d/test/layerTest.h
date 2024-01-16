#ifndef LAYERTEST_H
#define LAYERTEST_H

#include "serviceManager.h"

class LayerTest
{
public:
    LayerTest(ServiceManager *serviceManager);

private:
    void createLayers();
    void updateLayers();

private:
    ServiceManager *mServiceManager;
    std::vector<LayerData> LayerDataList;
};

#endif // LAYERTEST_H

#ifndef FEATURELAYER_H
#define FEATURELAYER_H

#include "plugininterface.h"

class FeatureLayer : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "featurelayer.json")
    Q_INTERFACES(PluginInterface)
public:
    FeatureLayer(QWidget *parent = nullptr);
    ~FeatureLayer()override{}
    void setUpUI() override;

private:
    void GDAL();
    void WFS();
    void ArcGis();
};

#endif // FEATURELAYER_H

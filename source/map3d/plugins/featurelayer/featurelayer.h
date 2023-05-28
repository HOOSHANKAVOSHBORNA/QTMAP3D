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
    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    void onToolboxItemClicked(const QString &name, const QString &category) override;
    bool setup(MapItem *mapItem,
               UIHandle *UIHandle) override;
private:
    void addGDAL();
    void addWFS();
    void addArcGIS();
private:
    MapItem* mMapItem{nullptr};
};

#endif // FEATURELAYER_H

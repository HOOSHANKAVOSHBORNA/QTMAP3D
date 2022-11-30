
#ifndef CPTESTPLUGIN_H
#define CPTESTPLUGIN_H

#include "plugininterface.h"

class Test1Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "test1plugin.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit Test1Plugin();

public:
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc) override;
    virtual void onSideItemCreated(int index, QObject *pSideItem) override;
    virtual void onToolboxItemClicked(const QString& name, const QString& category) override;

    virtual bool initialize3D(MapController *pMapController) override;
};

#endif // CPTESTPLUGIN_H

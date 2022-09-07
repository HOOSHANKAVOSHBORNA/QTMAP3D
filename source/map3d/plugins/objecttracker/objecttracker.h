#ifndef OBJECTTRACKER_H
#define OBJECTTRACKER_H

#include "plugininterface.h"

class ObjectTracker : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "objecttracker.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit ObjectTracker(QWidget *parent = nullptr);
    void setUpUI() override;

private:
    void trackObject();
};

#endif // OBJECTTRACKER_H

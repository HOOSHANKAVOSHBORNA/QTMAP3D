#ifndef EARTHFILE_H
#define EARTHFILE_H

#include "plugininterface.h"

class EarthFile : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "earthfile.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit EarthFile(QWidget *parent = nullptr);
    void setUpUI() override;
private:
};

#endif // EARTHFILE_H

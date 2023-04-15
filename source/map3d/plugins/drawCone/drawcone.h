#ifndef DRAWCONE_H
#define DRAWCONE_H

#include "plugininterface.h"

#define DRAW_LAYER_NAME "Cone"

class DrawCone : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCone.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawCone(QObject *parent = nullptr);
};

#endif // DRAWCONE_H

#include "map3dwidget.h"
#include "plugininterface.h"

#include <QDockWidget>

PluginInterface::PluginInterface(QWidget *parent)
    : QWidget(parent)
{

}

void PluginInterface::setMap3dWidget(Map3dWidget *value)
{
    map3dWidget = value;
}

void PluginInterface::setDocWidget(QDockWidget *value)
{
    docWidget = value;
}

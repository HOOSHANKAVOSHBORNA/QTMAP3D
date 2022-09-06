#include "map3dwidget.h"
#include "plugininterface.h"

#include <QVBoxLayout>

PluginInterface::PluginInterface(QWidget *parent)
    : QWidget(parent)
{

}

void PluginInterface::setMap3dWidget(Map3dWidget *value)
{
    mMap3dWidget = value;
}

void PluginInterface::setToolBar(QToolBar *toolBar)
{
    mToolBar = toolBar;
}

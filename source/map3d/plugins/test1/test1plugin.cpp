
#include <iostream>
#include "test1plugin.h"

Test1Plugin::Test1Plugin()
{
    Q_INIT_RESOURCE(test1plugin);
}

bool Test1Plugin::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc)
{
    pDesc->pluginHasSideItem = true;
    pDesc->sideItemMenuBarTitle = "Layers";
    pDesc->sideItemMenuBarIconUrl = "qrc:///test1plugin/resources/Layers.png";
    pDesc->sideItemUrl = "qrc:///test1plugin/Layers.qml";

    pDesc->toolboxItemsList.push_back(new ToolboxItemDesc{"One", "Two", "", true, false, ""});

    return true;
}

void Test1Plugin::onSideItemCreated(int index, QObject *pSideItem)
{

}

void Test1Plugin::onToolboxItemClicked(const QString& name, const QString& category)
{
    std::cout << "Hello from plugin: " << name.toStdString() << " , " << category.toStdString() << std::endl;
}

bool Test1Plugin::initialize3D(MapController *pMapController)
{

    return true;
}

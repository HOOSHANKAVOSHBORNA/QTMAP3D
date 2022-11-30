
#include <iostream>
#include "test1plugin.h"

Test1Plugin::Test1Plugin()
{
    Q_INIT_RESOURCE(Test1Plugin);
}

bool Test1Plugin::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc)
{
    pDesc->pluginHasSideItem = true;
    pDesc->sideItemMenuBarTitle = "Layers";
    pDesc->sideItemMenuBarIconUrl = "qrc:///Test1Plugin/Resources/Layers.png";
    pDesc->sideItemUrl = "qrc:///Test1Plugin/Layers.qml";

    pDesc->toolboxItemsList.push_back(new ToolboxItemDesc{"One", "Two", "", true});

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

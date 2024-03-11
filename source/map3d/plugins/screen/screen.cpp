#include "screen.h"
#include "mapItem.h"
#include <QQmlContext>

Screen::Screen(QWidget *parent): PluginInterface(parent)
{

//    mEngine = new QQmlEngine;
}

bool Screen::setup()
{
    auto toolboxItemSnapShot =  new ToolboxItem{SNAPSHOT, CATEGORY, "qrc:/resources/screen.png", false};
    QObject::connect(toolboxItemSnapShot, &ToolboxItem::itemClicked, this, &Screen::onSnapShotClicked);
    toolbox()->addItem(toolboxItemSnapShot);
    //////////////////////////////////////////////////////////////////////////////////////////////
    auto toolboxItemSnipTool =  new ToolboxItem{SNIP, CATEGORY, "qrc:/resources/snip.png", false};
    QObject::connect(toolboxItemSnipTool, &ToolboxItem::itemClicked, this, &Screen::onSnipToolClicked);
    toolbox()->addItem(toolboxItemSnipTool);
    //////////////////////////////////////////////////////////////////////////////////////////////
    mSnapShot = new SnapShot(mapItem(), this);
    return true;
}


void Screen::onSnapShotClicked()
{
    mSnapShot->capture();
}


void Screen::onSnipToolClicked()
{
    mSnipTool = new SnipToolInterface(mapItem(),qmlEngine());
}




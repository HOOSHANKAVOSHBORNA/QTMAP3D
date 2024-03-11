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
    return true;
}


void Screen::onSnapShotClicked()
{
//    auto point = mapItem()->mapToScene(QPoint(0,0));
//    auto mapHeight =  mapItem()->boundingRect().height();
//    auto mapWidth =  mapItem()->boundingRect().width();
//    CaptureCallback *captureCallback = new CaptureCallback("/home/client110/Pictures/capTest.png",point,mapWidth, mapHeight);
//    mapItem()->addCaptureCallback(captureCallback);
//    captureCallback->capture();

    mSnapShot = new SnapShot(mapItem());
}


void Screen::onSnipToolClicked()
{
    mSnipTool = new SnipToolInterface(mapItem(),qmlEngine());
}




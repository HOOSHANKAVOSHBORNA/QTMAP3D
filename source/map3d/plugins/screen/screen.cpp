#include "screen.h"
#include "mapItem.h"

#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>


const QString CATEGORY = "Screen";
const QString SNAPSHOT = "Snap Shot";






Screen::Screen(QWidget *parent):PluginInterface(parent)
{
}


bool Screen::setup()
{
    auto toolboxItemSnapShot =  new ToolboxItem{SNAPSHOT, CATEGORY, "qrc:/resources/screen.png", false};
    QObject::connect(toolboxItemSnapShot, &ToolboxItem::itemClicked, this, &Screen::takeSnapShot);
    toolbox()->addItem(toolboxItemSnapShot);
    mCamera =  mapItem()->getViewer()->getCamera();
    return true;
}

bool Screen::frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    if(mViewCaptureCallback && mViewCaptureCallback->screenTaken)
        mCamera->setFinalDrawCallback(NULL);
    return false;
}

void Screen::takeSnapShot()
{
    osg::ref_ptr<osg::Image> img = new osg::Image();
    mViewCaptureCallback = new ViewCaptureCallback(img);
    mViewCaptureCallback->screenTaken = false;
    mCamera->setFinalDrawCallback(mViewCaptureCallback);
}

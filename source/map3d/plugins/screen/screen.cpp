#include "screen.h"
#include "mapItem.h"

#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QQuickView>
#include <QTimer>
#include <QStandardPaths>


const QString CATEGORY = "Screen";
const QString SNAPSHOT = "Snap Shot";




Screen::Screen(QWidget *parent): PluginInterface(parent)
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

//bool Screen::frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//{

//    return false;
//}

void Screen::takeSnapShot()
{
    mReadyStatus = false;
    std::string directory = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last().toStdString() + "/QArch SnapShot/" + QDateTime::currentDateTime().date().toString().toStdString();
    std::string filename = "SnapShot ";
    QString timeNow = QDateTime::currentDateTime().time().toString();
    osgDB::makeDirectory(directory);
    mFileDialog = osgDB::getRealPath(directory + "/" + filename + timeNow.toStdString() + ".png");
    auto point = mapItem()->mapToScene(QPoint(0,0));
    osg::ref_ptr<osg::Image> img = new osg::Image();
    mViewCaptureCallback = new ViewCaptureCallback(img, point ,mFileDialog);
    connect(mViewCaptureCallback,&ViewCaptureCallback::imageProcessComplete,this,&Screen::onImageProcessComplete);
    mCamera->setFinalDrawCallback(mViewCaptureCallback);

    QQmlComponent component(&mEngine, QUrl("qrc:/resources/SnapShot.qml"));
    mObject = component.create();
    auto window = dynamic_cast<QQuickWindow*>(mObject);
    if(window)
        window->showFullScreen();

    QTimer *timer = new QTimer(this);
    timer->singleShot(50, this, SLOT(takingProcessFinished()));
    timer->deleteLater();
}

void Screen::onImageProcessComplete(osg::Image *iImage)
{
    mCamera->setFinalDrawCallback(NULL);
    mViewCaptureCallback->screenTaken = false;
}

void Screen::takingProcessFinished()
{
    mObject->deleteLater();
}



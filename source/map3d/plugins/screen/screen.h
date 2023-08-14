#ifndef SCREEN_H
#define SCREEN_H

#include "plugininterface.h"
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <thread>
#include <QFile>
#include <QDateTime>

class ViewCaptureCallback : public osg::Camera::DrawCallback
{
public:
    ViewCaptureCallback(osg::Image* image, QPointF sceneStartPoint) :
        mImage(image),
        mSceneStartPoint(sceneStartPoint)
    {}

    virtual void operator () (osg::RenderInfo& renderInfo) const
    {
        std::string directory = "../SnapShots";
        std::string filename = "SnapShot ";
        QString timeNow = QDateTime::currentDateTime().toString();
        osgDB::makeDirectory(directory);
        std::string fullpath = osgDB::getRealPath(directory + "/" + filename + timeNow.toStdString() + ".png");
        auto viewPort = renderInfo.getCurrentCamera()->getViewport();
        mImage->readPixels(mSceneStartPoint.x(), mSceneStartPoint.y(), viewPort->width(), viewPort->height(), GL_RGB, GL_UNSIGNED_BYTE);
        bool resultSnap = osgDB::writeImageFile(*mImage, fullpath);
        screenTaken = resultSnap;
    }
    mutable bool screenTaken = false;
protected:
    osg::ref_ptr<osg::Image> mImage;
    QPointF mSceneStartPoint;

};

class Screen: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "screen.json")
    Q_INTERFACES(PluginInterface)
public:
    Screen(QWidget *parent = nullptr);
    ~Screen()override{}
    bool setup() override;
    bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private slots:
    void takeSnapShot();

private:
    osg::Camera* mCamera;
    ViewCaptureCallback* mViewCaptureCallback{nullptr};
};



#endif // SCREEN_H

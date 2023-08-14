#ifndef SCREEN_H
#define SCREEN_H

#include "plugininterface.h"
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <QFileDialog>
#include <QQmlComponent>
#include <QQuickItem>
#include <QObject>

class ViewCaptureCallback :public QObject, public osg::Camera::DrawCallback

{
    Q_OBJECT
public:
    ViewCaptureCallback(osg::Image* image, QPointF sceneStartPoint , std::string filePath) :
        mImage(image),
        mSceneStartPoint(sceneStartPoint),
        mFilePath(filePath)
    {}



    virtual void operator () (osg::RenderInfo& renderInfo) const
    {
        auto viewPort = renderInfo.getCurrentCamera()->getViewport();
        mImage->readPixels(mSceneStartPoint.x(), mSceneStartPoint.y(), viewPort->width(), viewPort->height(), GL_RGB, GL_UNSIGNED_BYTE);
        bool resultSnap = osgDB::writeImageFile(*mImage, mFilePath);
        screenTaken = resultSnap;
        if(resultSnap)
        emit imageProcessComplete(mImage);
    }
    mutable bool screenTaken = false;

signals:
    void imageProcessComplete(osg::Image *osgImage) const;

protected:
    osg::ref_ptr<osg::Image> mImage;
    QString mDirectorryPath;
    QPointF mSceneStartPoint;
    std::string mFilePath;

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
//    bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;


signals:
    void fileUpdated();

private slots:
    void takeSnapShot();
    void onImageProcessComplete(osg::Image *iImage);
    void takingProcessFinished();

private:
    osg::Camera* mCamera;
    ViewCaptureCallback* mViewCaptureCallback{nullptr};
    std::string mFileDialog;
    QQmlEngine mEngine;
    QObject *mObject;
    bool mReadyStatus;
};



#endif // SCREEN_H

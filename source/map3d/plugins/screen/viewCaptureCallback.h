#ifndef VIEWCAPTURECALLBACK_H
#define VIEWCAPTURECALLBACK_H

#include <QObject>
#include <osg/Camera>
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
    ViewCaptureCallback(osg::Image* image, QPointF sceneStartPoint, double width , double height) :
        mImage(image),
        mSceneStartPoint(sceneStartPoint),
//        mFilePath(filePath),
        mWidth(width),
        mHeight(height)
    {}
    virtual void operator () (osg::RenderInfo& renderInfo) const
    {
        //        auto viewPort = renderInfo.getCurrentCamera()->getViewport();
        mImage->readPixels(mSceneStartPoint.x(), mSceneStartPoint.y(), mWidth , mHeight , GL_RGB, GL_UNSIGNED_BYTE);
//        bool resultSnap = osgDB::writeImageFile(*mImage, mFilePath);
        screenTaken = true;
        if(true)
            emit imageProcessComplete(mImage , mWidth ,mHeight);
    }
    mutable bool screenTaken = false;

signals:
    void imageProcessComplete(osg::Image *osgImage , int width , int height) const;
protected:
    osg::ref_ptr<osg::Image> mImage;
    QString mDirectorryPath;
    QPointF mSceneStartPoint;
//    std::string mFilePath;
    double mHeight;
    double mWidth;
};

#endif // VIEWCAPTURECALLBACK_H

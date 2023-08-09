#ifndef DRAWIMAGE_H
#define DRAWIMAGE_H
#include "imageProperties.h"
#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>
#include <QMouseEvent>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthSymbology/GeometryFactory>
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/PlaceNode>
#include <QQmlEngine>
#include <QQmlComponent>

#include "drawShape.h"

#define IMAGEOVERLAY "ImageOverlay"
//class ImageProperties;

class DrawImage : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawImage.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawImage(QObject *parent = nullptr);
    bool setup() override;
    void loadImage();
private slots:
    void onImageItemCheck(bool check);

private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;
    osg::ref_ptr<ParenticAnnotationLayer> mImgLayer;
    static int mCount;
    QQuickItem *mItem;
    ImageProperties *mImageProperties{nullptr};


private:
    osg::ref_ptr<osg::Image> mImage{nullptr};
    osg::ref_ptr<osg::Image> imageAddr{nullptr};
    osg::ref_ptr<osgEarth::Annotation::ImageOverlay> mImageOverlay{nullptr};
    osg::ref_ptr<osgEarth::Annotation::ImageOverlayEditor> mImgOvlEditor{nullptr};
};

#endif // DRAWIMAGE_H

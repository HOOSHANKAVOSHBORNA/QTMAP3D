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

#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/PlaceNode>
#include <QQmlEngine>
#include <QQmlComponent>

#include "mapcontroller.h"
#include "plugininterface.h"


#define DRAW_LAYER_NAME "image"
class ImageProperties;
class DrawImage : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawImage.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawImage(QObject *parent = nullptr);
    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    bool setup(MapController *mapController, UIHandle *uiHandle) override;
    void loadImage();
    void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
//    osg::Vec2d getCenter();
//    void setCenter(osg::Vec2d center);
//    void setImage(QUrl file);
//    void setHeight(double height);
//    void setWidth(double width);

private:
    void startDraw(QMouseEvent* event);
    void finishDrawing(QMouseEvent* event);
    void cancelDrawing(QMouseEvent* event);
    osgEarth::Annotation::PlaceNode* makeIconNode();
    ImageProperties *mImageProperties{nullptr};


private:
    osg::Image *mImage{nullptr};
    QQmlEngine *mQmlEngine;
    UIHandle *mUiHandle;
    MapController *mMapcontroller;
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    bool mEnterImageZone{false};
    osg::Image* imageAddr{nullptr};
    osgEarth::Annotation::ImageOverlay* mImageOverlay{nullptr};
    osgEarth::Annotation::ImageOverlayEditor* mImgOvlEditor{nullptr};



    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWIMAGE_H

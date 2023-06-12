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
//    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    bool setup() override;
    void loadImage();
//    void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private slots:
    void onImageItemCheck(bool check);

private:
    bool startDraw(const osgGA::GUIEventAdapter &ea);
    bool finishDrawing(const osgGA::GUIEventAdapter &ea);
    bool cancelDrawing(const osgGA::GUIEventAdapter &ea);
    osgEarth::Annotation::PlaceNode* makeIconNode();
    ImageProperties *mImageProperties{nullptr};


private:
    osg::Image *mImage{nullptr};
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    bool mEnterImageZone{false};
    osg::Image* imageAddr{nullptr};
    osgEarth::Annotation::ImageOverlay* mImageOverlay{nullptr};
    osgEarth::Annotation::ImageOverlayEditor* mImgOvlEditor{nullptr};



    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWIMAGE_H

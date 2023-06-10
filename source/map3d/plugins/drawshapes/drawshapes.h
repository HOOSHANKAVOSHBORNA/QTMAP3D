#ifndef GENERICPLUGIN_H
#define GENERICPLUGIN_H

#include <QMouseEvent>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthSymbology/GeometryFactory>
#include "plugininterface.h"

#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/PlaceNode>
#include <QQmlEngine>
#include <QQmlComponent>

#define DRAW_LAYER_NAME "DrawShapes"

class DrawShapes : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawshapes.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit DrawShapes(QWidget *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup() override;
protected:
    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseDoubleClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private:
    enum class Shape {NONE ,LINE, SPHERE, CONE, CYLINDER, CAPSULE, BOX, POLYGON, IMGOVLY,
               RECT, ELLIPSE};
    Shape mShape;
    enum class DrawingState {NONE, START, FINISH, DELETE, INITIAL};
    DrawingState mDrawingState;
    osgEarth::Annotation::ImageOverlay* mImageOverlay{nullptr};
    osgEarth::Annotation::ImageOverlayEditor* mImgOvlEditor{nullptr};

private slots:
    bool onImgOvlyBtnClick(const osgGA::GUIEventAdapter& event);


private:
    QQuickItem *mItem = nullptr;

    bool mEnterShapeZone{false};

};

#endif // GENERICPLUGIN_H

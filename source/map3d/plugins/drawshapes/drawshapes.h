#ifndef GENERICPLUGIN_H
#define GENERICPLUGIN_H

#include <QMouseEvent>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthSymbology/GeometryFactory>
#include "plugininterface.h"

#include "spherenodeeditor.h"

#include "osgEarthAnnotation/AnnotationEditing"
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
    bool setup(MapController *mapController,
               UIHandle *UIHandle) override;
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
private:
    enum class Shape {NONE ,LINE, SPHERE, CONE, CYLINDER, CAPSULE, BOX, POLYGON, IMGOVLY,
               RECT, ELLIPSE};
    Shape mShape;
    enum class DrawingState {NONE, START, FINISH, DELETE, INITIAL};
    DrawingState mDrawingState;
    osgEarth::Annotation::ImageOverlay* mImageOverlay{nullptr};
    osgEarth::Annotation::ImageOverlayEditor* mImgOvlEditor{nullptr};

private slots:
    void onImgOvlyBtnClick(QMouseEvent* event);


private:

    MapController* mMapController{nullptr};
    QQmlEngine *mQmlEngine = nullptr;
    QQuickItem *mItem = nullptr;

    bool mEnterShapeZone{false};

};

#endif // GENERICPLUGIN_H

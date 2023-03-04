#ifndef LINE_H
#define LINE_H

#include <QMouseEvent>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthSymbology/GeometryFactory>
#include "plugininterface.h"
#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <QQmlEngine>
#include <QQmlComponent>
#include <linenode.h>
#include <QPainter>
#include <lineproperties.h>

#define DRAW_LAYER_NAME "Line"

class drawLine: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawline.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit drawLine(QWidget *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup(MapController *mapController,
               UIHandle *UIHandle) override;
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    void startDrawLine();
    void drawingLine(QMouseEvent* event);
    void cancelDrawingLine(QMouseEvent* event);
    void mouseMoveDrawing(QMouseEvent* event);
    void finishDrawing(QMouseEvent* event, osg::Node *nodeEditor = nullptr);

    bool addNodeToLayer(osg::Node *node);
    void removeNodeFromLayer(osg::Node *node);
private:
    MapController* mMapController{nullptr};
    QQmlEngine *mQmlEngine = nullptr;
    enum class DrawingState {START, DRAWING, FINISH};
    DrawingState mDrawingState;
    enum class Shape {NONE, LINESTRING, LINE};
    Shape mShape;
    LineNode* mLine{nullptr};
    LineProperties *mLineProperties = nullptr;
};

#endif // LINE_H

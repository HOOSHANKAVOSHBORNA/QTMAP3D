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
#include <osgEarthAnnotation/PlaceNode>
#include <QQmlEngine>
#include <QQmlComponent>

#define DRAW_LAYER_NAME "Line"



class Line: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "line.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit Line(QWidget *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup(MapController *mapController,
               UIHandle *UIHandle) override;

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
//    virtual void mouseMoveEvent(QMouseEvent* event) override;
//    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    MapController* mMapController{nullptr};
    osgEarth::Annotation::AnnotationLayer* mAnnoLayer;
    QQmlEngine *mQmlEngine = nullptr;

    enum class DrawingState {NONE, START, FINISH, DELETE, INITIAL};
    DrawingState mDrawingState;
    enum class Shape {NONE, LINE, LINE_II};
    Shape mShape;

private slots:
    void onLineBtnClick(QMouseEvent* event);
    void onLineIIBtnClick(QMouseEvent* event);
};

#endif // LINE_H

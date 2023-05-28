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
#include <measureHeight.h>

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
    bool setup(MapItem *mapItem,
               UIHandle *UIHandle) override;

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    void startDrawLine();
    void startDrawMeasureHeight();
    void drawingMeasureHeight(QMouseEvent* event);
    void mouseMoveMeasureHeightDrawing(QMouseEvent* event);
    void drawingLine(QMouseEvent* event);
    void cancelDrawingLine(QMouseEvent* event);
    void mouseMoveDrawing(QMouseEvent* event);
    void finishDrawing(QMouseEvent* event, osg::Node *nodeEditor = nullptr);
    osgEarth::Annotation::PlaceNode *makeIconNode();


private:
    MapItem* mMapItem{nullptr};
    QQmlEngine *mQmlEngine = nullptr;
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    enum class Type{NONE, LINE, RULER, HEIGHT};
    Type mType;
    LineNode* mLine{nullptr};
    MeasureHeight* mMeasureHeight{nullptr};
    LineProperties *mLineProperties = nullptr;
    UIHandle *muiHandle;
    bool mEnterLineZone{false};
    bool mLenghtShow;
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<osg::Image> mIcon;
};

#endif // LINE_H

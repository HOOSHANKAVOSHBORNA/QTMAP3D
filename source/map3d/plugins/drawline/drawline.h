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
    bool setup() override;

protected:
    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
//    virtual bool mouseDoubleClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;

private:
    void startDrawLine();
    void startDrawMeasureHeight();
    bool drawingMeasureHeight(const osgGA::GUIEventAdapter& event);
    bool mouseMoveMeasureHeightDrawing(const osgGA::GUIEventAdapter& event);
    bool drawingLine(const osgGA::GUIEventAdapter& event);
    bool cancelDrawingLine(const osgGA::GUIEventAdapter& event);
    bool mouseMoveDrawing(const osgGA::GUIEventAdapter& event);
    bool finishDrawing(const osgGA::GUIEventAdapter& event, osg::Node *nodeEditor = nullptr);
    osgEarth::Annotation::PlaceNode *makeIconNode();


private:
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    enum class Type{NONE, LINE, RULER, HEIGHT, SLOPE};
    Type mType;
    LineNode* mLine{nullptr};
    MeasureHeight* mMeasureHeight{nullptr};
    LineProperties *mLineProperties = nullptr;
    bool mEnterLineZone{false};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<osg::Image> mIcon;
};

#endif // LINE_H

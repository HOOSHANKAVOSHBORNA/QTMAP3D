#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "plugininterface.h"
#include "polygonproperties.h"
#include "polygon.h"

#define DRAW_LAYER_NAME "Polygon"

class DrawPolygon : public PluginInterface
{
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawPolygon.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit DrawPolygon(QObject *parent = nullptr);
//    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
//    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup() override;

    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
//    virtual bool mouseDoubleClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private slots:
    void onPolygonItemCheck(bool check);
private:
    void startDraw(const osgGA::GUIEventAdapter& event);
    bool drawing(const osgGA::GUIEventAdapter& event);
    void cancelDraw();
    void finishDraw();
    bool mouseMoveDrawing(const osgGA::GUIEventAdapter& event);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Polygon* mPolygon{nullptr};

    PolygonProperties* mPolygonProperties{nullptr};

    bool mEnterPolygonZone{false};    
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};

};

#endif // DRAWPOLYGON_H

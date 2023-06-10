#ifndef DRAWCONE_H
#define DRAWCONE_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>

#include "plugininterface.h"
#include "cone.h"
#include "coneProperties.h"

#define DRAW_LAYER_NAME "Cone"

class DrawCone : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCone.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawCone(QObject *parent = nullptr);
    bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    bool setup() override;

    void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    virtual bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private:
    bool startDraw(const osgGA::GUIEventAdapter& event);
    bool finishDrawing(const osgGA::GUIEventAdapter& event);
    bool cancelDrawing(const osgGA::GUIEventAdapter& event);

    osgEarth::Annotation::PlaceNode* makeIconNode();

private:
    enum class DrawingState{START, DRAWING, FINISH};
    DrawingState mDrawingState;
    Cone* mCone{nullptr};
    bool mEnterConeZone{false};

    ConeProperties *mConeProperties{nullptr};

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
};

#endif // DRAWCONE_H

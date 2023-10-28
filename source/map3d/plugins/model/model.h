#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "attackerModelNode.h"
#include "plugininterface.h"
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/AnnotationLayer>
#include "simpleModelNode.h"
#include <osg/PolygonMode>
#include <osg/Fog>

#define MODEL "Model"
#define TREE "Tree"
#define CAR "Car"
#define AIRPLANE "Airplane"
#define TANK "Tank"

class Model : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "model.json")
    Q_INTERFACES(PluginInterface)

public:
    enum class State{
        NONE,
        READY,
        MOVING,
        CANCEL,
        CONFIRM
    };
    enum class Type{
        SIMPLE,
        MOVEABLE,
        FLYABLE,
        ATTACKER
    };

public:
    explicit Model(QObject *parent = nullptr);
    ~Model();
    bool setup();
    void makeIconNode(const QString &fileName);
    osgEarth::Annotation::PlaceNode *iconNode() const;
    Model::State state() const;
    void setState(Model::State newState);

    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

    static osgEarth::Symbology::Style &getDefaultStyle();
public slots:
    void onTreeItemCheck (bool check);
    void onCarItemCheck (bool check);
    void onAirplanItemCheck (bool check);
    void onTankItemCheck (bool check);
    void onStatusItemCheck (bool check);
    void onModeChanged(bool is3DView);

    void addUpdateFlyableNode(NodeData *nodeData);
    void addUpdateNode(NodeData *nodeData);
    void addUpdateStatusNode(StatusNodeData *statusnNodeData);
    void addUpdateMovableNode(NodeData *nodeData);

protected:
    void initModel(const osgEarth::GeoPoint &geoPos);;
    void moving(osgEarth::GeoPoint &geoPos);;
    void confirm();
    void cancel();

private:
    SimpleModelNode* pick(float x, float y);

private:
    Type mType;
    static int mCount;
    bool mIs3D;
    State mState{State::NONE};

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<CompositeAnnotationLayer> mModelNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mSimpleNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mMoveableNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mAttackerNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mFlyableNodelLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mStatusNodelLayer{nullptr};

    osg::ref_ptr<SimpleModelNode> mCurrentModel {nullptr};
    QMap<int, osg::ref_ptr<FlyableModelNode>> mFlyableNodeMap;
    QMap<int, osg::ref_ptr<SimpleModelNode>> mNodeMap;
    QMap<int, osg::ref_ptr<MoveableModelNode>> mMovableNodeMap;
    QMap<int, osg::ref_ptr<AttackerModelNode>> mAttackerNodeMap;
};

#endif // MODEL_H

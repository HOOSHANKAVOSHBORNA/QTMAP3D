#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <circle.h>
#include <osg/Fog>
#include <osg/PolygonMode>
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>

#include "dataManager.h"
#include "flyableModelNode.h"
#include "moveableModelNode.h"
#include "plugininterface.h"
#include "property.h"
#include "simpleModelNode.h"

#define MODEL "Model"
#define TREE "Tree"
#define CAR "Car"
#define AIRPLANE "Airplane"

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
        FLYABLE
    };

public:
    explicit Model(QObject *parent = nullptr);
    ~Model();
    bool setup();
    void makeIconNode(const QString &fileName);
    osgEarth::Annotation::PlaceNode *iconNode() const;
    Model::State state() const;
    void setState(Model::State newState);

    bool mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    void updatePropertyItem(const osgEarth::GeoPoint &geoPos, State nodeState);
    void updateNodeFromProperty();

    static osgEarth::Symbology::Style &getDefaultStyle();

public slots:
    void onTreeItemCheck (bool check);
    void onCarItemCheck (bool check);
    void onAirplanItemCheck (bool check);
    void onStatusItemCheck (bool check);
    void onModeChanged(bool is3DView);

protected:
    void initModel(const osgEarth::GeoPoint &geoPos);;
    void moving(osgEarth::GeoPoint &geoPos);
    void confirm();
    void cancel();
    void createProperty(QString name);

private:
    SimpleModelNode* pick(float x, float y);
    NodeData* sampleNodeData(std::string name, std::string url2d, std::string url3d, std::string imgSrc, std::string iconSrc, osgEarth::GeoPoint geopos);

private:
    Type mType;
    static int mCount;
    bool mIs3D;
    State mState{State::NONE};
    int mBulletID;

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<CompositeAnnotationLayer> mModelNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mSimpleNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mMoveableNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mFlyableNodelLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mStatusNodelLayer{nullptr};
    osg::ref_ptr<SimpleModelNode> mCurrentModel {nullptr};
    osg::ref_ptr<SimpleModelNode> mPickModelNode {nullptr};

    NodeData* mNodeData{nullptr};
    DataManager *mDataManager;
    QQuickItem *mItem;
    PropertyItem *mProperty;
};

#endif // MODEL_H

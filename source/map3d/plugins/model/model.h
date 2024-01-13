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
//#include "flyableModelNode.h"
//#include "moveableModelNode.h"
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

    bool mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

    static osgEarth::Symbology::Style &getDefaultStyle();
    DataManager *getDataManager();

public slots:
    void onTreeItemCheck (bool check);
    void onCarItemCheck (bool check);
    void onAirplanItemCheck (bool check);
    void onTankItemCheck(bool check);
    void onModeChanged(bool is3DView);

protected:
    void initModel(const osgEarth::GeoPoint &geoPos);;
    void moving(osgEarth::GeoPoint &geoPos);
    void confirm();
    void cancel();

private:
    SimpleModelNode* pick(float x, float y);
//    NodeData* sampleNodeData(std::string name, std::string url2d, std::string url3d, std::string imgSrc, std::string iconSrc);

private:
    Type mType;
    static int mCount;
    bool mIs3D;
    State mState{State::NONE};
    int mBulletID;

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
//    osg::ref_ptr<CompositeAnnotationLayer> mModelNodeLayer{nullptr};
//    osg::ref_ptr<ParenticAnnotationLayer> mSimpleNodeLayer{nullptr};
//    osg::ref_ptr<ParenticAnnotationLayer> mMoveableNodeLayer{nullptr};
//    osg::ref_ptr<ParenticAnnotationLayer> mFlyableNodelLayer{nullptr};
//    osg::ref_ptr<ParenticAnnotationLayer> mStatusNodelLayer{nullptr};
    osg::ref_ptr<SimpleModelNode> mCurrentModel {nullptr};
    osg::ref_ptr<SimpleModelNode> mPickModelNode {nullptr};
    osg::ref_ptr<SimpleModelNode> mHighliteModelNode {nullptr};

    NodeData mNodeData;
    LayerData mLayerData;
//    LayerData mLayerDataFixed;
//    LayerData mLayerDataMovable;
//    LayerData mLayerDataFlyable;
//    osg::ref_ptr<NodeData> mBaseNodeData{nullptr};
    DataManager *mDataManager;
//    QQuickItem *mItem;
    Property *mProperty;
};

#endif // MODEL_H

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <circle.h>
#include <osg/PolygonMode>
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>

#include "dataManager.h"
#include "nodeList.h"
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

private:
    NodeType mType;
    static int mCount;
    bool mIs3D;
    State mState{State::NONE};
    int mBulletID;

    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<SimpleModelNode> mCurrentModel {nullptr};
    osg::ref_ptr<SimpleModelNode> mPickModelNode {nullptr};
    osg::ref_ptr<SimpleModelNode> mHighliteModelNode {nullptr};

    NodeData mNodeData;
    LayerData mLayerData;
    DataManager *mDataManager;
    Property *mProperty;
    NodeList *mNodeList;
};

#endif // MODEL_H

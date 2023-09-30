#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "plugininterface.h"
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/AnnotationLayer>
#include "simpleModelNode.h"

#include <osg/PolygonMode>
#include <osg/Fog>
#include "sphereNode.h"
#include "circle.h"

#include <statusnode.h>

#include "cone.h"


#define MODEL "Model"
#define TREE "Tree"
#define CAR "Car"
#define AIRPLANE "Airplane"
#define STATUS "Status"

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
        INFO
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
    bool clicked() const;

public slots:
    static osgEarth::Symbology::Style &getDefaultStyle();
    void onTreeItemCheck (bool check);
    void onCarItemCheck (bool check);
    void onAirplanItemCheck (bool check);
    void onStatusItemCheck (bool check);
    void onModeChanged(bool is3DView);

    void addUpdateFlyableNode(NodeData *nodeData);
    void addUpdateStatusNode(StatusNodeData *statusnNodeData);

protected:
    void initModel(const osgEarth::GeoPoint &geoPos);;
    void moving(osgEarth::GeoPoint &geoPos);;
    void confirm();
    void cancel();;
private:
    State mState{State::NONE};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<CompositeAnnotationLayer> mModelNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mSimpleNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mMoveableNodeLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mFlyableNodelLayer{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mStatusNodelLayer{nullptr};
    osg::ref_ptr<SimpleModelNode> mCurrentModel {nullptr};
    osg::ref_ptr<StatusNode> mStatusModel {nullptr};

    SimpleModelNode* pick(float x, float y);
    SimpleModelNode* mSelectedModelNode{nullptr};
    bool mClicked{false};
    osg::ref_ptr<osg::PolygonMode> mPm;
    Type mType;
    osg::ref_ptr<SphereNode> mSphere{nullptr};
    osg::ref_ptr<Circle> mCircle{nullptr};
    osg::ref_ptr<Cone> mCone{nullptr};
    static int mCount;
    double mCurrentModelSize;
    bool mIs3D;
private:
    QMap<int, osg::ref_ptr<FlyableModelNode>> mFlyableNodeMap;
    QMap<int, osg::ref_ptr<StatusNode>> mStatusNodeMap;

    QImage *mRenderImage{nullptr};
    static constexpr int LABEL_IMAGE_WIDTH = 90;
    static constexpr int LABEL_IMAGE_HEIGHT = 20;
    void updateModelDataLabel(std::string name);
    osg::ref_ptr<osg::Image> mImageLabel;
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mLabelNode;

};

#endif // MODEL_H

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

    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool clicked() const;

public slots:
    static osgEarth::Symbology::Style &getDefaultStyle();
    void onTreeItemCheck (bool check);
    void onCarItemCheck (bool check);
    void onAirplanItemCheck (bool check);
    void onModeChanged(bool is3DView);
    void addFlyableFromService(QJsonDocument *json);

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
    osg::ref_ptr<SimpleModelNode> mCurrentModel {nullptr};

    SimpleModelNode* pick(float x, float y);
    SimpleModelNode* mSelectedModelNode{nullptr};
    bool mClicked{false};
    osg::ref_ptr<osg::PolygonMode> mPm;
    Type mType;
    osg::ref_ptr<SphereNode> mSphere{nullptr};
    osg::ref_ptr<Circle> mCircle{nullptr};
    static int mCount;
    double mCurrentModelSize;
    bool mIs3D;

};

#endif // MODEL_H

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "plugininterface.h"
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/AnnotationLayer>
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
    Model::State state() const;
    void setState(Model::State newState);
    osgEarth::Annotation::AnnotationLayer *modelLayer();

    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
    bool mouseMoveEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
public slots:
    static osgEarth::Symbology::Style &getDefaultStyle();
    void onTreeItemCheck (bool check);
    void onCarItemCheck (bool check);
    void onAirplanItemCheck (bool check);
protected:
    void initModel(const osgEarth::GeoPoint &geoPos);;
    void moving(osgEarth::GeoPoint &geoPos);;
    void confirm();
    void cancel();;
private:
    State mState{State::NONE};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mIconNode{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mModelLayer{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mTreelLayer{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mCarlLayer{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mAirplanelLayer{nullptr};
    osg::ref_ptr<SimpleModelNode> mModelNode{nullptr};
    osg::ref_ptr<SimpleModelNode> mCurrentModel {nullptr};

    static int mCount;

};

#endif // MODEL_H

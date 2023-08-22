#ifndef SIMPLEMODELNODE_H
#define SIMPLEMODELNODE_H
#include "mapItem.h"
#include <QObject>
#include <osgEarthAnnotation/ModelNode>

class MoveableModelNode;
class FlyableModelNode;

class SimpleModelNode : public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    SimpleModelNode(MapItem* mapControler, const std::string& modelUrl, const std::string& iconUrl, QObject *parent = nullptr);
    virtual SimpleModelNode* getNewModel();

    MapItem *mapItem() const;

    std::string iconUrl() const;

    std::string modelUrl() const;
    bool mIs3D{false};


    bool getScalability() const;
    void setScalability(bool newScalability);

    virtual SimpleModelNode* asSimpleModelNode(){return this;}
    virtual MoveableModelNode* asMoveableModel(){return nullptr;}
    virtual FlyableModelNode* asFlyableModelNode(){return nullptr;}

private slots:
    void onModeChanged(bool is3DView);

private:
    osg::ref_ptr<osg::Switch> mSwitchNode;
    osg::ref_ptr<osg::LOD> m3DNode;
    osg::ref_ptr<osg::Geode> m2DNode;

    std::string mModelUrl;
    std::string mIconUrl;
    MapItem *mMapItem;
    bool scalability{true};



};

#endif // SIMPLEMODELNODE_H

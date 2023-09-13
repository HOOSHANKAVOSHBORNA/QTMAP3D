#ifndef SIMPLEMODELNODE_H
#define SIMPLEMODELNODE_H
#include "mapItem.h"
#include <QObject>
#include <osgEarthAnnotation/ModelNode>
#include "modelAutoScaler.h"
#include <osgFX/Outline>
#include <osgFX/Scribe>
#include <circle.h>
#include <cone.h>
#include <osg/ComputeBoundsVisitor>


class MoveableModelNode;
class FlyableModelNode;

class SimpleModelNode : public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    SimpleModelNode(MapItem* mapControler, const std::string& modelUrl, const std::string& iconUrl, QObject *parent = nullptr);
    MapItem *mapItem() const;
    std::string iconUrl() const;
    std::string modelUrl() const;
    bool mIs3D{false};

    bool getScalability() const;
    void setScalability(bool newScalability);

    virtual SimpleModelNode* asSimpleModelNode(){return this;}
    virtual MoveableModelNode* asMoveableModelNode(){return nullptr;}
    virtual FlyableModelNode* asFlyableModelNode(){return nullptr;}

    void selectModel(bool isSelected);

private slots:
    void onModeChanged(bool is3DView);

private:
    osg::ref_ptr<osg::Switch> mSwitchNode;
//    osg::ref_ptr<osgFX::Scribe> mHighlight;
    osg::ref_ptr<osg::LOD> m3DNode;
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<ModelAutoScaler> mScaler;
    osg::ref_ptr<osg::Group> root;

    osg::ref_ptr<Circle> mCircle;
    osg::ref_ptr<Cone> mCone ;


    std::string mModelUrl;
    std::string mIconUrl;
    MapItem *mMapItem;
    bool mScalability{true};
    bool mIsSelected{false};



};

#endif // SIMPLEMODELNODE_H

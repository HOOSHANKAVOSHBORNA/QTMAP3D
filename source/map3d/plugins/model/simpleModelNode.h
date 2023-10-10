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

    virtual SimpleModelNode* asSimpleModelNode(){return this;}
    virtual MoveableModelNode* asMoveableModelNode(){return nullptr;}
    virtual FlyableModelNode* asFlyableModelNode(){return nullptr;}

    void selectModel();
    bool isAutoScale() const;
    void setAutoScale(bool newIsAutoScale);

    NodeData *nodeData() const;
    void setNodeData(NodeData *newNodeData);
    void setModelColor(osgEarth::Color color);

private slots:
    void onModeChanged(bool is3DView);

private:
    osg::ref_ptr<osg::Image> mImage;
    osg::ref_ptr<osg::Node> mSimpleNode;
    osg::ref_ptr<osg::Switch> mSwitchNode;
    osg::ref_ptr<osg::LOD> m3DNode;
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<Circle> mCircleSelectNode;
    osg::ref_ptr<Cone> mConeSelecteNode;

    osg::ref_ptr<ModelAutoScaler> mAutoScaler;
    std::string mModelUrl;
    std::string mIconUrl;
    MapItem *mMapItem;
    bool mIs3D{false};
    bool mIsAutoScale{true};
    bool mIsSelected{false};
    NodeData* mNodeData;
private:
    static QMap<std::string, osg::ref_ptr<osg::Node>> mNodes3D;
    static QMap<std::string, osg::ref_ptr<osg::Image>> mImages2D;
};

#endif // SIMPLEMODELNODE_H

#ifndef SIMPLEMODELNODE_H
#define SIMPLEMODELNODE_H
#include "mapItem.h"
#include <QObject>
#include <osgEarthAnnotation/ModelNode>
#include "modelAutoScaler.h"

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

private slots:
    void onModeChanged(bool is3DView);

private:
    osg::ref_ptr<osg::Switch> mSwitchNode;
    osg::ref_ptr<osg::LOD> m3DNode;
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<ModelAutoScaler> mScaler;

    std::string mModelUrl;
    std::string mIconUrl;
    MapItem *mMapItem;
    bool mScalability{true};



};

#endif // SIMPLEMODELNODE_H

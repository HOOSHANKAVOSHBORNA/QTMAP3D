#ifndef SIMPLEMODELNODE_H
#define SIMPLEMODELNODE_H
#include "mapItem.h"
#include <QObject>
#include <osgEarthAnnotation/ModelNode>

class simpleModelNode : public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    simpleModelNode(MapItem* mapControler, const std::string& modelUrl, const std::string& iconUrl, QObject *parent = nullptr);
    virtual simpleModelNode* getNewModel();

    MapItem *mapItem() const;

    std::string iconUrl() const;

    std::string modelUrl() const;
    bool mIs3D{false};
    double getMlenght() const;
    void setMlenght(double newMlenght);

private slots:
    void onModeChanged(bool is3DView);

private:
    osg::ref_ptr<osg::Switch> mSwitchNode;
    osg::ref_ptr<osg::LOD> m3DNode;
    osg::ref_ptr<osg::Geode> m2DNode;

    std::string mModelUrl;
    std::string mIconUrl;
    MapItem *mMapItem;
    double mlenght;


};

#endif // SIMPLEMODELNODE_H

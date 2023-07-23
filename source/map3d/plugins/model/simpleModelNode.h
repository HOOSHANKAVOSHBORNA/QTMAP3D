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
    osg::ref_ptr<osg::Image> getM2DImage() const;
    void setM2DImage(const osg::ref_ptr<osg::Image> &newM2DImage);

protected:
    std::string mModelUrl;
    std::string mIconUrl;
    MapItem *mMapItem;
    bool mIs3D{false};
    osg::ref_ptr<osg::Geode> mImgGeod;

private:
    osg::ref_ptr<osg::Image> m2DImage;
    osg::ref_ptr<osg::Switch> mMode;

private slots:
    void onModeChanged(bool is3DView);
signals:

};

#endif // SIMPLEMODELNODE_H

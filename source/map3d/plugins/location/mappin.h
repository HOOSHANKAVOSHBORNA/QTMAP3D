#ifndef MAPPIN_H
#define MAPPIN_H


#include <QObject>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "mapItem.h"
#include "modelAutoScaler.h"




class MapPin: public QObject , public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    MapPin(MapItem *mapControler, osg::Vec4f color);
    MapItem *mapItem() const;
    std::string iconUrl() const;
    std::string modelUrl() const;
    bool mIs3D{false};
    osgEarth::Symbology::Style getDefaultStyle();


public slots:
    void onModeChanged(bool is3DView);


private:

    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osg::Node> m3DPin;
    osg::ref_ptr<osg::Geode> m2DPin;
    ModelAutoScaler *mScaler;
    MapItem mMapItem;


    bool is3D;
};

#endif // MAPPIN_H

#ifndef CLOUD_H
#define CLOUD_H

#include <QObject>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>
#include <osgEarth/Lighting>
#include <osg/LightSource>



class Cloud: public QObject , public osgEarth::Annotation::GeoPositionNode
{
    Q_OBJECT
public:
    Cloud(MapItem *map);
    osg::ref_ptr<osgParticle::SmokeEffect> getCloud();


public slots:
    void onModeChanged(bool is3DView);


private:

//    osg::ref_ptr<osg::Geode> m2DNode;

    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud1{nullptr};
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud2{nullptr};
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud3{nullptr};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> m2DNode ;


    bool is3D;
};

#endif // CLOUD_H

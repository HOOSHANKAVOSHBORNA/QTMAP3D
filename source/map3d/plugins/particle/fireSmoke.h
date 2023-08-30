#ifndef FIRESMOKE_H
#define FIRESMOKE_H


#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>
#include <osgEarthAnnotation/PlaceNode>
#include <QObject>



class FireSmoke:public QObject , public osgEarth::Annotation::GeoPositionNode
{
     Q_OBJECT
public:
    FireSmoke(MapItem *map);
    osg::PositionAttitudeTransform *getFireSmoke();
public slots:
    void onModeChanged(bool is3DView);
private:
    osg::ref_ptr<osgParticle::FireEffect> mFire;
    osg::ref_ptr<osgParticle::SmokeEffect> mSmoke;
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> m2DNode ;

    bool is3D;
};




#endif // FIRESMOKE_H

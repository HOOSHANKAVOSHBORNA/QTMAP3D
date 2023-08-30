#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "mapItem.h"
#include <QObject>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/FireEffect>
#include <osgEarthAnnotation/PlaceNode>

class Explosion :public QObject , public osgEarth::Annotation::GeoPositionNode
{
    Q_OBJECT
public:
    explicit Explosion(MapItem *map);
    osg::PositionAttitudeTransform *getExplosion();
public slots:
    void onModeChanged(bool is3DView);

private:
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> m2DNode ;
    osg::ref_ptr<osgParticle::ExplosionEffect> mExplosion;
    osg::ref_ptr<osg::PositionAttitudeTransform> pSphereGroup ;

    osg::ref_ptr<osgParticle::ExplosionEffect> explosion1 ;
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion2 ;
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion3 ;
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion4 ;
    osg::ref_ptr<osgParticle::ExplosionDebrisEffect> debris1;
    osg::ref_ptr<osgParticle::SmokeEffect> smoke ;
    osg::ref_ptr<osgParticle::FireEffect> fire ;

    bool is3D;
};

#endif // EXPLOSION_H

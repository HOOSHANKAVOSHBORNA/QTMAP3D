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

class Explosion : public osgEarth::Annotation::GeoPositionNode
{
public:
    explicit Explosion(MapItem *map);
    osg::PositionAttitudeTransform *getExplosion();
private:
    osg::ref_ptr<osgParticle::ExplosionEffect> mExplosion;
    osg::ref_ptr<osg::PositionAttitudeTransform> pSphereGroup = new osg::PositionAttitudeTransform;
};

#endif // EXPLOSION_H

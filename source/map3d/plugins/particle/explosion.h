#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "mapItem.h"
#include <QObject>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/FireEffect>
#include <osgEarthAnnotation/PlaceNode>

class AutoScaler;

class Explosion :public QObject , public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    explicit Explosion(MapItem *map, double duration = 30);
    ~Explosion() override;
    void setScaleRatio(double ratio);
public slots:
    void onModeChanged(bool is3DView);

private:
    osg::ref_ptr<osg::Group> mRoot{nullptr};
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<osg::Image> mImage;
    osg::ref_ptr<osgParticle::ExplosionEffect> mExplosion;

    osg::ref_ptr<osgParticle::ExplosionEffect> mExplosion1;
    osg::ref_ptr<osgParticle::ExplosionEffect> mExplosion2;
    osg::ref_ptr<osgParticle::ExplosionEffect> mExplosion3;
    osg::ref_ptr<osgParticle::ExplosionEffect> mExplosion4;
    osg::ref_ptr<osgParticle::ExplosionDebrisEffect> mDebris;
    osg::ref_ptr<osgParticle::SmokeEffect> mSmoke;
    osg::ref_ptr<osgParticle::FireEffect> mFire;

    bool mIs3D;
    AutoScaler* mAutoScaler;
    QTimer *mTimerDuration{nullptr};
};

#endif // EXPLOSION_H

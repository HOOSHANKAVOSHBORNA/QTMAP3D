#ifndef FIRESMOKE_H
#define FIRESMOKE_H

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>
#include <osgEarthAnnotation/PlaceNode>
#include <QObject>

class AutoScaler;

class FireSmoke:public QObject , public osgEarth::Annotation::ModelNode
{
     Q_OBJECT
public:
    FireSmoke(MapItem *map, double duration = 30);
    ~FireSmoke();
    void setScaleRatio(double ratio);
public slots:
    void onModeChanged(bool is3DView);
private:
    osg::ref_ptr<osg::Group> mRoot{nullptr};
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<osg::Image> mImage;
    osg::ref_ptr<osgParticle::FireEffect> mFire;
    osg::ref_ptr<osgParticle::SmokeEffect> mSmoke;

    bool mIs3D;
    AutoScaler* mAutoScaler;
    QTimer *mTimerDuration{nullptr};
};




#endif // FIRESMOKE_H

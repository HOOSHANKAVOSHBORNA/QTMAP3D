#ifndef RAIN_H
#define RAIN_H

#include <QObject>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgParticle/PrecipitationEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>

class AutoScaler;

class Rain:public QObject, public osgEarth::Annotation::ModelNode {
    Q_OBJECT
public:
    Rain(MapItem *map, double duration = 30);
    ~Rain() override;
    void setScaleRatio(double ratio);

public slots:
    void onModeChanged(bool is3DView);
private:
    osg::ref_ptr<osgParticle::PrecipitationEffect> mRain;
    osg::ref_ptr<osgEarth::Annotation::CircleNode> mPrecipitationRange;
    osg::ref_ptr<osg::Group> mRoot{nullptr};
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<osg::Image> mImage;

    bool mIs3D;
    AutoScaler* mAutoScaler;
    QTimer *mTimerDuration{nullptr};

};
#endif // RAIN_H

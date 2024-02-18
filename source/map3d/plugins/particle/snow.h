#ifndef SNOW_H
#define SNOW_H

#include <QObject>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/PrecipitationEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>



class AutoScaler;

class Snow:public QObject, public osgEarth::Annotation::ModelNode {
    Q_OBJECT
public:
    Snow(MapItem *map, double duration = 30);
    ~Snow() override;
    void setScaleRatio(double ratio);

public slots:
    void onModeChanged(bool is3DView);
private:
    osg::ref_ptr<osgParticle::PrecipitationEffect> mSnow;
    osg::ref_ptr<osgEarth::Annotation::CircleNode> mPrecipitationRange;
    osg::ref_ptr<osg::Group> mRoot{nullptr};
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<osg::Image> mImage;

    bool mIs3D;
    AutoScaler* mAutoScaler;
    QTimer *mTimerDuration{nullptr};
};


#endif // SNOW_H

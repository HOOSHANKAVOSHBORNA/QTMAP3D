#ifndef CLOUD_H
#define CLOUD_H

#include <QObject>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>

class AutoScaler;

class Cloud: public QObject , public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    Cloud(MapItem *map, double duration = 30);
    ~Cloud() override;
    void setScaleRatio(double ratio);

public slots:
    void onModeChanged(bool is3DView);

private:
    osg::ref_ptr<osg::Group> mRoot{nullptr};
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<osg::Image> mImage;
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud1{nullptr};
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud2{nullptr};
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud3{nullptr};

    bool mIs3D;
    AutoScaler* mAutoScaler;
    QTimer *mTimerDuration{nullptr};
};

#endif // CLOUD_H

#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include "basemodel.h"
#include "map3dwidget.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>

#include <QObject>
#include <QMouseEvent>
class MapAnimationPathCallback;
class EventCallback;
class Airplane: public BaseModel
{
public:
    Airplane(Map3dWidget *value, osgEarth::MapNode* mapNode, osg::Node* node, QObject* parent = nullptr);
    void flyTo(const osg::Vec3d& pos, double heading, double speed);
    void stop() override;
    void setTruckModel(osgEarth::Annotation::ModelNode* truckModel);
    osgEarth::Annotation::ModelNode *getTruckModel() const;
protected:
    void mousePushEvent(bool onModel, const osgGA::GUIEventAdapter& ea) override;
    void mouseMoveEvent(bool onModel, const osgGA::GUIEventAdapter& ea) override;
    void cameraRangeChanged(double range) override;
private:
    void addEffect(double emitterDuration);
    void removeEffect();
    void select(bool val);
private:
    Map3dWidget* mMap3dWidget{nullptr};
    ModelAnimationPathCallback* mAnimationPathCallback{nullptr};
    osgEarth::Annotation::ModelNode* mTruckModel;
//    osg::ref_ptr<osg::Geode> mGeodeParticle;
    osg::ref_ptr<osgParticle::SmokeTrailEffect> mSmoke;
    osg::ref_ptr<osgParticle::FireEffect> mFire;
    osg::ref_ptr<osg::Switch> mRoot;

    bool mIsStop{false};
};

#endif // FLYINGMODEL_H

#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include "basemodel.h"
#include "mapcontroller.h"
#include <plugininterface.h>

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
    Q_OBJECT
public:
    Airplane(MapController *value, UIHandle* uiHandle, osgEarth::MapNode* mapNode, osg::Node* node, QObject* parent = nullptr);
    void flyTo(const osg::Vec3d& pos, double heading, double speed);
    void stop() override;
    void setTruckModel(osgEarth::Annotation::ModelNode* truckModel);
    osgEarth::Annotation::ModelNode *getTruckModel() const;
    void setInformation(QString info);
    Q_INVOKABLE
    void iw2D3DButtonClicked();
    Q_INVOKABLE
    void iwRouteButtonClicked();
    Q_INVOKABLE
    void iwFollowButtonClicked();
    Q_INVOKABLE
    void iwMoreButtonClicked();
protected:
    virtual void mousePushEvent(bool onModel, const osgGA::GUIEventAdapter& ea) override;
    virtual void curentPosition(osgEarth::GeoPoint pos) override;
private:
    void addEffect(double emitterDuration);
    void removeEffect();
private:
    MapController* mMapController{nullptr};
    ModelAnimationPathCallback* mAnimationPathCallback{nullptr};
    osgEarth::Annotation::ModelNode* mTruckModel;
//    osg::ref_ptr<osg::Geode> mGeodeParticle;
    osg::ref_ptr<osgParticle::SmokeTrailEffect> mSmoke;
    osg::ref_ptr<osgParticle::FireEffect> mFire;

    bool mIsStop{false};
    bool mIsRoute{false};
    UIHandle* mUIHandle;
    QString mInformation;
    osg::ref_ptr<osg::Vec3Array> mLocationPoints;
    osg::ref_ptr<osg::Vec3Array> mTempLocationPoints;
};

#endif // FLYINGMODEL_H

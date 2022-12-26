#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include "basemodel.h"
#include "mapcontroller.h"
#include "plugininterface.h"
#include "datamanager.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>

#include <QObject>
#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class MapAnimationPathCallback;
class EventCallback;
class Aircraft: public BaseModel
{
    Q_OBJECT
public:
    Aircraft(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle* uiHandle, QObject* parent = nullptr);
    void flyTo(const osg::Vec3d& pos, double heading, double speed);
    void stop() override;
    void setTruckModel(osgEarth::Annotation::ModelNode* truckModel);
    osgEarth::Annotation::ModelNode *getTruckModel() const;
    void setInformation(AircraftInfo info);
public slots:
    void iw2D3DButtonClicked();
    void iwRouteButtonClicked();
    void iwFollowButtonClicked();
    void iwMoreButtonClicked();
    void onModeChanged(bool is3DView);
    void onContextmenuItemClicked(int index, QString systemName);
public:
    virtual void frameEvent()override;
    virtual void mousePressEvent(QMouseEvent *event, bool onModel) override;
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
    AircraftInfo mInformation;
    osg::ref_ptr<osg::Vec3Array> mLocationPoints;
    osg::ref_ptr<osg::Vec3Array> mTempLocationPoints;
    QQmlEngine *mQmlEngine;
    QQuickItem *mCurrentContextMenuItem;
};

#endif // FLYINGMODEL_H

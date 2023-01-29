#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include "defenseModelNode.h"
#include "mapcontroller.h"
#include "plugininterface.h"
#include "dataManager.h"
#include "aircraftInformation.h"
#include "line.h"
#include "contextMenu.h"

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
class AircraftModelNode: public DefenseModelNode
{
    Q_OBJECT
public:
    AircraftModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle* uiHandle, QObject* parent = nullptr);
    void flyTo(const osg::Vec3d& pos, double heading, double speed);
    void stop() override;
    void setTruckModel(osgEarth::Annotation::ModelNode* truckModel);
    osgEarth::Annotation::ModelNode *getTruckModel() const;
    void setInformation(AircraftInfo info);
    void goOnTrack();
public slots:

    void onLeftButtonClicked(bool val);
public:
    void frameEvent()override;
    void mousePressEvent(QMouseEvent *event, bool onModel) override;
    void curentPosition(osgEarth::GeoPoint pos) override;
private slots:
    void onGotoButtonClicked();
    void onRouteButtonToggled(bool check);
    void onTrackButtonToggled(bool check);
    void onModeChanged(bool is3DView);
    void onContextmenuItemClicked(int index, QString systemName);
private:
    void setModelColor(AircraftInfo::Identify identify);
    void create2DImageColore(osgEarth::Color color);
    void showInfoWidget();
    void addEffect(double emitterDuration);
    void removeEffect();
    void updateOrCreateLabelImage();
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
    ContextMenu *mCurrentContextMenu = nullptr;

    Line *mRouteLine;
    Line *mTempRouteLine;
    AircraftInformation *mAircraftinformation{nullptr};

    static osg::ref_ptr<osg::Node> mNode3DRef;
    static constexpr int LABEL_IMAGE_WIDTH = 160;
    static constexpr int LABEL_IMAGE_HEIGHT = 170;
    QImage *mRenderTargetImage{nullptr};
    osg::ref_ptr<osg::Image> mLabelImage{nullptr};
};

#endif // FLYINGMODEL_H

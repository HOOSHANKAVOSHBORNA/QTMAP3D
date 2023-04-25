#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include "defenseModelNode.h"
#include "mapcontroller.h"
#include "plugininterface.h"
#include "listManager.h"
#include "aircraftInformation.h"
#include "linenode.h"
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
class SystemModelNode;

class AircraftModelNode: public DefenseModelNode
{
    Q_OBJECT
public:
    AircraftModelNode(MapController *mapControler, AircraftInfo::AircraftType aircraftType, QQmlEngine *qmlEngine, UIHandle* uiHandle, QObject* parent = nullptr);
    void flyTo(osgEarth::GeoPoint posGeo, double heading, double speed);
    void stop() override;
    void setInformation(AircraftInfo info);
    AircraftInfo getInformation() const;
    void goOnTrack();
public slots:
    void onLeftButtonClicked(bool val);
public:
    void frameEvent()override;
    void mousePressEvent(QMouseEvent *event, bool onModel) override;
    virtual void updateColors() override;

    SystemModelNode *getAssignment(int number) const;
    void addAssignment(int number, SystemModelNode *assignmentModelNode);
    void removeAssignment(int number);
    void acceptAssignment(int number, bool value);
    void clearAssignments(int exceptNumber = -1);
    bool hasAssignment();
    QMap<int, SystemModelNode *> getAssignments() const;

private slots:
    void onGotoButtonClicked();
    void onRouteButtonToggled(bool check);
    void onLatestPointsToggled(bool check);
    void onTrackButtonToggled(bool check);
    void onModeChanged(bool is3DView);
    void onContextmenuItemClicked(int index, QString systemName);
private:
    void changeModelColor(AircraftInfo::Identify identify);
    void showInfoWidget();
    void addEffect(double emitterDuration);
    void removeEffect();
    bool addNodeToLayer(osg::Node *node);
    bool removeNodeFromLayer(osg::Node *node);
    void change2DImageColore(osgEarth::Color color);
    void updateOrCreateLabelImage();
private:
    MapController* mMapController{nullptr};
    ModelAnimationPathCallback* mAnimationPathCallback{nullptr};
    osgEarth::Annotation::ModelNode* mTruckModel;
    //    osg::ref_ptr<osg::Geode> mGeodeParticle;
    osg::ref_ptr<osgParticle::SmokeTrailEffect> mSmoke;
    osg::ref_ptr<osgParticle::FireEffect> mFire;

    QMap<int, SystemModelNode*> mAssignmentMap;

    bool mIsStop{false};
    bool mIsRoute{false};
    UIHandle* mUIHandle;
    AircraftInfo mInformation;
    osg::ref_ptr<osg::Vec3Array> mLocationPoints;
    osg::ref_ptr<osg::Vec3Array> mTempLocationPoints;
    QQmlEngine *mQmlEngine;
    ContextMenu *mCurrentContextMenu = nullptr;

    osg::ref_ptr<LineNode> mRouteLine;
    osg::ref_ptr<LineNode> mLatestPointLine;
    osg::ref_ptr<LineNode> mTempLine;

    AircraftInformation *mAircraftinformation{nullptr};

    static osg::ref_ptr<osg::Node> mAircraft3DRef;
    static osg::ref_ptr<osg::Node> mFighter3DRef;
    static osg::ref_ptr<osg::Node> mMissile3DRef;
    static osg::ref_ptr<osg::Node> mHelicopter3DRef;
    static osg::ref_ptr<osg::Node> mDrone3DRef;
//    static osg::ref_ptr<osg::Image> m2dIcon;

    static constexpr int LABEL_IMAGE_WIDTH = 210;
    static constexpr int LABEL_IMAGE_HEIGHT = 230;
    QImage *mRenderTargetImage{nullptr};
    osg::ref_ptr<osg::Image> mLabelImage{nullptr};

    //osg::Image* m2DIcon;
    //osg::Image* mSelect2DIcon;

    double mCurrentHeading{500};
    osgEarth::GeoPoint mCurrentFlyPoint;

    osg::ref_ptr<osg::PositionAttitudeTransform> mPat2D;

    osg::ref_ptr<osg::Switch> mNode2DNormal;
    osg::ref_ptr<osg::Switch> mNode2DHovered;

    double mAutoScaleDefaultValue = 2.5;
    double mAutoScaleMinValue = 1;
    double mAutoScaleMaxValue = 500;
};

#endif // FLYINGMODEL_H

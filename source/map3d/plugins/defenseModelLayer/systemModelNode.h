#ifndef SYSTEM_H
#define SYSTEM_H

#include "mapcontroller.h"
#include "defenseModelNode.h"
#include "systemInformation.h"
#include "circle.h"
#include "sphereNode.h"
#include "polygone.h"
#include "aircraftModelNode.h"
#include "linenode.h"
#include "truck.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <QImage>
#include <QPainter>


class SystemModelNode: public DefenseModelNode
{
public:
    SystemModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle* uiHandle, QObject* parent = nullptr);

    void setInformation(const SystemInfo &info);
    SystemInfo getInformation() const;
    void setStatusInfo(const SystemStatusInfo &systemStatusInfo);
    void setCambatInfo(const SystemCambatInfo &systemCambatInfo);
    SystemCambatInfo getSystemCombatInfo() const;

    void addAssignment(int tn, AircraftModelNode *assignModelNode);
    AircraftModelNode *getAssignment(int tn) const;
    void acceptAssignment(int tn, bool value);
    void removeAssignment(int tn);
    void clearAssignments(int exceptTN = -1);
    QMap<int, AircraftModelNode *> getAssignments() const;

    void goOnTrack();
public slots:
    void onLeftButtonClicked(bool val);
public:
    virtual void frameEvent()override;
    void mousePressEvent(QMouseEvent *event, bool onModel) override; 
    virtual void updateColors() override;

protected:
private slots:
    void onGotoButtonClicked();
    void onRangeButtonToggled(bool check);
    void onWezButtonToggled(bool checked);
    void onMezButtonToggled(bool checked);
    void onActiveButtonToggled(bool checked);
    void onModeChanged(bool is3DView);

private:
    void searchPhase();
    void lockPhase(int tn);
    void firePhase(int tn);
    void killPhase(int tn);
    void noKillPhase(int tn);
//    bool hasAssigned();
    bool addNodeToLayer(osg::Node *node,  bool insert = false);
    bool removeNodeFromLayer(osg::Node *node);
    void showInfoWidget();
    void updateOrCreateLabelImage();
private:
    struct Assignment{
        AircraftModelNode *mModelNode{nullptr};
        osg::ref_ptr<LineNode> mLine;
        Assignment(MapController *mapControler);
        void accept();
        void updateLine(const osgEarth::GeoPoint &position);
    };
private:
    MapController* mMapController{nullptr};
    SystemInfo mInformation;
    SystemStatusInfo mStatusInfo;
    SystemCambatInfo mCambatInfo;
    UIHandle* mUIHandle;
    QQmlEngine *mQmlEngine;
    osg::ref_ptr<Circle> mRangeCircle;
    osg::ref_ptr<SphereNode> mMezSphere;
    osg::ref_ptr<Polygone> mWezPolygon;

    osg::ref_ptr<class TruckS> mTruckS;
    osg::ref_ptr<class TruckF> mTruckF;
    osg::ref_ptr<class TruckL> mTruckL;
    Rocket* mFiredRocket{nullptr};

    QMap<int, Assignment*> mAssignmentMap;
    SystemInformation *mSystemInformation{nullptr};

    AircraftModelNode* mTargetModelNode{nullptr};
private:
    QImage                  *mRenderTargetImage = nullptr;
    osg::ref_ptr<osg::Image> mLabelImage = nullptr;
    static constexpr int LABEL_IMAGE_WIDTH = 160;
    static constexpr int LABEL_IMAGE_HEIGHT = 190;

    osg::ref_ptr<osg::LOD> mLOD = nullptr;

    osg::ref_ptr<osg::Switch> mNode2DActive;
    osg::ref_ptr<osg::Switch> mNode2DDeactive;

    osg::ref_ptr<osgEarth::Annotation::CircleNode> mCircleNode = nullptr;
    osgEarth::Symbology::Style mCircleStyleActive;
    osgEarth::Symbology::Style mCircleStyleDeactive;
};

#endif // SYSTEM_H

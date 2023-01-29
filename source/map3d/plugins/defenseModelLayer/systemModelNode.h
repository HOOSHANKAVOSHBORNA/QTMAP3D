#ifndef SYSTEM_H
#define SYSTEM_H

#include "mapcontroller.h"
#include "defenseModelNode.h"
#include "systemInformation.h"
#include "circle.h"
#include "spherenode.h"
#include "polygone.h"
#include "aircraftModelNode.h"
#include "line.h"
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
    void goOnTrack();
public slots:
    void onLeftButtonClicked(bool val);
public:
    virtual void frameEvent()override;
    void mousePressEvent(QMouseEvent *event, bool onModel) override;

    DefenseModelNode *getAssignedModelNode() const;
    void setAssignedModelNode(DefenseModelNode *assignedModelNode);
    void unassignedModelNode();
    void fire();

protected:
private slots:
    void onGotoButtonClicked();
    void onRangeButtonToggled(bool check);
    void onWezButtonToggled(bool checked);
    void onMezButtonToggled(bool checked);
    void onActiveButtonToggled(bool checked);
    void onModeChanged(bool is3DView);

//    void setMissleCount(int numMissles);
//    void setDisplayText(QString displayText);
//    void setBCCStatus(QString bccStatus);
//    void setRadarSearchStatus(QString radarSearchStatus);

public:
//    int getMissleCount() const;
//    QString getDisplayText() const;
//    QString getBCCStatus() const;
//    QString getRadarSearchStatus() const;


private:
    void collision();
    void showInfoWidget();

    void updateOrCreateLabelImage();

private:
    MapController* mMapController{nullptr};
    SystemInfo mInformation;
    UIHandle* mUIHandle;
    QQmlEngine *mQmlEngine;
    osg::ref_ptr<Circle> mRangeCircle;
    osg::ref_ptr<SphereNode> mMezSphere;
    osg::ref_ptr<Polygone> mWezPolygon;
    DefenseModelNode* mAssignedModelNode{nullptr};
    Line* mAssignedLine;
    osg::ref_ptr<Truck> mTruck;
    Rocket* mFiredRocket{nullptr};
    bool mHit{false};


private:
    QImage                  *mRenderTargetImage = nullptr;
    osg::ref_ptr<osg::Image> mLabelImage = nullptr;
    static constexpr int LABEL_IMAGE_WIDTH = 160;
    static constexpr int LABEL_IMAGE_HEIGHT = 190;

//    int     mMissleCount = 3;
//    QString mDisplayText = "System0";
//    QString mBCCStatus = "us";
//    QString mRadarSearchStatus = "s";

};

#endif // SYSTEM_H

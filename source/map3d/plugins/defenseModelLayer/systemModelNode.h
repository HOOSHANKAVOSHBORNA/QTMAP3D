#ifndef SYSTEM_H
#define SYSTEM_H

#include "mapcontroller.h"
#include "defenseModelNode.h"
#include "systemInformation.h"
#include "circle.h"
#include "spherenode.h"
#include <polygone.h>

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>


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
protected:
private slots:
    void onGotoButtonClicked();
    void onRangeButtonToggled(bool check);
    void onWezButtonToggled(bool checked);
    void onMezButtonToggled(bool checked);
    void onActiveButtonToggled(bool checked);
    void onModeChanged(bool is3DView);
private:
    void showInfoWidget();
private:
    MapController* mMapController{nullptr};
    SystemInfo mInformation;
    UIHandle* mUIHandle;
    QQmlEngine *mQmlEngine;
    osg::ref_ptr<Circle> mRangeCircle;
    osg::ref_ptr<SphereNode> mMezSphere;
    osg::ref_ptr<Polygone> mWezPolygon;
};

#endif // SYSTEM_H

#ifndef STATION_H
#define STATION_H


#include "mapcontroller.h"
#include "defenseModelNode.h"
#include "dataManager.h"
#include "stationInformation.h"
#include "circle.h"
#include "polygone.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>


class StationModelNode: public DefenseModelNode
{
public:
    StationModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle* uiHandle, QObject* parent = nullptr);
    void setInformation(const StationInfo &info);
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
    void onVisibleButtonToggled(bool checked);
    void onActivateButtonToggled(bool checked);
    void onModeChanged(bool is3DView);
private:
    void showInfoWidget();
    void updateOrCreateLabelImage();
private:
    MapController* mMapController{nullptr};
    StationInfo mInformation;
    UIHandle* mUIHandle;
    QQmlEngine *mQmlEngine;
    osg::ref_ptr<Circle> mRangeCircle;
    osg::ref_ptr<Polygone> mVisiblePolygone;

    static osg::ref_ptr<osg::Node> mNode3DRef;
    static constexpr int LABEL_IMAGE_WIDTH = 180;
    static constexpr int LABEL_IMAGE_HEIGHT = 150;
    QImage *mRenderTargetImage{nullptr};
    osg::ref_ptr<osg::Image> mLabelImage{nullptr};
};


#endif // STATION_H

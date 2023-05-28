#ifndef STATION_H
#define STATION_H


#include "mapItem.h"
#include "defenseModelNode.h"
#include "stationInformation.h"
#include "circle.h"
#include "polygon.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>

class StationModelNode: public DefenseModelNode
{
public:
    StationModelNode(MapItem *mapControler, QQmlEngine *qmlEngine, UIHandle* uiHandle, QObject* parent = nullptr);
    void setInformation(const StationInfo &info);
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
    void onVisibleButtonToggled(bool checked);
    void onActivateButtonToggled(bool checked);
    void onModeChanged(bool is3DView);
private:
    void showInfoWidget();
    void updateOrCreateLabelImage();
private:
    MapItem* mMapController{nullptr};
    StationInfo mInformation;
    UIHandle* mUIHandle;
    QQmlEngine *mQmlEngine;
    osg::ref_ptr<Circle> mRangeCircle;
    osg::ref_ptr<Polygon> mVisiblePolygon;

    static osg::ref_ptr<osg::Node> mNode3DRef;
    static constexpr int LABEL_IMAGE_WIDTH = 210;
    static constexpr int LABEL_IMAGE_HEIGHT = 150;
    QImage *mRenderTargetImage{nullptr};
    osg::ref_ptr<osg::Image> mLabelImage{nullptr};


    osg::ref_ptr<osg::Switch> mNode2DActive;
    osg::ref_ptr<osg::Switch> mNode2DDeactive;

    osg::ref_ptr<osgEarth::Annotation::CircleNode> mCircleNode = nullptr;
    osgEarth::Symbology::Style mCircleStyleActive;
    osgEarth::Symbology::Style mCircleStyleDeactive;
    StationInformtion *mStationInformation = nullptr;
};


#endif // STATION_H

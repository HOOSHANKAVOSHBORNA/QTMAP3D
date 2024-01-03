#ifndef STATION_H
#define STATION_H

#include "defenseModelNode.h"
#include "stationInformation.h"
#include "circle.h"
#include "polygon.h"
#include "annotatedNode.h"
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>

namespace Station {
struct Data;
}

class StationModelNode: public DefenseModelNode
{
public:
	StationModelNode(DefenseModelLayer* defenseModelLayer, Station::Data* data, QObject* parent = nullptr);

    void dataChanged();
public:
    virtual bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;
    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, bool onModel) override;
    virtual void updateColors() override;

protected:
	void setSelectionMode(SelectionMode sm) override;
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
//    MapController* mMapController{nullptr};
//    StationInfo mInformation;
//    UIHandle* mUIHandle;
//    QQmlEngine *mQmlEngine;
    DefenseModelLayer* mDefenseModelLayer;
    Station::Data* mData;
    osg::ref_ptr<Circle> mRangeCircle;
    osg::ref_ptr<AnnotatedNode> mVisiblePolygon;

    static osg::ref_ptr<osg::Node> mNode3DRef;
	static constexpr int LABEL_IMAGE_WIDTH = 180;
	static constexpr int LABEL_IMAGE_HEIGHT = 88;
    QImage *mRenderTargetImage{nullptr};
    osg::ref_ptr<osg::Image> mLabelImage{nullptr};


    osg::ref_ptr<osg::Switch> mNode2DActive;
    osg::ref_ptr<osg::Switch> mNode2DDeactive;

	osg::ref_ptr<Circle> mBackCircleNode = nullptr;
//    osgEarth::Symbology::Style mCircleStyleActive;
//    osgEarth::Symbology::Style mCircleStyleDeactive;
    StationInformtion *mStationInformation = nullptr;
};


#endif // STATION_H

#ifndef SYSTEM_H
#define SYSTEM_H

#include "mapItem.h"
#include "defenseModelNode.h"
#include "systemInfoItem.h"
#include "circle.h"
#include "sphereNode.h"
#include "polygon.h"
#include "aircraftModelNode.h"
#include "lineNode.h"
#include "truck.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <QImage>
#include <QPainter>
#include <osgEarthAnnotation/RectangleNode>

namespace System {
struct Data;
}
class SystemModelNode: public DefenseModelNode
{
public:
    SystemModelNode(DefenseModelLayer* defenseModelLayer, System::Data* systemData, QObject* parent = nullptr);

    void informationChanged();
//    SystemInfo getInformation() const;
    void statusInfoChanged();
    void combatInfoChanged();
    void assignmentChanged();
//    SystemCombatInfo getSystemCombatInfo() const;

//    void addAssignment(int tn, AircraftModelNode *assignModelNode);
//    AircraftModelNode *getAssignment(int tn) const;
//    void acceptAssignment(int tn, bool value);
//    void removeAssignment(int tn);
//    void clearAssignments(int exceptTN = -1);
//    QMap<int, AircraftModelNode *> getAssignments() const;
    System::Data* getData() const;

public slots:
public:
    virtual bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;
    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, bool onModel) override;
    virtual void updateColors() override;

protected:
	void setSelectionMode(SelectionMode sm) override;
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
    void showInfoWidget();
    void updateOrCreateLabelImage();
private:
//    struct Assignment{
//        AircraftModelNode *mModelNode{nullptr};
//        osg::ref_ptr<LineNode> mLine;
//        Assignment(MapController *mapControler);
//        void accept();
//        void updateLine(const osgEarth::GeoPoint &position);
//    };
private:
//    MapController* mMapController{nullptr};
//    SystemInfo mInformation;
//    SystemStatusInfo mStatusInfo;
//    SystemCombatInfo mCombatInfo;
//    UIHandle* mUIHandle;
//    QQmlEngine *mQmlEngine;
    DefenseModelLayer* mDefenseModelLayer;
	System::Data* mData;

    osg::ref_ptr<Circle> mRangeCircle;
    osg::ref_ptr<SphereNode> mMezSphere;
    osg::ref_ptr<Polygon> mWezPolygon;

    osg::ref_ptr<class TruckS> mTruckS;
    osg::ref_ptr<class TruckF> mTruckF;
    osg::ref_ptr<class TruckL> mTruckL;
    Rocket* mFiredRocket{nullptr};

//    QMap<int, Assignment*> mAssignmentMap;
    SystemInfoItem *mSystemInfoItem{nullptr};

    osg::ref_ptr<AircraftModelNode> mTargetModelNode{nullptr};
private:
    QImage                  *mRenderTargetImage = nullptr;
	osg::ref_ptr<osg::Image> mStatusImage = nullptr;
	static constexpr int LABEL_IMAGE_WIDTH = 180;
	static constexpr int LABEL_IMAGE_HEIGHT = 132;

    osg::ref_ptr<osg::LOD> mLOD = nullptr;

    osg::ref_ptr<osg::Switch> mNode2DActive;
    osg::ref_ptr<osg::Switch> mNode2DDeactive;

	osg::ref_ptr<Circle> mBackCircleNode = nullptr;
//    osgEarth::Symbology::Style mRectangleStyleSelected;
//    osgEarth::Symbology::Style mRectangleStyleDeselected;
};

#endif // SYSTEM_H

#ifndef AIRCRAFTMODELNODE_H
#define AIRCRAFTMODELNODE_H

#include "defenseModelNode.h"
#include "aircraftInfoItem.h"
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
class DefenseModelLayer;
namespace Aircraft {
struct Data;
}

class VisualData
{
public:
	struct Image2D{
		osg::ref_ptr<osg::Image> image;
		osg::ref_ptr<osg::Image> imageHover;
	};
public:
	static osg::ref_ptr<osg::Node> get3DNode(AircraftInfo::AircraftType type);
	static Image2D get2DImage(AircraftInfo::AircraftType type, AircraftInfo::Identify identification);
private:
	static QMap<AircraftInfo::AircraftType, osg::ref_ptr<osg::Node>> mNodes3D;
	static QMap<AircraftInfo::AircraftType, QMap<AircraftInfo::Identify, Image2D>> mImages2D;

};

class AircraftModelNode: public DefenseModelNode
{
    Q_OBJECT
public:
	AircraftModelNode(DefenseModelLayer* defenseModelLayer, const Aircraft::Data &data, QObject* parent = nullptr);
	~AircraftModelNode()override;
    void flyTo(osgEarth::GeoPoint posGeo, double heading, double speed);
    void stop() override;
	void dataChanged();
    const Aircraft::Data &getData() const;
//	void select();
protected:
	void setSelectionMode(SelectionMode sm) override;
public:
    bool frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)override;
    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, bool onModel) override;
    virtual void updateColors() override;

private slots:
    void onGotoButtonClicked();
    void onRouteButtonToggled(bool check);
    void onLatestPointsToggled(bool check);
    void onTrackButtonToggled(bool check);
    void onModeChanged(bool is3DView);
    void onContextmenuItemClicked(int index, QString systemName);
private:
	void updateOrCreateNodes();
    void showInfoWidget();
    void addEffect(double emitterDuration);
    void removeEffect();
	void updateOrCreateStatusImage();
private:
    DefenseModelLayer* mDefenseModelLayer{nullptr};
    ModelAnimationPathCallback* mAnimationPathCallback{nullptr};
    osgEarth::Annotation::ModelNode* mTruckModel;

    osg::ref_ptr<osgParticle::SmokeTrailEffect> mSmoke;
    osg::ref_ptr<osgParticle::FireEffect> mFire;

	const Aircraft::Data* mData;
    AircraftInfo::AircraftType mType;
	AircraftInfo::Identify mIdentification;

    bool mIsStop{false};
    bool mIsRoute{false};
    ContextMenu *mCurrentContextMenu = nullptr;

	osg::ref_ptr<LineNode> mRouteLine;
	osg::ref_ptr<LineNode> mLatestPointLine;
	osg::ref_ptr<LineNode> mTempLine;

	AircraftInfoItem *mAircraftInfoItem{nullptr};

	static constexpr int LABEL_IMAGE_WIDTH = 180;
	static constexpr int LABEL_IMAGE_HEIGHT = 132;
	QImage *mRenderStatusImage{nullptr};
	osg::ref_ptr<osg::Image> mStatusImage{nullptr};


    double mCurrentHeading{500};
    osgEarth::GeoPoint mCurrentFlyPoint;

    osg::ref_ptr<osg::PositionAttitudeTransform> mPat2D;

    double mAutoScaleDefaultValue = 2.5;
    double mAutoScaleMinValue = 1;
    double mAutoScaleMaxValue = 500;

	osg::ref_ptr<osg::Image>    mImage                = nullptr;
	osg::ref_ptr<osg::Image>    mImageHover           = nullptr;
	bool mIsTrack{false};
};

#endif // AIRCRAFTMODELNODE_H

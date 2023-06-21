#include "systemModelNode.h"
#include "polygon.h"
#include "systemDataManager.h"

#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Depth>
#include <osg/Material>
#include "defenseModelNodeAutoScaler.h"
#include "truckf.h"
#include "trucks.h"
#include "truckl.h"
#include "defenseModelLayer.h"
#include <QtMath>
#include <osgEarthAnnotation/CircleNode>
#include <osg/AutoTransform>

const float RANGE3D = 600;//std::numeric_limits<float>::max();;

SystemModelNode::SystemModelNode(DefenseModelLayer* defenseModelLayer, System::Data* systemData, QObject* parent)
    :DefenseModelNode(defenseModelLayer->mapItem(), parent),
	  mDefenseModelLayer(defenseModelLayer),
	  mData(systemData)
{
    mIs3D = mDefenseModelLayer->mapItem()->getMode();
	//--create root node--------------------------------------------------------------------------
	mRootNode = new osg::LOD;

	osgEarth::Symbology::Style  rootStyle;
	rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);

	this->setCullingActive(false);
	this->addCullCallback(new DefenseModelNodeAutoScaler(3.5, 1, 600));


	setStyle(rootStyle);

	osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
	geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);

	static bool bFirst = true;
	static osg::ref_ptr<osg::Image> mainImage;
	static osg::ref_ptr<osg::Image> systemImageActive;
	static osg::ref_ptr<osg::Image> systemImageActiveHovered;
	static osg::ref_ptr<osg::Image> systemImageDeactive;
	static osg::ref_ptr<osg::Image> systemImageDeactiveHovered;

	if (bFirst) {
		mainImage = osgDB::readImageFile("../data/models/system/truck.png");

		systemImageActive = createColoredImage(mainImage, osgEarth::Color(0.2f, 0.8f, 0.2f, 1.0f));
		systemImageActiveHovered = createDarkerImage(systemImageActive, 0.5f);
		systemImageDeactive = createColoredImage(mainImage, osgEarth::Color(0.8f, 0.2f, 0.2f, 1.0f));
		systemImageDeactiveHovered = createDarkerImage(systemImageDeactive, 0.5f);


		if(systemImageActive)
			systemImageActive->scaleImage(100, 100, systemImageActive->r());
		if(systemImageActiveHovered)
			systemImageActiveHovered->scaleImage(100, 100, systemImageActiveHovered->r());
		if(systemImageDeactive)
			systemImageDeactive->scaleImage(100, 100, systemImageDeactive->r());
		if(systemImageDeactiveHovered)
			systemImageDeactiveHovered->scaleImage(100, 100, systemImageDeactiveHovered->r());


		bFirst = false;
	}



	osg::ref_ptr<osg::Geometry> systemImageDrawableActive = osgEarth::Annotation::AnnotationUtils::createImageGeometry(systemImageActive, osg::Vec2s(0,0), 0, 0, 0.3);
	osg::ref_ptr<osg::Geode>  systemGeodeActive = new osg::Geode();
	systemGeodeActive->setStateSet(geodeStateSet);
	systemGeodeActive->addDrawable(systemImageDrawableActive);


	osg::ref_ptr<osg::Geometry> systemImageDrawableActiveHovered = osgEarth::Annotation::AnnotationUtils::createImageGeometry(systemImageActiveHovered, osg::Vec2s(0,0), 0, 0, 0.3);
	osg::ref_ptr<osg::Geode>  systemGeodeActiveHovered = new osg::Geode();
	systemGeodeActiveHovered->setStateSet(geodeStateSet);
	systemGeodeActiveHovered->addDrawable(systemImageDrawableActiveHovered);

	osg::ref_ptr<osg::Geometry> systemImageDrawableDeactive = osgEarth::Annotation::AnnotationUtils::createImageGeometry(systemImageDeactive, osg::Vec2s(0,0), 0, 0, 0.3);
	osg::ref_ptr<osg::Geode>  systemGeodeDeactive = new osg::Geode();
	systemGeodeDeactive->setStateSet(geodeStateSet);
	systemGeodeDeactive->addDrawable(systemImageDrawableDeactive);


	osg::ref_ptr<osg::Geometry> systemImageDrawableDeactiveHovered = osgEarth::Annotation::AnnotationUtils::createImageGeometry(systemImageDeactiveHovered, osg::Vec2s(0,0), 0, 0, 0.3);
	osg::ref_ptr<osg::Geode>  systemGeodeDeactiveHovered = new osg::Geode();
	systemGeodeDeactiveHovered->setStateSet(geodeStateSet);
	systemGeodeDeactiveHovered->addDrawable(systemImageDrawableDeactiveHovered);



	osg::AutoTransform *at = new osg::AutoTransform;

	mNode2DActive = new osg::Switch;
	mNode2DActive->addChild(systemGeodeActive, true);
	mNode2DActive->addChild(systemGeodeActiveHovered, false);

	mNode2DDeactive = new osg::Switch;
	mNode2DDeactive->addChild(systemGeodeDeactive, true);
	mNode2DDeactive->addChild(systemGeodeDeactiveHovered, false);




	mNode2D = new osg::Switch;
	mNode2D->addChild(mNode2DActive, true);
	mNode2D->addChild(mNode2DDeactive, false);

	at->addChild(mNode2D);
	at->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::ROTATE_TO_CAMERA);

    mTruckF = new TruckF(mDefenseModelLayer->mapItem());
	mTruckF->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,2.0,0));
    mTruckS = new TruckS(mDefenseModelLayer->mapItem());
	mTruckS->stopSearch();
	mTruckS->getPositionAttitudeTransform()->setPosition(osg::Vec3d(-4.0 * std::sin(qDegreesToRadians(60.0)), -4.0 * std::cos(qDegreesToRadians(60.0)),0));

    mTruckL = new TruckL(mDefenseModelLayer->mapItem());
	mTruckL->getPositionAttitudeTransform()->setPosition(osg::Vec3d(4.0 * std::sin(qDegreesToRadians(60.0)), -4.0 * std::cos(qDegreesToRadians(60.0)),0));

	mNode3D = new Group;
	mNode3D->addChild(mTruckL);
	mNode3D->addChild(mTruckS);
	mNode3D->addChild(mTruckF);

	//    osgEarth::Symbology::Style labelStyle;
	//    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	//    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
	//    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;

	updateOrCreateLabelImage();
	mStatusNode = new osgEarth::Annotation::PlaceNode();
	mStatusNode->setIconImage(mStatusImage);

	getGeoTransform()->addChild(mStatusNode);
	mStatusNode->setNodeMask(false);
	mStatusNode->setPriority(10);


	if(mIs3D)
	{
		mRootNode->addChild(mNode3D, 0, RANGE3D);
		mRootNode->addChild(at, RANGE3D, std::numeric_limits<float>::max());
	}
	else
	{
		mRootNode->addChild(mNode3D, 0, 0);
		mRootNode->addChild(at, 0, std::numeric_limits<float>::max());
	}
    mBackCircleNode = new Circle();
	mBackCircleNode->setRadius(6.5);
	//	mBackCircleNode->setClamp(osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN);
	mBackCircleNode->setColor(osgEarth::Color(0.2f, 0.2f, 0.2f, 0.05f));
	mBackCircleNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.08));
	//    mRectangleNode = new osgEarth::Annotation::RectangleNode();
	//    mRectangleNode->setSize(16, 16);

	//    mRectangleStyleSelected.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(0.2f, 0.2f, 0.2f, 1.0f);
	//    mRectangleStyleSelected.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
	//    mRectangleStyleSelected.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_SCENE;
	//    mRectangleStyleSelected.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->binding() = osgEarth::Symbology::AltitudeSymbol::BINDING_CENTROID;

	//    mRectangleStyleDeselected.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(0.4f, 0.4f, 0.4f, 1.0f);
	//    mRectangleStyleDeselected.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
	//    mRectangleStyleDeselected.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_SCENE;
	//    mRectangleStyleDeselected.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->binding() = osgEarth::Symbology::AltitudeSymbol::BINDING_CENTROID;

	//    mRectangleNode->setStyle(mRectangleStyleDeselected);
	//    mRectangleNode->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0,0,0.05));
	//    mRectangleNode->getOrCreateStateSet()->setAttributeAndModes( new osg::Depth(osg::Depth::LEQUAL,0,1,false), 1);

	mNode3D->addChild(mBackCircleNode);


    //map mode changed-----------------------------------------------------------------------
    connect(mDefenseModelLayer->mapItem(), &MapItem::modeChanged, this, &SystemModelNode::onModeChanged);
    //--create shapes-----------------------------------------------------------------------------
    mRangeCircle = new Circle();
    mRangeCircle->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.4f));
	mRangeCircle->setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping::CLAMP_TO_TERRAIN);

	mMezSphere = new SphereNode();
	mMezSphere->setColor(osg::Vec4(1.0, 1.0, 0.0, 0.3f));
	mMezSphere->setSphereShape(SphereNode::SphereShape::SphereTopHalf);

    mWezPolygon = new Polygon(mDefenseModelLayer->mapItem());
    mWezPolygon->setLineColor(osg::Vec4(0.0, 1.0, 0.0, 0.3f));
    mWezPolygon->setFillColor(osg::Vec4(0.0, 1.0, 0.0, 0.3f));

	if (!mSystemInfoItem) {
        mSystemInfoItem = new SystemInfoItem(mDefenseModelLayer->qmlEngine(), mDefenseModelLayer->uiHandle(), mData, this);
		connect(mSystemInfoItem->getInfo(), &SystemInfoModel::gotoButtonClicked, this, &SystemModelNode::onGotoButtonClicked);
		connect(mSystemInfoItem->getInfo(), &SystemInfoModel::rangeButtonClicked, this, &SystemModelNode::onRangeButtonToggled);
		connect(mSystemInfoItem->getInfo(), &SystemInfoModel::wezButtonClicked, this, &SystemModelNode::onWezButtonToggled);
		connect(mSystemInfoItem->getInfo(), &SystemInfoModel::mezButtonClicked, this, &SystemModelNode::onMezButtonToggled);
		connect(mSystemInfoItem->getInfo(), &SystemInfoModel::activeButtonToggled, this, &SystemModelNode::onActiveButtonToggled);
	}

//	mModelColor = osgEarth::Color(0.2f, 0.8f, 0.2f, 1.0f);
//	updateColors();
}

void SystemModelNode::informationChanged()
{
	//    if (mSystemInfoItem)
	//        mSystemInfoItem->setInfo(info);
	//    mInformation = info;
	updateOrCreateLabelImage();
	mStatusNode->setStyle(mStatusNode->getStyle());
}

//SystemInfo SystemModelNode::getInformation() const
//{
//    return mInformation;
//}

void SystemModelNode::combatInfoChanged()
{
	auto combatInfo = mData->information->systemCombatInfo;
	if (mSystemInfoItem)
		mSystemInfoItem->setInfo(mData);

	switch (combatInfo.Phase) {
	case SystemCombatInfo::Search:
		searchPhase();
		break;
	case SystemCombatInfo::Lock:
		lockPhase(combatInfo.TN);
		break;
	case SystemCombatInfo::Fire:
		firePhase(combatInfo.TN);
		break;
	case SystemCombatInfo::Kill:
		killPhase(combatInfo.TN);
		break;
	case SystemCombatInfo::NoKill:
		noKillPhase(combatInfo.TN);
		break;
	}
	updateOrCreateLabelImage();
	mStatusNode->setStyle(mStatusNode->getStyle());
}

void SystemModelNode::assignmentChanged()
{
	updateOrCreateLabelImage();
	mStatusNode->setStyle(mStatusNode->getStyle());
}

System::Data *SystemModelNode::getData() const
{
	return mData;
}

//SystemCombatInfo SystemModelNode::getSystemCombatInfo() const
//{
//    return mCombatInfo;
//}

void SystemModelNode::statusInfoChanged()
{
	auto systemStatusInfo = mData->information->systemStatusInfo;
	if (mSystemInfoItem)
		mSystemInfoItem->setInfo(mData);
	mNode2D->setValue(0, systemStatusInfo.RadarSearchStatus == SystemStatusInfo::S);
	mNode2D->setValue(1, systemStatusInfo.RadarSearchStatus != SystemStatusInfo::S);

	//    mRectangleNode->setStyle(systemStatusInfo.RadarSearchStatus == SystemStatusInfo::S ? mRectangleStyleActive : mRectangleStyleDeactive);
	//mStatusInfo = systemStatusInfo;
	updateOrCreateLabelImage();
	mStatusNode->setStyle(mStatusNode->getStyle());

	updateColors();
}

//void SystemModelNode::addAssignment(int tn, AircraftModelNode *assignModelNode)
//{
//    if(!assignModelNode)
//        return;
//    if(!mAssignmentMap.contains(tn))
//    {
//        Assignment* assignmentModel = new  Assignment(mapItem());
//        assignmentModel->mModelNode = assignModelNode;
//        mAssignmentMap[tn] = assignmentModel;
//        mSystemInfoItem->addAssignment(tn, assignModelNode);
//        mapItem()->addNodeToLayer(assignmentModel->mLine, SYSTEMS_LAYER_NAME);
//    }
//    updateOrCreateLabelImage();
//    mLabelNode->setStyle(mLabelNode->getStyle());
//}

//AircraftModelNode *SystemModelNode::getAssignment(int tn) const
//{
//    if(!mAssignmentMap.contains(tn))
//        return mAssignmentMap[tn]->mModelNode;
//    return nullptr;
//}

//void SystemModelNode::acceptAssignment(int tn, bool value)
//{
//    if(mAssignmentMap.contains(tn))
//    {
//        if(value)
//            mAssignmentMap[tn]->accept();
//        else {
//            removeAssignment(tn);
//        }
//    }
//}

//void SystemModelNode::removeAssignment(int tn)
//{
//    if(mAssignmentMap.contains(tn))
//    {
//        mapItem()->removeNodeFromLayer(mAssignmentMap[tn]->mLine, SYSTEMS_LAYER_NAME);
//        mAssignmentMap.remove(tn);
//        mSystemInfoItem->removeAssignment(tn);
//    }
//    updateOrCreateLabelImage();
//    mLabelNode->setStyle(mLabelNode->getStyle());
//}

//void SystemModelNode::clearAssignments(int exceptTN)
//{
//    for(auto tn: mAssignmentMap.keys())
//    {
//        if(tn != exceptTN)
//        {
//            removeAssignment(tn);
//            mSystemInfoItem->removeAssignment(tn);
//        }
//    }
//}

//QMap<int, AircraftModelNode *> SystemModelNode::getAssignments() const
//{
//    QMap<int, AircraftModelNode *> assignModelNods;
//    for(auto key: mAssignmentMap.keys())
//        assignModelNods[key] = mAssignmentMap[key]->mModelNode;
//    return assignModelNods;
//}

void SystemModelNode::setSelectionMode(SelectionMode sm)
{
	//    updateColors();
	//select(val);
	DefenseModelNode::setSelectionMode(sm);
	if(sm == SELECTED)
	{
		showInfoWidget();
	}
	else
	{
//		mDefenseModelLayer->mapItem()->untrackNode(getGeoTransform());
		//        onRangeButtonToggled(val);
		//        onWezButtonToggled(val);
		//        onMezButtonToggled(val);
	}
}

bool SystemModelNode::frameEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
	mStatusNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( 0, 0, 0));

	for(auto assinment:mData->assignments)
		assinment->updateLine(getPosition());

	if (mTargetModelNode.valid()) {
		mTruckF->aimTarget(mTargetModelNode->getPosition());
		mTruckL->lockOnTarget(mTargetModelNode->getPosition());
	}
    return false;
}

bool SystemModelNode::mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, bool onModel)
{
    return DefenseModelNode::mousePressEvent(ea, aa, onModel);

	//    if(event->button() == Qt::LeftButton)
	//    {
	//        onLeftButtonClicked(onModel);
	//        if(onModel)
	//            event->accept();
	//    }
}

void SystemModelNode::updateColors()
{

	if (mSelectionMode == SELECTED || mHoverMode == HOVERED) {

		mNode2DActive->setValue(0, false);
		mNode2DActive->setValue(1, true);
		mNode2DDeactive->setValue(0, false);
		mNode2DDeactive->setValue(1, true);


	} else {

		mNode2DActive->setValue(0, true);
		mNode2DActive->setValue(1, false);
		mNode2DDeactive->setValue(0, true);
		mNode2DDeactive->setValue(1, false);

	}

	if (mData->information->systemStatusInfo.RadarSearchStatus == SystemStatusInfo::S) {
		mModelColor = osgEarth::Color(0.2f, 0.8f, 0.2f, 1.0f);
	} else {
		mModelColor = osgEarth::Color(0.8f, 0.2f, 0.2f, 1.0f);
	}

	DefenseModelNode::updateColors();

	//    if(mSelectionMode == SELECTED || mHoverMode == HOVERED) {
	//        mRectangleNode->setStyle(mRectangleStyleSelected);
	//    } else {
	//        mRectangleNode->setStyle(mRectangleStyleDeselected);
	//    }


}

void SystemModelNode::onModeChanged(bool is3DView)
{
	mIs3D = is3DView;
	if(mIs3D)
	{
		mRootNode->setRange(0, 0, RANGE3D);
		mRootNode->setRange(1, RANGE3D, std::numeric_limits<float>::max());
	}
	else
	{
		mRootNode->setRange(0, 0, 0);
		mRootNode->setRange(1,0, std::numeric_limits<float>::max());
	}

	updateColors();
	//select(mIsSelected);
}

void SystemModelNode::onGotoButtonClicked()
{
    mDefenseModelLayer->mapItem()->goToPosition(getPosition(), 200);
}

void SystemModelNode::onRangeButtonToggled(bool check)
{
	if(check)
	{
		mRangeCircle->setPosition(getPosition());
		mRangeCircle->setRadius(osgEarth::Distance(mData->information->systemInfo.ViewRange, osgEarth::Units::METERS));

        mDefenseModelLayer->mapItem()->addNodeToLayer(mRangeCircle, SYSTEMS_LAYER_NAME);
	}
	else
	{
        mDefenseModelLayer->mapItem()->removeNodeFromLayer(mRangeCircle, SYSTEMS_LAYER_NAME);
	}
}

void SystemModelNode::onWezButtonToggled(bool checked)
{
	if(checked)
	{
		mWezPolygon->clearPoints();
		osg::Vec3d worldPosition;
        getPosition().toWorld(worldPosition, mDefenseModelLayer->mapItem()->getMapNode()->getTerrain());
		osgEarth::GeoPoint geoPoint;
		double radius = mData->information->systemInfo.MezRange;

		osg::Vec3d v1 = osg::Vec3d(worldPosition.x() - radius*2/4, worldPosition.y() - radius*2/4, worldPosition.z());
		osg::Vec3d v2 = osg::Vec3d(worldPosition.x() - radius*2/4, worldPosition.y() + radius*2/4, worldPosition.z());
		osg::Vec3d v3 = osg::Vec3d(worldPosition.x() + radius*2/4, worldPosition.y() + radius*2/4, worldPosition.z());
		osg::Vec3d v4 = osg::Vec3d(worldPosition.x() + radius*2/4, worldPosition.y() - radius*2/4, worldPosition.z());

		osgEarth::GeoPoint geoPoint1;
        geoPoint1.fromWorld(mDefenseModelLayer->mapItem()->getMapSRS(), v1);
		geoPoint1.z() = 0;
        geoPoint1.transformZ(osgEarth::AltitudeMode::ALTMODE_RELATIVE, mDefenseModelLayer->mapItem()->getMapNode()->getTerrain());
		osgEarth::GeoPoint geoPoint2;
        geoPoint2.fromWorld(mDefenseModelLayer->mapItem()->getMapSRS(), v2);
		geoPoint2.z() = 0;
        geoPoint2.transformZ(osgEarth::AltitudeMode::ALTMODE_RELATIVE, mDefenseModelLayer->mapItem()->getMapNode()->getTerrain());
		osgEarth::GeoPoint geoPoint3;
        geoPoint3.fromWorld(mDefenseModelLayer->mapItem()->getMapSRS(), v3);
		geoPoint3.z() = 0;
        geoPoint3.transformZ(osgEarth::AltitudeMode::ALTMODE_RELATIVE, mDefenseModelLayer->mapItem()->getMapNode()->getTerrain());
		osgEarth::GeoPoint geoPoint4;
        geoPoint4.fromWorld(mDefenseModelLayer->mapItem()->getMapSRS(), v4);
		geoPoint4.z() = 0;
        geoPoint4.transformZ(osgEarth::AltitudeMode::ALTMODE_RELATIVE, mDefenseModelLayer->mapItem()->getMapNode()->getTerrain());

		mWezPolygon->addPoints(geoPoint1);
		mWezPolygon->addPoints(geoPoint2);
		mWezPolygon->addPoints(geoPoint3);
		mWezPolygon->addPoints(geoPoint4);

		float height = static_cast<float>(radius/3);
		mWezPolygon->setHeight(height);

        mDefenseModelLayer->mapItem()->addNodeToLayer(mWezPolygon, SYSTEMS_LAYER_NAME);

	}
	else {
        mDefenseModelLayer->mapItem()->removeNodeFromLayer(mWezPolygon, SYSTEMS_LAYER_NAME);
	}
}

void SystemModelNode::onMezButtonToggled(bool checked)
{
	if(checked)
	{
		mMezSphere->setPosition(getPosition());
		mMezSphere->setRadius(mData->information->systemInfo.MezRange);
        mDefenseModelLayer->mapItem()->addNodeToLayer(mMezSphere, SYSTEMS_LAYER_NAME);
	}
	else
	{
        mDefenseModelLayer->mapItem()->removeNodeFromLayer(mMezSphere, SYSTEMS_LAYER_NAME);
	}
}

void SystemModelNode::onActiveButtonToggled(bool checked)
{
	mData->information->systemStatusInfo.RadarSearchStatus = (checked ? SystemStatusInfo::S : SystemStatusInfo::US);

	mNode2D->setValue(0, checked);
	mNode2D->setValue(1, !checked);

	//    mRectangleNode->setStyle(checked ? mRectangleStyleActive : mRectangleStyleDeactive);
	updateColors();
}

void SystemModelNode::searchPhase()
{
	for(auto assignment: mData->assignments)
		assignment->line->setColor(osgEarth::Color::Yellow);

	mTruckS->startSearch();
}

void SystemModelNode::lockPhase(int tn)
{
	auto index = mData->findAssignment(tn);

	if(index != -1)
	{
		auto assignment = mData->assignments.at(index);
		mTargetModelNode = assignment->modelNode;
		assignment->line->setColor(osgEarth::Color::Orange);

		mTruckL->lockOnTarget(mTargetModelNode->getPosition());
		mTruckF->aimTarget(mTargetModelNode->getPosition());

		//        //remove other assigned models
		//        for(auto assignModel:mAssignmentMap)
		//        {
		//            auto aircraftModelNode = static_cast<AircraftModelNode*>(assignModel->mModelNode);
		//            if(aircraftModelNode && aircraftModelNode->getInformation().TN != tn)
		//                removeAssignedModelNode(aircraftModelNode->getInformation().TN);
		//        }
	}
}

void SystemModelNode::firePhase(int tn)
{
	auto index = mData->findAssignment(tn);

	if(index != -1)
	{
		auto assignment = mData->assignments.at(index);
		mTargetModelNode = assignment->modelNode;
		assignment->line->setColor(osgEarth::Color::Red);
		mFiredRocket = mTruckF->getActiveRocket();
		if(mFiredRocket)
		{
			mFiredRocket->setAutoScale();
			mTruckF->shoot(mTargetModelNode->getPosition().vec3d(), 2000);//1000 m/s
            mDefenseModelLayer->mapItem()->setTrackNode(mFiredRocket->getGeoTransform(), 400);
		}
	}
}

void SystemModelNode::killPhase(int tn)
{
	auto index = mData->findAssignment(tn);

	if(index != -1)
	{
		auto assignment = mData->assignments.at(index);
        mDefenseModelLayer->mapItem()->untrack();
		assignment->line->setColor(osgEarth::Color::Black);
		assignment->modelNode->collision();

		if(mFiredRocket)
			mFiredRocket->stop();

		//        removeAssignment(tn);
	}
}

void SystemModelNode::noKillPhase(int tn)
{
	auto index = mData->findAssignment(tn);

	if(index != -1)
	{
		auto assignment = mData->assignments.at(index);
        mDefenseModelLayer->mapItem()->untrack();
		assignment->line->setColor(osgEarth::Color::Brown);
		if(mFiredRocket)
			mFiredRocket->stop();
		//        removeAssignment(tn);
	}
}

void SystemModelNode::showInfoWidget()
{
	//    if (!mSystemInfoItem) {
	//        mSystemInfoItem = new SystemInfoItem(mQmlEngine, mUIHandle, mInformation, mStatusInfo, mCombatInfo, this);
	//        connect(mSystemInfoItem->getInfo(), &SystemInfoModel::gotoButtonClicked, this, &SystemModelNode::onGotoButtonClicked);
	//        connect(mSystemInfoItem->getInfo(), &SystemInfoModel::rangeButtonClicked, this, &SystemModelNode::onRangeButtonToggled);
	//        connect(mSystemInfoItem->getInfo(), &SystemInfoModel::wezButtonClicked, this, &SystemModelNode::onWezButtonToggled);
	//        connect(mSystemInfoItem->getInfo(), &SystemInfoModel::mezButtonClicked, this, &SystemModelNode::onMezButtonToggled);
	//        connect(mSystemInfoItem->getInfo(), &SystemInfoModel::activeButtonToggled, this, &SystemModelNode::onActiveButtonToggled);
	mSystemInfoItem->setInfo(mData);
	mSystemInfoItem->show();
}

void SystemModelNode::updateOrCreateLabelImage()
{
	//    qDebug() << ">>> : " << mAssignmentMap.keys().count();
	int txtLeftPos = 5;
	int txtTopPos = 0;
	//	int txtWidth = 30;
	int txtHeight = 22;

	int missleImageWidth = static_cast<int>((LABEL_IMAGE_WIDTH - 2*txtLeftPos) / 9.0);
	int missleImageheight = static_cast<int>(4.0/3.0*missleImageWidth);

	int height = LABEL_IMAGE_HEIGHT + (mData->assignments.count()) * txtHeight + missleImageheight;
	if (mData->information->systemStatusInfo.Operational == SystemStatusInfo::OP2)
		height += missleImageheight;

	if (!mRenderTargetImage) {
		mRenderTargetImage = new QImage(
					LABEL_IMAGE_WIDTH,
					height,
					QImage::Format_RGBA8888
					);
	} else {
		mRenderTargetImage->~QImage();
		mRenderTargetImage = new(mRenderTargetImage) QImage(
					LABEL_IMAGE_WIDTH,
					height,
					QImage::Format_RGBA8888
					);

	}

	if (!mStatusImage) {
		mStatusImage = new osg::Image;
	}

	{
		mRenderTargetImage->fill(QColor(Qt::transparent));
		QPainter painter(mRenderTargetImage);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);


		static const QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.3f)));

		static const QFont textFont("SourceSansPro", 10, QFont::Normal);
		static const QFont textFontBold("SourceSansPro", 10, QFont::Bold);
		QPen  textPen(QColor(255, 255, 255));
		static QPen  textPenYellow(QColor(204, 204, 51, 255));

		painter.setPen(Qt::NoPen);
		painter.setBrush(backgroundBrush);

		painter.drawRoundedRect(
					mRenderTargetImage->rect(),
					8,8);
		painter.setBrush(QBrush(QColor(26, 77, 46, int(255 * 0.2f))));
		painter.drawRoundedRect(
					QRect(0, 0, LABEL_IMAGE_WIDTH, txtHeight),
					8,8);
		//--------------------------------------------------------------
		static const QPen linePen(QColor(255, 255, 255),
								  1,
								  Qt::PenStyle::DashLine
								  );

		painter.setPen(linePen);
		painter.setBrush(Qt::NoBrush);
		painter.drawLine(0, txtHeight, LABEL_IMAGE_WIDTH, txtHeight);
		painter.drawLine(LABEL_IMAGE_WIDTH/2, 0, LABEL_IMAGE_WIDTH/2, txtHeight);


		painter.setPen(textPenYellow);
		painter.setFont(textFontBold);
		painter.drawText(QRect(0, 0, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignCenter,
						 mData->information->systemInfo.Name);
		painter.drawText(QRect(LABEL_IMAGE_WIDTH/2, 0, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignCenter,
						 QString::number(mData->information->systemInfo.Number));
		//-------------------------------------------------------------
		txtTopPos += txtHeight;
		painter.setPen(textPen);
		painter.setFont(textFont);

		painter.drawText(QRect(txtLeftPos, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "Type:");
		painter.drawText(QRect(txtLeftPos + LABEL_IMAGE_WIDTH/2, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 mData->information->systemInfo.Type);

		txtTopPos += txtHeight;
		painter.drawText(QRect(txtLeftPos, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "BCC:");

		if(mData->information->systemStatusInfo.BCCStatus == SystemStatusInfo::S)
			painter.setPen(QColor(0, 255, 0));
		else
			painter.setPen(QColor(255, 0, 0));

		painter.drawText(QRect(txtLeftPos + LABEL_IMAGE_WIDTH/2, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 mData->information->systemStatusInfo.radarStatusToString(mData->information->systemStatusInfo.BCCStatus));

		txtTopPos += txtHeight;
		painter.setPen(textPen);
		painter.drawText(QRect(txtLeftPos, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "Radar:");
		if(mData->information->systemStatusInfo.RadarSearchStatus == SystemStatusInfo::S)
			painter.setPen(QColor(0, 255, 0));
		else
			painter.setPen(QColor(255, 0, 0));

		painter.drawText(QRect(txtLeftPos + LABEL_IMAGE_WIDTH/2, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 mData->information->systemStatusInfo.radarStatusToString(mData->information->systemStatusInfo.RadarSearchStatus));


		txtTopPos += txtHeight;
		painter.setPen(textPen);
		painter.drawText(QRect(txtLeftPos, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "Phase:");

		if(mData->assignments.count()<=0)
			painter.drawText(QRect(txtLeftPos + LABEL_IMAGE_WIDTH/2, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
							 Qt::AlignLeft | Qt::AlignVCenter,
							 "---");
		else{
			painter.setPen(mData->information->systemCombatInfo.phaseToColor());
			painter.drawText(QRect(txtLeftPos + LABEL_IMAGE_WIDTH/2, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
							 Qt::AlignLeft | Qt::AlignVCenter,
							 QString(mData->information->systemCombatInfo.phaseToString()));
		}

		//---------------------------------------------------------
		txtTopPos += txtHeight;
		painter.setPen(linePen);
		painter.setBrush(Qt::NoBrush);
		painter.drawLine(0, txtTopPos, LABEL_IMAGE_WIDTH, txtTopPos);

		painter.setPen(textPen);
		painter.drawText(QRect(txtLeftPos, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "Assignments:");


		const QFontMetrics fm(textFont);
		for(const auto& assignment: mData->assignments){
			txtTopPos += txtHeight;
			painter.drawText(QRect(txtLeftPos, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
							 Qt::AlignLeft | Qt::AlignVCenter,
							 assignment->info->aircraftTypeToString());
			painter.drawText(QRect(txtLeftPos + LABEL_IMAGE_WIDTH/2, txtTopPos, LABEL_IMAGE_WIDTH/2, txtHeight),
							 Qt::AlignLeft | Qt::AlignVCenter,
							 QString::number(assignment->info->TN));
		}
		//        int n = 0;
		//        while (n < mData->assignments.count()) {

		//            int indent = 0;
		//            for (int llidx = 0; llidx < 3; llidx++)// two elements per line
		//            {

		//                if (n >= mData->assignments.count())
		//                    break;

		//                const auto val = mData->assignments[n];

		//                const QString ss = (llidx == 0 ? QStringLiteral("") : QStringLiteral(", "))
		//                        + QString::number(val->info->TN);

		//                textPen.setColor(QColor(255,255,255));
		//                painter.setPen(textPen);

		//                painter.drawText(QRect(10 + indent, h, LABEL_IMAGE_WIDTH, 30),
		//                                 Qt::AlignLeft | Qt::AlignVCenter,
		//                                 ss);
		//                indent += fm.boundingRect(ss).width();

		//                n++;
		//            }

		//            h += 30;
		//        }


		//------------------------------------------------------------
		txtTopPos += txtHeight;
		//        h = height - 60 - (mData->information->systemStatusInfo.Operational == SystemStatusInfo::OP2 ? 50 : 0);
		painter.setPen(linePen);
		painter.setBrush(Qt::NoBrush);
		painter.drawLine(0, txtTopPos, LABEL_IMAGE_WIDTH, txtTopPos);

		//        h+=10;

		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::white);
		static const QImage missleRedImage(":/resources/bullet_red.png");
		static const QImage missleGreenImage(":/resources/bullet_green.png");

		if (mData->information->systemStatusInfo.Operational == SystemStatusInfo::NOP) {
			for (int i = 0; i < 9; i++) {
				painter.drawImage(
							QRect(txtLeftPos + missleImageWidth * i, txtTopPos, missleImageWidth, missleImageheight),
							missleRedImage,
							missleRedImage.rect()
							);

			}

		} else {
			for (int i = 0; i < 9; i++) {
				if(i < mData->information->systemStatusInfo.MissileCount) {
					painter.drawImage(
								QRect(txtLeftPos + missleImageWidth * i, txtTopPos, missleImageWidth, missleImageheight),
								missleGreenImage,
								missleRedImage.rect()
								);
				} else {
					painter.drawImage(
								QRect(txtLeftPos + missleImageWidth * i, txtTopPos, missleImageWidth, missleImageheight),
								missleRedImage,
								missleRedImage.rect()
								);
				}
			}

			if (mData->information->systemStatusInfo.Operational == SystemStatusInfo::OP2) {
				txtTopPos += missleImageheight;
				for (int i = 9; i < 18; i++) {
					if(i < mData->information->systemStatusInfo.MissileCount) {
						painter.drawImage(
									QRect(txtLeftPos + missleImageWidth *  (i-9), txtTopPos, missleImageWidth, missleImageheight),
									missleGreenImage,
									missleRedImage.rect()
									);
					} else {
						painter.drawImage(
									QRect(txtLeftPos + missleImageWidth * (i-9), txtTopPos, missleImageWidth, missleImageheight),
									missleRedImage,
									missleRedImage.rect()
									);

					}
				}

			}
		}

	}
	*mRenderTargetImage = mRenderTargetImage->mirrored(false, true);

	mStatusImage->setImage(LABEL_IMAGE_WIDTH,
						   height,
						   1,
						   GL_RGBA,
						   GL_RGBA,
						   GL_UNSIGNED_BYTE,
						   mRenderTargetImage->bits(),
						   osg::Image::AllocationMode::NO_DELETE);
}



//SystemModelNode::Assignment::Assignment(MapItem *mapControler)
//{
//    mLine = new LineNode(mapControler);
//    mLine->setPointVisible(true);
//    mLine->setColor(osgEarth::Color::White);
//    mLine->setPointColor(osgEarth::Color::Olive);
//    mLine->setWidth(1);
//    mLine->setPointWidth(5);
//    mLine->setTessellation(15);
//    mLine->setShowBearing(true);
//}

//void SystemModelNode::Assignment::accept()
//{
//    mLine->setColor(osgEarth::Color::Olive);
//    mLine->setPointVisible(false);
//    mLine->setWidth(5);
//}

//void SystemModelNode::Assignment::updateLine(const osgEarth::GeoPoint& position)
//{
//    mLine->clear();
//    mLine->addPoint(position);
//    mLine->addPoint(mModelNode->getPosition());
//}

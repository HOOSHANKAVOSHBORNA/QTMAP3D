#include "aircraftModelNode.h"
#include "contextMenu.h"
#include "aircraftInfoItem.h"
#include "draw.h"
#include "systemModelNode.h"
#include "defenseModelLayer.h"
#include "aircraftDataManager.h"
#include "systemDataManager.h"

#include <osgEarth/Registry>
#include <osgGA/EventVisitor>
#include <osgGA/EventHandler>
#include <osgViewer/View>
#include <osgEarth/IntersectionPicker>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarthAnnotation/ImageOverlay>
#include <osg/Depth>
#include <osg/Material>
#include<osg/Switch>

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QTimer>
#include <array>
#include "defenseModelNodeAutoScaler.h"


#include "defenseModelLayer.h"

#include <QFontMetrics>

const float RANGE3D = std::numeric_limits<float>::max();//832
const int NUM_LATEST_POINT = 100;

//------------------------------------------------------------------------------------------
QMap<AircraftInfo::AircraftType, osg::ref_ptr<osg::Node>> VisualData::mNodes3D;
QMap<AircraftInfo::AircraftType, QMap<AircraftInfo::Identify, VisualData::Image2D>> VisualData::mImages2D;

osg::ref_ptr<osg::Node> VisualData::get3DNode(AircraftInfo::AircraftType type)
{
	switch (type) {
	case AircraftInfo::Fighter:
		if (!mNodes3D.contains(type))
			mNodes3D[type] = osgDB::readRefNodeFile("../data/models/fighter/fighter.osgb");
		return mNodes3D[type];
	case AircraftInfo::Aircraft:
		if (!mNodes3D.contains(type))
			mNodes3D[type] = osgDB::readRefNodeFile("../data/models/aircraft/boeing-747.osgb");
		return mNodes3D[type];
	case AircraftInfo::Missile:
		if (!mNodes3D.contains(type))
			mNodes3D[type] = osgDB::readRefNodeFile("../data/models/missile/missile.osgb");
		return mNodes3D[type];
	case AircraftInfo::Drone:
		if (!mNodes3D.contains(type))
			mNodes3D[type] = osgDB::readRefNodeFile("../data/models/drone/drone.osgb");
		return mNodes3D[type];
	case AircraftInfo::Helicopter:
		if (!mNodes3D.contains(type))
			mNodes3D[type] = osgDB::readRefNodeFile("../data/models/hellicopter/HellicopterUC.osgb");
		return mNodes3D[type];
	}
	return nullptr;
}

VisualData::Image2D VisualData::get2DImage(AircraftInfo::AircraftType type, AircraftInfo::Identify identification)
{
	switch (type) {
	case AircraftInfo::Fighter:
		if (!mImages2D.contains(type)){
			QMap<AircraftInfo::Identify, Image2D> images;
			auto image = osgDB::readImageFile("../data/models/fighter/fighter.png");
			for (int i = AircraftInfo::Identify::Begin; i <= AircraftInfo::Identify::End; i++){
				AircraftInfo::Identify identy = static_cast<AircraftInfo::Identify>(i);
				images[identy].image = DefenseModelNode::createColoredImage(image,
																			AircraftInfo::qColor2osgEarthColor(AircraftInfo::aircraftColor(identy)));
				images[identy].imageHover = DefenseModelNode::createDarkerImage(images[identy].image, 0.5f);
			}
			mImages2D[type] = images;
		}
		return mImages2D[type][identification];
	case AircraftInfo::Aircraft:
		if (!mImages2D.contains(type)){
			QMap<AircraftInfo::Identify, Image2D> images;
			auto image = osgDB::readImageFile("../data/models/aircraft/aircraft.png");
			for (int i = AircraftInfo::Identify::Begin; i <= AircraftInfo::Identify::End; i++){
				AircraftInfo::Identify identy = static_cast<AircraftInfo::Identify>(i);
				images[identy].image = DefenseModelNode::createColoredImage(image,
																			AircraftInfo::qColor2osgEarthColor(AircraftInfo::aircraftColor(identy)));
				images[identy].imageHover = DefenseModelNode::createDarkerImage(images[identy].image, 0.5f);
			}
			mImages2D[type] = images;
		}
		return mImages2D[type][identification];
	case AircraftInfo::Missile:
		if (!mImages2D.contains(type)){
			QMap<AircraftInfo::Identify, Image2D> images;
			auto image = osgDB::readImageFile("../data/models/missile/missle.png");
			for (int i = AircraftInfo::Identify::Begin; i <= AircraftInfo::Identify::End; i++){
				AircraftInfo::Identify identy = static_cast<AircraftInfo::Identify>(i);
				images[identy].image = DefenseModelNode::createColoredImage(image,
																			AircraftInfo::qColor2osgEarthColor(AircraftInfo::aircraftColor(identy)));
				images[identy].imageHover = DefenseModelNode::createDarkerImage(images[identy].image, 0.5f);
			}
			mImages2D[type] = images;
		}
		return mImages2D[type][identification];
	case AircraftInfo::Drone:
		if (!mImages2D.contains(type)){
			QMap<AircraftInfo::Identify, Image2D> images;
			auto image = osgDB::readImageFile("../data/models/drone/drone.png");
			for (int i = AircraftInfo::Identify::Begin; i <= AircraftInfo::Identify::End; i++){
				AircraftInfo::Identify identy = static_cast<AircraftInfo::Identify>(i);
				images[identy].image = DefenseModelNode::createColoredImage(image,
																			AircraftInfo::qColor2osgEarthColor(AircraftInfo::aircraftColor(identy)));
				images[identy].imageHover = DefenseModelNode::createDarkerImage(images[identy].image, 0.5f);
			}
			mImages2D[type] = images;
		}
		return mImages2D[type][identification];
	case AircraftInfo::Helicopter:
		if (!mImages2D.contains(type)){
			QMap<AircraftInfo::Identify, Image2D> images;
			auto image = osgDB::readImageFile("../data/models/hellicopter/hellicopter.png");
			for (int i = AircraftInfo::Identify::Begin; i <= AircraftInfo::Identify::End; i++){
				AircraftInfo::Identify identy = static_cast<AircraftInfo::Identify>(i);
				images[identy].image = DefenseModelNode::createColoredImage(image,
																			AircraftInfo::qColor2osgEarthColor(AircraftInfo::aircraftColor(identy)));
				images[identy].imageHover = DefenseModelNode::createDarkerImage(images[identy].image, 0.5f);
			}
			mImages2D[type] = images;
		}
		return mImages2D[type][identification];
	}
	return Image2D();
}
//-------------------------------------------------------------------------------------------
AircraftModelNode::AircraftModelNode(DefenseModelLayer *defenseModelLayer, const Aircraft::Data& data, QObject *parent)
	:DefenseModelNode(defenseModelLayer->mMapController, parent)
{
	//--init----------------------------------------------------------------------------------------
	mData = &data;
	mDefenseModelLayer = defenseModelLayer;
	mIs3D = mDefenseModelLayer->mMapController->getMode();
	mType = mData->info.Type;
	mIdentification = mData->info.Identification;
	connect(mDefenseModelLayer->mMapController, &MapController::modeChanged, this, &AircraftModelNode::onModeChanged);

	//----------------------------------------------------------------------------------------------
	this->setCullingActive(false);
	this->addCullCallback(mDefenseModeNodeAutoScaler);

	mRootNode = new osg::LOD;
	osgEarth::Symbology::Style  rootStyle;
	rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
	setStyle(rootStyle);

	mNode3D = new osg::Group;
	mNode2D = new osg::Switch;
	mRootNode->addChild(mNode3D);
	mRootNode->addChild(mNode2D);

	osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
	geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);

//	mImage = osgDB::readImageFile("../data/models/fighter/fighter.png");
//	mImageHover = osgDB::readImageFile("../data/models/fighter/fighter.png");

	updateOrCreateNodes();
	auto imageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(mImage, osg::Vec2s(0,0), 0, 0, 0.4);
	auto geode = new osg::Geode();
	geode->setStateSet(geodeStateSet);
	geode->addDrawable(imageDrawable);

	auto imageDrawableHovered = osgEarth::Annotation::AnnotationUtils::createImageGeometry(mImageHover, osg::Vec2s(0,0), 0, 0, 0.4);
	auto geodeHovered = new osg::Geode();
	geodeHovered->setStateSet(geodeStateSet);
	geodeHovered->addDrawable(imageDrawableHovered);
	mNode2D->addChild(geode, true);
	mNode2D->addChild(geodeHovered, false);

	mPat2D = new osg::PositionAttitudeTransform;
	mPat2D->setAttitude(osg::Quat(osg::inDegrees(45.0), -osg::Z_AXIS));

	mPat2D->addChild(mNode2D);

	osg::AutoTransform *at = new osg::AutoTransform;

	at->addChild(mPat2D);
	at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_CAMERA);
	//--status image node--------------------------------------------------------------------
	updateOrCreateStatusImage();
	mStatusNode = new osgEarth::Annotation::PlaceNode();
	mStatusNode->setIconImage(mStatusImage);
	mStatusNode->setDynamic(true);
	mStatusNode->setPriority(10);


	getGeoTransform()->addChild(mStatusNode);
	mStatusNode->setNodeMask(false);


	//--fire------------------------------------------------------------------------
	float radius = getBound().radius();
	float scale = 3;

	mFire = new osgParticle::FireEffect(osg::Vec3f(0, 2*radius,0),scale,100.0);
	mFire->setUseLocalParticleSystem(false);

	mSmoke = new osgParticle::SmokeTrailEffect(osg::Vec3f(0, 2*radius,0),scale/3,100.0);
	mSmoke->setUseLocalParticleSystem(false);

	//--lines-----------------------------------------------------------------------
	mRouteLine = new LineNode(defenseModelLayer->mMapController);
	mRouteLine->setPointVisible(false);
	mRouteLine->setTessellation(10);
	mRouteLine->setColor(osgEarth::Color::Purple);
	mRouteLine->setWidth(5);
	mRouteLine->setShowLenght(false);

	mLatestPointLine = new LineNode(defenseModelLayer->mMapController);
	mLatestPointLine->setPointVisible(true);
	mLatestPointLine->setPointColor(osgEarth::Color::Blue);
	mLatestPointLine->setColor(osgEarth::Color::Purple);
	mLatestPointLine->setWidth(5);
	mLatestPointLine->setPointWidth(15);
	mLatestPointLine->setShowLenght(false);
	mLatestPointLine->setSmooth(true);

	mTempLine = new LineNode(defenseModelLayer->mMapController);
	mTempLine->setPointVisible(false);
	mTempLine->setColor(osgEarth::Color::Purple);
	mTempLine->setWidth(5);
	mTempLine->setShowLenght(false);
}

void AircraftModelNode::flyTo(osgEarth::GeoPoint posGeo, double heading, double /*speed*/)
{

	posGeo.transformInPlace(mDefenseModelLayer->mMapController->getMapSRS());
	osg::Vec3d currentPosW;
	getPosition().toWorld(currentPosW);

	osg::Vec3d posW;
	posGeo.toWorld(posW);

	osg::Vec3d diffW = posW - currentPosW;
	osg::Matrixd currentPoslocalTransfer;
	getPosition().createWorldToLocal(currentPoslocalTransfer);
	osg::Vec3d diffLocal = diffW * osg::Matrixd::rotate(currentPoslocalTransfer.getRotate());
	diffLocal.normalize();

	osg::Quat diffRotate;
	diffRotate.makeRotate(osg::Vec3d(0, 1, 0), diffLocal);

	osg::Vec3d northVec(0, 1, 0);//in local
	osg::Vec3d headingVecLocal = northVec * osg::Matrixd::rotate(osg::inDegrees(-heading), osg::Z_AXIS);
	headingVecLocal.normalize();

	headingVecLocal.z() = diffLocal.z();

	osg::Matrixd posLocalToWorld;
	posGeo.createLocalToWorld(posLocalToWorld);


	osg::Quat headingRotate;
	headingRotate.makeRotate(osg::Vec3d(0, 1, 0), headingVecLocal);


	osg::AnimationPath* path = new osg::AnimationPath();
	path->setLoopMode(osg::AnimationPath::NO_LOOPING);

	path->insert(0, osg::AnimationPath::ControlPoint(currentPosW,getPositionAttitudeTransform()->getAttitude(),getScale()));
	path->insert(0.001,osg::AnimationPath::ControlPoint(posW,headingRotate, getScale()));




	mAnimationPathCallback = new ModelAnimationPathCallback();
	mAnimationPathCallback->setAnimationPath(path);
	setUpdateCallback(mAnimationPathCallback);


	if(mRouteLine->getSize() <= 0)
	{
		mRouteLine->addPoint(getPosition());
		mLatestPointLine->addPoint(getPosition());
		mCurrentFlyPoint = getPosition();
	}
	mLatestPointLine->addPoint(mCurrentFlyPoint);
	if(mLatestPointLine->getSize() >= NUM_LATEST_POINT)
	{
		mLatestPointLine->removeFirstPoint();
	}
	if(std::abs(mCurrentHeading - heading) < 5)
		mRouteLine->removePoint();
	mRouteLine->addPoint(mCurrentFlyPoint);

	mCurrentHeading = heading;
	mCurrentFlyPoint = posGeo;
	mTempLine->clear();
}

void AircraftModelNode::stop()
{
	mIsStop = true;
	if(mAnimationPathCallback != nullptr)
		mAnimationPathCallback->getAnimationPath()->clear();
	removeEffect();
}

void AircraftModelNode::dataChanged()
{
	if(mType != mData->info.Type ||
			mIdentification != mData->info.Identification){
		mType = mData->info.Type;
		mIdentification = mData->info.Identification;
		updateOrCreateNodes();
	}
	if(mAircraftinformation)
		mAircraftinformation->updateAircraft();

	updateOrCreateStatusImage();
	mStatusNode->setStyle(mStatusNode->getStyle()); // force PlaceNode to recreate texture
}

const Aircraft::Data& AircraftModelNode::getData() const
{
	return *mData;
}

void AircraftModelNode::goOnTrack()
{
	mDefenseModelLayer->mMapController->setTrackNode(getGeoTransform());
}

void AircraftModelNode::onLeftButtonClicked(bool val)
{
	//select(val);
	updateColors();
	if(val)
	{
		showInfoWidget();
	}
	else
	{
		mDefenseModelLayer->mMapController->untrackNode(getGeoTransform());
		//        mDefenseModelLayer->mMapController->removeNodeFromLayer(mRouteLine, AIRCRAFTS_LAYER_NAME);
		//        mDefenseModelLayer->mMapController->removeNodeFromLayer(mLatestPointLine, AIRCRAFTS_LAYER_NAME);
		//        mDefenseModelLayer->mMapController->removeNodeFromLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
		if(mAircraftinformation)
			mAircraftinformation->setTrackOff();
	}
	if(mCurrentContextMenu){
		mCurrentContextMenu->hideMenu();
		mCurrentContextMenu = nullptr;
	}
}

void AircraftModelNode::frameEvent()
{
	mPat2D->setAttitude(osg::Quat(osg::inDegrees(-double(mDefenseModelLayer->mMapController->getViewpoint().getHeading())
												 + mData->info.Heading),
								  -osg::Z_AXIS));

	mPat2D->setPosition(osg::Vec3d(0, 0, 0));

	if (mCurrentContextMenu) {
		osg::Vec3d wordPos;
		getPosition().toWorld(wordPos);
		float x, y;
		mDefenseModelLayer->mMapController->worldToScreen(wordPos,x, y);
		mCurrentContextMenu->updatePosition(static_cast<int>(x), static_cast<int>(y));
	}


	mStatusNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( 0, 0, 0));

	if(mTempLine->getSize() > 1)
		mTempLine->removePoint();
	mTempLine->addPoint(getPosition());
}

void AircraftModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{

	DefenseModelNode::mousePressEvent(event, onModel);
	if(event->button() == Qt::LeftButton)
	{
		onLeftButtonClicked(onModel);
		if(onModel)
			event->accept();
	}
	if(event->button() == Qt::RightButton) {
		mCurrentContextMenu = new ContextMenu(mDefenseModelLayer->mQmlEngine, mDefenseModelLayer->mUIHandle, this);
		for(auto detectSystem: mData->info.DetectionSystems)
			mCurrentContextMenu->addRow(detectSystem);

		connect(mCurrentContextMenu->getModel(), &ContextMenumodel::itemClicked, this, &AircraftModelNode::onContextmenuItemClicked);
		osg::Vec3d wordPos;
		getPosition().toWorld(wordPos);
		float x, y;
		mDefenseModelLayer->mMapController->worldToScreen(wordPos,x, y);
		mCurrentContextMenu->show(static_cast<int>(x), static_cast<int>(y));
		event->accept();
	}
	if(!onModel && mCurrentContextMenu){
		mCurrentContextMenu->hideMenu();
		mCurrentContextMenu = nullptr;
	}

}

void AircraftModelNode::updateColors()
{
	mModelColor = AircraftInfo::qColor2osgEarthColor(mData->info.aircraftColor());
	DefenseModelNode::updateColors();

	if (mSelectionMode == SELECTED || mHoverMode == HOVERED) {
		mNode2D->setValue(0, false);
		mNode2D->setValue(1, true);
	} else {
		mNode2D->setValue(0, true);
		mNode2D->setValue(1, false);
	}
}

void AircraftModelNode::onGotoButtonClicked()
{
	//    goOnTrack();
	mDefenseModelLayer->mMapController->goToPosition(getPosition(), 400, 0);
	mDefenseModelLayer->mMapController->setTrackNode(getGeoTransform());
}

void AircraftModelNode::onRouteButtonToggled(bool check)
{
	if(check)
	{
		mDefenseModelLayer->mMapController->addNodeToLayer(mRouteLine, AIRCRAFTS_LAYER_NAME);
		mDefenseModelLayer->mMapController->addNodeToLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
	}
	else
	{
		mDefenseModelLayer->mMapController->removeNodeFromLayer(mRouteLine, AIRCRAFTS_LAYER_NAME);
		mDefenseModelLayer->mMapController->removeNodeFromLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
	}

}

void AircraftModelNode::onLatestPointsToggled(bool check) {
	if (check)
	{
		mDefenseModelLayer->mMapController->addNodeToLayer(mLatestPointLine, AIRCRAFTS_LAYER_NAME);
		mDefenseModelLayer->mMapController->addNodeToLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
	}
	else
	{
		mDefenseModelLayer->mMapController->removeNodeFromLayer(mLatestPointLine, AIRCRAFTS_LAYER_NAME);
		mDefenseModelLayer->mMapController->removeNodeFromLayer(mTempLine, AIRCRAFTS_LAYER_NAME);
	}
}

void AircraftModelNode::onTrackButtonToggled(bool check)
{
	//std::cout << check << std::endl;
	if(check)
		mDefenseModelLayer->mMapController->setTrackNode(getGeoTransform());
	else
		mDefenseModelLayer->mMapController->untrackNode(getGeoTransform());
}

void AircraftModelNode::onModeChanged(bool is3DView)
{
	mIs3D = is3DView;
	if(mIs3D)
	{
		mRootNode->setRange(0, 0, RANGE3D);
		mRootNode->setRange(1, RANGE3D, std::numeric_limits<float>::max());

		mDefenseModeNodeAutoScaler->setDefaultScale(mAutoScaleDefaultValue);
		mDefenseModeNodeAutoScaler->setMinScale(mAutoScaleMinValue);
		mDefenseModeNodeAutoScaler->setMaxScale(mAutoScaleMaxValue);

	}
	else
	{
		mRootNode->setRange(0, 0, 0);
		mRootNode->setRange(1,0, std::numeric_limits<float>::max());


		mDefenseModeNodeAutoScaler->setDefaultScale(0.3);
		mDefenseModeNodeAutoScaler->setMinScale(1);
		mDefenseModeNodeAutoScaler->setMaxScale(500);
	}

}

void AircraftModelNode::onContextmenuItemClicked(int /*index*/,  QString /*systemName*/)
{
	//std::cout << index << ", " << systemName.toStdString() << std::endl;
}

void AircraftModelNode::updateOrCreateNodes()
{
	auto node3D = VisualData::get3DNode(mData->info.Type);
	if(!mNode3D->setChild(0, node3D))
		mNode3D->addChild(node3D);


	auto image2D = VisualData::get2DImage(mData->info.Type, mData->info.Identification);
	if(!mImage.valid())
		mImage = new osg::Image(*image2D.image);
	if(!mImageHover.valid())
		mImageHover = new osg::Image(*image2D.imageHover);
	mImage->setImage(image2D.image->s(), image2D.image->t(), image2D.image->r(),
					 image2D.image->getInternalTextureFormat(),image2D.image->getPixelFormat(),
					 image2D.image->getDataType(),image2D.image->data(), osg::Image::AllocationMode::NO_DELETE);
	mImageHover->setImage(image2D.imageHover->s(), image2D.imageHover->t(), image2D.imageHover->r(),
						  image2D.imageHover->getInternalTextureFormat(),image2D.imageHover->getPixelFormat(),
						  image2D.imageHover->getDataType(),image2D.imageHover->data(), osg::Image::AllocationMode::NO_DELETE);

	switch (mData->info.Type) {
	case AircraftInfo::Fighter: {
		mAutoScaleDefaultValue = 11;
		mAutoScaleMinValue = 7;
		mAutoScaleMaxValue = 350;
		break;
	}
	case AircraftInfo::Aircraft: {
		mAutoScaleDefaultValue = 2.5;
		mAutoScaleMinValue = 1;
		mAutoScaleMaxValue = 500;
		break;
	}
	case AircraftInfo::Missile: {
		mAutoScaleDefaultValue = 25;
		mAutoScaleMinValue = 15;
		break;
	}

	case AircraftInfo::Drone: {
		mAutoScaleDefaultValue = 30;
		mAutoScaleMinValue = 20;
		mAutoScaleMaxValue = 400;
		break;
	}
	case AircraftInfo::Helicopter: {
		mAutoScaleDefaultValue = 22;
		mAutoScaleMinValue = 12;
		mAutoScaleMaxValue = 500;
		break;
	}
	}
	//-------------------------------------------------------------------------------------
	onModeChanged(mIs3D);
	updateColors();

}

void AircraftModelNode::showInfoWidget()
{
	if (!mAircraftinformation)
	{
		mAircraftinformation = new AircraftInfoItem(mDefenseModelLayer, *mData, this);
		connect(mAircraftinformation->getInfo(), &AircraftInfoModel::gotoButtonClicked, this, &AircraftModelNode::onGotoButtonClicked);
		connect(mAircraftinformation->getInfo(), &AircraftInfoModel::routeButtonClicked, this, &AircraftModelNode::onRouteButtonToggled);
		connect(mAircraftinformation->getInfo(), &AircraftInfoModel::trackButtonClicked, this, &AircraftModelNode::onTrackButtonToggled);
		connect(mAircraftinformation->getInfo(), &AircraftInfoModel::latestPointsClicked, this, &AircraftModelNode::onLatestPointsToggled);
	}
	mAircraftinformation->show();
}

void AircraftModelNode::addEffect(double emitterDuration)
{

	osgEarth::Registry::shaderGenerator().run(mFire);// for textures or lighting
	getPositionAttitudeTransform()->addChild(mFire);
	mFire->setEmitterDuration(emitterDuration);
	mFire->setParticleDuration(0.2);
	osgEarth::Registry::shaderGenerator().run(mFire->getParticleSystem());// for textures or lighting
	mDefenseModelLayer->mMapController->addNodeToLayer(mFire->getParticleSystem(), AIRCRAFTS_LAYER_NAME);
	//add smoke----------------------------------------------------------------------------------------------------
	osgEarth::Registry::shaderGenerator().run(mSmoke);// for textures or lighting
	getPositionAttitudeTransform()->addChild(mSmoke);
	mSmoke->setEmitterDuration(emitterDuration);
	mSmoke->setParticleDuration(5);
	osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());// for textures or lighting
	mDefenseModelLayer->mMapController->addNodeToLayer(mSmoke->getParticleSystem(), AIRCRAFTS_LAYER_NAME);
}

void AircraftModelNode::removeEffect()
{
	//remove fire---------------------------------------------
	mDefenseModelLayer->mMapController->removeNodeFromLayer(mFire->getParticleSystem(), AIRCRAFTS_LAYER_NAME);
	getPositionAttitudeTransform()->removeChild(mFire);
	//remove smoke--------------------------------------------
	mDefenseModelLayer->mMapController->removeNodeFromLayer(mSmoke->getParticleSystem(), AIRCRAFTS_LAYER_NAME);
	getPositionAttitudeTransform()->removeChild(mSmoke);
}

void AircraftModelNode::updateOrCreateStatusImage()
{
	int height = LABEL_IMAGE_HEIGHT + ((mData->assignments.count()+1)/2) * 30;
	//qDebug()<<"hight:"<<height;
	if (!mRenderStatusImage) {
		mRenderStatusImage = new QImage(
					LABEL_IMAGE_WIDTH,
					height,
					QImage::Format_RGBA8888
					);
	} else {
		mRenderStatusImage->~QImage();
		mRenderStatusImage = new(mRenderStatusImage) QImage(
					LABEL_IMAGE_WIDTH,
					height,
					QImage::Format_RGBA8888
					);
	}

	if (!mStatusImage) {
		mStatusImage = new osg::Image;
	}


	{
		mRenderStatusImage->fill(QColor(Qt::transparent));
		QPainter painter(mRenderStatusImage);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);


		static const QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.3f)));

		static const QFont textFont("SourceSansPro", 12, QFont::Normal);
		static QPen  textPen(QColor(255, 255, 255));

		painter.setPen(Qt::NoPen);
		painter.setBrush(backgroundBrush);

		painter.drawRoundedRect(
					mRenderStatusImage->rect(),
					8,8);
		painter.setBrush(QBrush(QColor(26, 77, 46, int(255 * 0.2f))));
		painter.drawRoundedRect(
					QRect(0, 0, LABEL_IMAGE_WIDTH, 35),
					8,8);
		//------------------------------------------------------------
		static const QPen linePen(QColor(255, 255, 255),
								  1,
								  Qt::PenStyle::DashLine
								  );

		painter.setPen(linePen);
		painter.setBrush(Qt::NoBrush);
		painter.drawLine(0, 35, LABEL_IMAGE_WIDTH, 35);
		painter.drawLine(LABEL_IMAGE_WIDTH/2, 0, LABEL_IMAGE_WIDTH/2, 35);

		painter.setPen(textPen);
		painter.setFont(textFont);
		painter.drawText(QRect(0, 0, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignCenter,
						 mData->info.aircraftTypeToString());
		painter.drawText(QRect(LABEL_IMAGE_WIDTH/2, 0, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignCenter,
						 QString::number(mData->info.TN));
		//-------------------------------------------------------------

		painter.setPen(textPen);
		painter.setFont(textFont);
		painter.drawText(QRect(10, 40, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "CallSign:");
		painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 40, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 mData->info.CallSign);


		painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "IFFCode:");
		painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 70, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 mData->info.IFFCode);


		painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "M-Radar:");
		painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 100, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 mData->info.MasterRadar);

		painter.drawText(QRect(10, 130, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "I-Method:");
		painter.drawText(QRect(10 + LABEL_IMAGE_WIDTH/2, 130, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 mData->info.IdentificationMethod);
		//---------------------------------------------------------

		painter.setPen(linePen);
		painter.setBrush(Qt::NoBrush);
		painter.drawLine(0, 165, LABEL_IMAGE_WIDTH, 165);

		painter.setPen(textPen);

		painter.drawText(QRect(10, 170, LABEL_IMAGE_WIDTH/2, 30),
						 Qt::AlignLeft | Qt::AlignVCenter,
						 "Assignments:");

		int h = 200;
		const QFontMetrics fm(textFont);
		int n = 0;
		while (n < mData->assignments.count()) {

			int indent = 0;
			for (int llidx = 0; llidx < 2; llidx++)// two elements per line
			{

				if (n >= mData->assignments.count())
					break;

				auto val = mData->assignments.at(n);

				const QString ss = (llidx == 0 ? QStringLiteral("(") : QStringLiteral(", ("))
						+ QString::number(val->info->systemInfo.Number)
						+ QStringLiteral(", ");
				const QString cc = QString(val->info->systemCombatInfo.phaseToString()[0]);
				const QString ee = QStringLiteral(")");

				textPen.setColor(QColor(255,255,255));
				painter.setPen(textPen);

				painter.drawText(QRect(10 + indent, h, LABEL_IMAGE_WIDTH, 30),
								 Qt::AlignLeft | Qt::AlignVCenter,
								 ss);
				indent += fm.boundingRect(ss).width();

				//                textPen.setColor(val->getSystemCombatInfo().phaseToColor());
				painter.setPen(textPen);

				painter.drawText(QRect(10 + indent, h, LABEL_IMAGE_WIDTH, 30),
								 Qt::AlignLeft | Qt::AlignVCenter,
								 cc);
				indent += fm.boundingRect(cc).width();

				textPen.setColor(QColor(255,255,255));
				painter.setPen(textPen);

				painter.drawText(QRect(10 + indent, h, LABEL_IMAGE_WIDTH, 30),
								 Qt::AlignLeft | Qt::AlignVCenter,
								 ee);
				indent += fm.boundingRect(ee).width();


				n++;
			}

			h += 30;
		}


	}
	*mRenderStatusImage = mRenderStatusImage->mirrored(false, true);

	mStatusImage->setImage(LABEL_IMAGE_WIDTH,
						   height,
						   1,
						   GL_RGBA,
						   GL_RGBA,
						   GL_UNSIGNED_BYTE,
						   mRenderStatusImage->bits(),
						   osg::Image::AllocationMode::NO_DELETE);
}




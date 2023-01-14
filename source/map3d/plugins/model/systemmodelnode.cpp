#include "systemmodelnode.h"
#include "truck.h"

#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Material>

const int RANGE3D = 500;

SystemModelNode::SystemModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent)
    :BaseModel(mapControler->getMapNode(), parent), mMapController(mapControler), mUIHandle(uiHandle), mQmlEngine(qmlEngine)
{
    //--create root node--------------------------------------------------------------------------
    mRootNode = new osg::LOD;

    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    setStyle(rootStyle);
    //--create 2D Nodes---------------------------------------------------------------------------
    osg::Image* redIcon = osgDB::readImageFile("../data/models/system/system_red.png");
    redIcon->scaleImage(32, 32, redIcon->r());
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(redIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
    redGeode->addDrawable(redImageDrawable);

    osg::Image* yellowIcon = osgDB::readImageFile("../data/models/system/system_yell.png");
    yellowIcon->scaleImage(32, 32, yellowIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(yellowIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
    yellowGeode->addDrawable(yellowImageDrawable);

    mNode2D = new osg::Switch;
    mNode2D->addChild(yellowGeode, false);
    mNode2D->addChild(redGeode, true);
    //--create 3D node---------------------------------------------------------------------------
    osg::ref_ptr<Truck> truck = new Truck(getMapNode());
    mNode3D = truck;
    //--create lable-----------------------------------------------------------------------------
    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;
    osg::Image* lableImage = osgDB::readImageFile("../data/models/text-background.png");
    mLableNode = new osgEarth::Annotation::PlaceNode(getName(),labelStyle, lableImage);
    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3(0, 0.5, 1));
    getGeoTransform()->addChild(mLableNode);
    //--add nods--------------------------------------------------------------------------------
    if(mIs3D)
    {
        mRootNode->addChild(mNode3D, 0, RANGE3D);
        mRootNode->addChild(mNode2D, RANGE3D, std::numeric_limits<float>::max());
    }
    else
    {
        mRootNode->addChild(mNode3D, 0, 0);
        mRootNode->addChild(mNode2D, 0, std::numeric_limits<float>::max());
    }
    //map mode changed-----------------------------------------------------------------------
    connect(mapControler, &MapController::modeChanged, this, &SystemModelNode::onModeChanged);
    //--create shapes-----------------------------------------------------------------------------
    mRangeCircle = new Circle(mMapController, true);
    mRangeCircle->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.5f));

    mMezSphere = new SphereNode();

}

void SystemModelNode::setInformation(const SystemInfo& info)
{
    mInformation = info;
}

void SystemModelNode::goOnTrack()
{
    mMapController->setTrackNode(getGeoTransform());
}

void SystemModelNode::frameEvent()
{
    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( getPositionAttitudeTransform()->getBound().radius()/2, getPositionAttitudeTransform()->getBound().radius(), 2));
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

    select(mIsSelected);
}

void SystemModelNode::onLeftButtonClicked(bool val)
{
    select(val);
    if(val)
    {
        showInfoWidget();
    }
    else
    {
        mMapController->untrackNode();
    }
}

void SystemModelNode::showInfoWidget()
{
    SystemInformation *systemInformation = new SystemInformation(mQmlEngine, mUIHandle, mInformation, this);
    connect(systemInformation->getInfo(), &SystemInfoModel::gotoButtonClicked, this, &SystemModelNode::onGotoButtonClicked);
    connect(systemInformation->getInfo(), &SystemInfoModel::rangeButtonClicked, this, &SystemModelNode::onRangeButtonToggled);
    connect(systemInformation->getInfo(), &SystemInfoModel::wezButtonClicked, this, &SystemModelNode::onWezButtonToggled);
    connect(systemInformation->getInfo(), &SystemInfoModel::mezButtonClicked, this, &SystemModelNode::onMezButtonToggled);
    systemInformation->show();
}

void SystemModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{
    BaseModel::mousePressEvent(event, onModel);
    if(event->button() == Qt::LeftButton)
    {
        onLeftButtonClicked(onModel);
        if(onModel)
            event->accept();
    }
}

void SystemModelNode::onGotoButtonClicked()
{
    mMapController->goToPosition(getPosition(), 200);
}

void SystemModelNode::onRangeButtonToggled(bool check)
{
    if(check)
    {
        mRangeCircle->setPosition(getPosition());
        mRangeCircle->setRadius(osgEarth::Distance(mInformation.ViewRange, osgEarth::Units::METERS));
        mMapController->addNode(mRangeCircle);
    }
    else
    {
        mMapController->removeNode(mRangeCircle);
    }
}

void SystemModelNode::onWezButtonToggled(bool checked)
{

}

void SystemModelNode::onMezButtonToggled(bool checked)
{
    if(checked)
    {
        mMezSphere->setPosition(getPosition());
        mMezSphere->setRadius(mInformation.MezRange);
        mMezSphere->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.5f));
        mMapController->addNode(mMezSphere);
    }
    else
    {
        mMapController->removeNode(mMezSphere);
    }
}
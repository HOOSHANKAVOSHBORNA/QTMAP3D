#include "defenseModelLayer.h"
#include "draw.h"
#include "truck.h"
#include "rocket.h"
#include "systemModelNode.h"
#include "stationModelNode.h"
#include "mapcontroller.h"
#include "defenseDataManager.h"

#include <QDebug>
#include <QMainWindow>
#include <QTimer>
#include <QDockWidget>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QQmlComponent>

#include <osgDB/ReadFile>
#include <osgEarthSymbology/GeometryFactory>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/ModelNode>
#include <osg/PositionAttitudeTransform>
#include <osgEarth/GeoData>
#include <osg/PositionAttitudeTransform>
#include <osg/AnimationPath>
#include <osg/LineWidth>
#include <osg/Array>
#include <osgAnimation/EaseMotion>
#include <osgEarth/ElevationLayer>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgViewer/Viewer>
#include <QQuickItem>
#include <QSortFilterProxyModel>


#include "aircraftTableModel.h"
#include "contextMenu.h"
#include "aircraftInformation.h"
#include "stationTableModel.h"
#include "stationInformation.h"
#include "systemInformation.h"
#include "systemTableModel.h"

//const QString FLYING = "Flying";
const QString AIRCRAFT = "Aircraft";
const QString SYSTEM = "System";
const QString STATION = "Station";
//const QString ROCKET = "Rocket";
//const QString TRUCK = "Truck";
//----------------------------------------------
const QString CATEGORY = "Defense Model";
//const QString ADD_AIRCRAFT = "Add Aircraft";
const QString ADD_ROCKET = "Fire";
//const QString ADD_TRUCK = "Add Truck";
//const QString ADD_STATION = "Add Station";
//const QString ADD_SYSTEM = "Add System";

DefenseModelLayer::DefenseModelLayer(QObject *parent)
    : PluginInterface(parent)
{
    //    Q_INIT_RESOURCE(modelqml);
    Q_INIT_RESOURCE(defenseModelLayer);
    Q_INIT_RESOURCE(defenseModelLayerqml);
}

bool DefenseModelLayer::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc)
{
    //    pDesc->pluginHasSideItem = true;
    //    pDesc->sideItemMenuBarTitle = "Layers";
    //    pDesc->sideItemMenuBarIconUrl = "qrc:///test1plugin/resources/Layers.png";
    //    pDesc->sideItemUrl = "qrc:///test1plugin/Layers.qml";

    qmlRegisterType<AircraftTableModel>("Crystal", 1, 0, "AircraftTableModel");
    qmlRegisterType<ContextMenumodel>("Crystal", 1, 0, "ContextMenumodel");
    qmlRegisterType<AircraftInfoModel>("Crystal", 1, 0, "AircraftInfoModel");
    qmlRegisterType<StationTableModel>("Crystal", 1, 0, "StationTableModel");
    qmlRegisterType<StationInfoModel>("Crystal", 1, 0, "StationInfoModel");
    qmlRegisterType<SystemInfoModel>("Crystal", 1, 0, "SystemInfoModel");
    qmlRegisterType<SystemTableModel>("Crystal", 1, 0, "SystemTableModel");
    mQmlEngine = engine;

    pDesc->toolboxItemsList.push_back(new ItemDesc{AIRCRAFT, CATEGORY, "qrc:/resources/airplan.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{SYSTEM, CATEGORY, "qrc:/resources/system_1.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{STATION, CATEGORY, "qrc:/resources/station_lV.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{ADD_ROCKET, CATEGORY, "", false, false, ""});
    return true;
}

void DefenseModelLayer::onSideItemCreated(int /*index*/, QObject */*pSideItem*/)
{
}

void DefenseModelLayer::onToolboxItemClicked(const QString &name, const QString &category)
{
    if(CATEGORY == category && name == AIRCRAFT)
    {
        AircraftInfo aircraftInfo;
        aircraftInfo.TN = mModelNodes[AIRCRAFT].count();
        aircraftInfo.Longitude = 52.8601;
        aircraftInfo.Latitude = 35.277;
        aircraftInfo.Altitude = 9100;
        aircraftInfo.Heading = 30;
        aircraftInfo.IFFCode = "a12345";
        aircraftInfo.CallSign = "cls";
        aircraftInfo.Type = "type2";
        aircraftInfo.MasterRadar = "radar2";
        aircraftInfo.Identification = AircraftInfo::X;
        aircraftInfo.IdentificationMethod = "mtd";
        aircraftInfo.Time = "1401/10/21 10:00 fdfd";
        aircraftInfo.Pos = "pos1";
        addUpdateAircraft(aircraftInfo);
        //demo();
    }
    if(CATEGORY == category && name == ADD_ROCKET)
    {
        for(auto modelNode:mModelNodes[SYSTEM])
        {
            auto systemModelNode = dynamic_cast<SystemModelNode*>(modelNode);
            if(systemModelNode && systemModelNode->getAssignedModelNode())
                systemModelNode->fire();
        }
    }
    else if(CATEGORY == category && name == SYSTEM)
    {
        SystemInfo systemInfo;
        systemInfo.Name = SYSTEM + QString::number(mModelNodes[SYSTEM].count());
        systemInfo.Longitude = 54.2;
        systemInfo.Latitude = 35.3;
        systemInfo.Number = 1234567;
        addUpdateSystem(systemInfo);

        SystemStatusInfo systemStatusInfo;
        systemStatusInfo.Number = systemInfo.Number;
        systemStatusInfo.BCCStatus = "s";
        systemStatusInfo.RadarSearchStatus = "us";
        systemStatusInfo.MissileCount = 3;
        onSystemStatusInfoChanged(systemStatusInfo);

    }
    else if(CATEGORY == category && name == STATION)
    {
        StationInfo stationInfo;
        stationInfo.Name = STATION + QString::number(mModelNodes[STATION].count());
        stationInfo.Longitude = 52;
        stationInfo.Latitude = 35.2;
        stationInfo.Number = 1234567;
        stationInfo.PrimSec = "primary";
        stationInfo.CycleTime = 10000;
        addUpdateStation(stationInfo);
    }
}

bool DefenseModelLayer::setup(MapController *mapController,
                              UIHandle *uiHandle)
{
    mMapController = mapController;
    mUIHandle = uiHandle;

    mDataManager = new DataManager(mQmlEngine, mUIHandle, this);
    connect(mDataManager, &DataManager::aircraftDoubleClicked,[=](int TN){

        if(mModelNodes[AIRCRAFT].contains(TN))
        {
            AircraftModelNode* aircraftModelNode = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][TN]);
            if(mSelectedModelNode)
                mSelectedModelNode->select(false);
            aircraftModelNode->onLeftButtonClicked(true);
            aircraftModelNode->goOnTrack();
            mSelectedModelNode = aircraftModelNode;
        }
    });
    connect(mDataManager, &DataManager::stationDoubleClicked,[=](int number){

        if(mModelNodes[STATION].contains(number))
        {
            StationModelNode* stationModelNode = dynamic_cast<StationModelNode*>(mModelNodes[STATION][number]);
            if(mSelectedModelNode)
                mSelectedModelNode->select(false);
            stationModelNode->onLeftButtonClicked(true);
            stationModelNode->goOnTrack();
            mSelectedModelNode = stationModelNode;
        }
    });
    connect(mDataManager, &DataManager::systemDoubleClicked,[=](int number){

        if(mModelNodes[SYSTEM].contains(number))
        {
            SystemModelNode* systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][number]);
            if(mSelectedModelNode)
                mSelectedModelNode->select(false);
            systemModelNode->onLeftButtonClicked(true);
            systemModelNode->goOnTrack();
            mSelectedModelNode = systemModelNode;
        }
    });
}

void DefenseModelLayer::setDefenseDataManager(DefenseDataManager *defenseDataManager)
{
    //--aircraft--------------------------------------------------------
    QObject::connect(defenseDataManager, &DefenseDataManager::aircraftInfoChanged,this ,&DefenseModelLayer::onAircraftInfoChanged);
    QObject::connect(defenseDataManager, &DefenseDataManager::clearAircraft,this ,&DefenseModelLayer::onClearAircraft);
    //--system----------------------------------------------------------
    QObject::connect(defenseDataManager, &DefenseDataManager::systemInfoChanged,this ,&DefenseModelLayer::onSystemInfoChanged);
    QObject::connect(defenseDataManager, &DefenseDataManager::systemStatusInfoChanged,this ,&DefenseModelLayer::onSystemStatusInfoChanged);
    QObject::connect(defenseDataManager, &DefenseDataManager::systemCambatInfoChanged,this ,&DefenseModelLayer::onSystemCambatInfoChanged);
    //--station---------------------------------------------------------
    QObject::connect(defenseDataManager, &DefenseDataManager::stationInfoChanged,this ,&DefenseModelLayer::onStationInfoChanged);

}

//void DefenseModelLayer::demo()
//{
//    //    int index = 0;
//    auto airplaneNames = mModelNodes[AIRCRAFT].keys();
//    for (auto name: airplaneNames)
//    {
//        auto model = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][name]);
//        auto mapPoint = model->getPosition();
//        osgEarth::GeoPoint  latLongPoint;
//        //latLongPoint.altitudeMode() = osgEarth::AltitudeMode::ALTMODE_ABSOLUTE;
//        //mapPoint.transform(osgEarth::SpatialReference::get("wgs84"), latLongPoint);
//        osg::Vec3d currentPos;
//        mapPoint.toWorld(currentPos);
//        //osg::Vec3d currentPos(latLongPoint.vec3d());
//        //-- 500 km/h ~ 139 m/s ------------------
//        //    int randomX = 10*(138 + (qrand() % 139));
//        //    int randomY = 10*(138 + (qrand() % 139));
//        //    int val = qrand() % 4;
//        //    if(val == 1)
//        //        currentPos += osg::Vec3d(randomX, randomY, 0.0);
//        //    else if(val == 2)
//        //        currentPos += osg::Vec3d(randomX, -randomY, 0.0);
//        //    else if(val == 3)
//        //        currentPos += osg::Vec3d(-randomX, randomY, 0.0);
//        //    else
//        //        currentPos += osg::Vec3d(-randomX, -randomY, 0.0);

//        int randomX = (100 + (qrand() % 19));
//        int randomY = (100 + (qrand() % 19));
//        int val = qrand() % 4;
//        if(val == 1)
//            mapPoint.vec3d() += osg::Vec3d(randomX/10000.0, randomY/10000.0, 0.0);
//        else if(val == 2)
//            mapPoint.vec3d() += osg::Vec3d(randomX/10000.0, -randomY/10000.0, 0.0);
//        else if(val == 3)
//            mapPoint.vec3d() += osg::Vec3d(-randomX/10000.0, randomY/10000.0, 0.0);
//        else
//            mapPoint.vec3d() += osg::Vec3d(-randomX/10000.0, -randomY/10000.0, 0.0);

//        //    setPosition(mCurrentWorldPoint, 138);
//        latLongPoint.fromWorld(osgEarth::SpatialReference::get("wgs84"), currentPos);
//        //qDebug()<<QString::fromUtf8(latLongPoint.toString().c_str());
//        model->flyTo(mapPoint.vec3d(),30, 138);

//        // fallow racket
//        //        auto truckNames = mModels[TRUCK].keys();
//        //        for(auto truckName: truckNames)
//        //        {
//        //            auto modeltruck = dynamic_cast<Truck*>(mModels[TRUCK][truckName]);
//        //            if(modeltruck->shoot())
//        //            {
//        //                addRocketModel(modeltruck->getPosition().vec3d());
//        //                auto modelRocket = dynamic_cast<FlyingModel*>(mModels[ROCKET].last());
//        //                modelRocket->flyTo(mapPoint.vec3d(), 120);
//        //                model->setFollowingModel(modelRocket);
//        //                break;
//        //            }
//        //        }
//        //        index += 1;
//    }
//}

//void DefenseModelLayer::addTruckModel()
//{
//    osgEarth::GeoPoint position(mMapController->getMapSRS()->getGeographicSRS(),52.8603, 35.274, 100, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
//    //create and setting model--------------------------------------------
//    osg::ref_ptr<Truck> model = new Truck(mMapController->getMapNode(), nullptr);
//    QString name = TRUCK + QString::number(mModelNodes[TRUCK].count());
//    model->setName(name.toStdString());
//    model->setPosition(position);
//    //model->setLocalRotation(osg::Quat(osg::inDegrees(-30.0),osg::Z_AXIS));
//    model->setScale(osg::Vec3(1,1,1));



//    //    QObject::connect(model.get(), &FlyingModel::positionChanged, [=](osgEarth::GeoPoint position){
//    //        positionChanged(ROCKET, name, position);
//    //    });

//    //add to container-----------------------------------------------------
//    //mModelNodes[TRUCK][name] = model;

//    //add to map ---------------------------------------------------------
//    mMapController->addNode(model);
//    mMapController->goToPosition(position.x(), position.y(), position.z() + 600);

//    //create random position ---------------------------------------------
//    //    double rnd = QRandomGenerator::global()->generateDouble();
//    double rndRotate = qrand() % 360;
//    model->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(rndRotate), osg::Z_AXIS));

//    double rndPX = (qrand() % 200)/100000.0;
//    double rndPY = (qrand() % 200)/100000.0;
//    osg::Vec3d nPosition(rndPX, rndPY, 0.0);


//    //    nPosition += position;
//    //model->setLatLongPosition(nPosition);
//    //mMap3dWidget->goPosition(nPosition.x(), nPosition.y(), nPosition.z() + 500);
//    //move
//    //model->moveTo(nPosition,10);
//}

void DefenseModelLayer::addUpdateAircraft(AircraftInfo aircraftInfo)
{
    osg::ref_ptr<AircraftModelNode> aircraftModelNode;
    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
                                          aircraftInfo.Longitude, aircraftInfo.Latitude, aircraftInfo.Altitude);

    if(mModelNodes.contains(AIRCRAFT) && mModelNodes[AIRCRAFT].contains(aircraftInfo.TN))
    {
        aircraftModelNode = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][aircraftInfo.TN]);
        aircraftModelNode->flyTo(geographicPosition.vec3d(), aircraftInfo.Heading, aircraftInfo.Speed);

    }
    else
    {
        //create and model node------------------------------------------------
        aircraftModelNode = new AircraftModelNode(mMapController, mQmlEngine,mUIHandle);
        aircraftModelNode->setQStringName(QString::number(aircraftInfo.TN));
        aircraftModelNode->setGeographicPosition(geographicPosition, aircraftInfo.Heading);

        //        QObject::connect(modelNode.get(), &BaseModel::positionChanged, [=](osgEarth::GeoPoint position){
        //            //positionChanged(AIRPLANE, name, position);

        //            auto truckNames = modelNode[TRUCK].keys();
        //            for(auto truckName: truckNames)
        //            {
        //                auto truck = dynamic_cast<Truck*>(modelNode[TRUCK][truckName]);
        //                if(truck->hasRocket())
        //                {
        //                    //                osg::Vec3d wPoint;
        //                    //                position.toWorld(wPoint);
        //                    truck->aimTarget(position.vec3d());
        //                }
        //            }
        //        });
        //add to container-----------------------------------------------------
        mModelNodes[AIRCRAFT][aircraftInfo.TN] = aircraftModelNode;
        //add to map ---------------------------------------------------------
        mMapController->addNode(aircraftModelNode);
        //hit------------------------------------------------------------------
        //        QObject::connect(modelNode.get(), &BaseModel::hit, [=](BaseModel */*other*/){

        //            mModelNodes[AIRCRAFT].remove(QString(modelNode->getName().c_str()));
        //        });
        //        if(mModelNodes.contains(SYSTEM))
        //        {
        //            auto systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM].first());
        //            systemModelNode->setAssignedModelNode(aircraftModelNode);
        //        }
    }
    //update information------------------------------------------------------------------
    aircraftModelNode->setInformation(aircraftInfo);
    //add update list view-----------------------------------------------------------------
    if (mDataManager)
    {
        mDataManager->setAircraftInfo(aircraftInfo);
    }

}

//void DefenseModelLayer::addRocketModel(osg::Vec3d position)
//{
//    osgEarth::GeoPoint geoposition(mMapController->getMapSRS()->getGeographicSRS(),position);
//    //create and setting model--------------------------------------------
//    osg::ref_ptr<Rocket> model = new Rocket(mMapController->getMapNode(), parent());
//    QString name = ROCKET + QString::number(mModelNodes[ROCKET].count());
//    model->setName(name.toStdString());
//    model->setGeographicPosition(geoposition, 0.0);
//    model->setScale(osg::Vec3(1,1,1));

////    QObject::connect(model.get(), &DefenseModelNode::positionChanged, [=](osgEarth::GeoPoint position){
////        positionChanged(ROCKET, name, position);
////    });

//    //add to container-----------------------------------------------------
//    mModelNodes[ROCKET][name] = model;


//    //add to map ---------------------------------------------------------
//    mMapController->addNode(model);
//    //mMap3dWidget->goPosition(position.x(), position.y(), position.z() + 500);
//    mMapController->setTrackNode(model->getGeoTransform());
//    //    mMap3dWidget->setObjectInfoWidgetVisible(false);

//    //    double rnd = QRandomGenerator::global()->generateDouble();
//    //    double rnd = qrand() % 360;
//    //    model->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(rnd), osg::Z_AXIS));
//    //hit------------------------------------------------------------------
////    QObject::connect(model.get(), &DefenseModelNode::hit, [=](DefenseModelNode */*other*/){

////        mModelNodes[ROCKET].remove(QString(model->getName().c_str()));
////    });
//}

void DefenseModelLayer::addUpdateSystem(SystemInfo systemInfo)
{
    osg::ref_ptr<SystemModelNode> systemModelNode;
    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
                                          systemInfo.Longitude, systemInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mModelNodes.contains(SYSTEM) && mModelNodes[SYSTEM].contains(systemInfo.Number))
    {
        systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][systemInfo.Number]);
    }
    else
    {
        //create and setting model-------------------------------------------
        systemModelNode = new SystemModelNode(mMapController, mQmlEngine, mUIHandle);
        systemModelNode->setQStringName(systemInfo.Name);
        systemModelNode->setGeographicPosition(geographicPosition, 0.0);
        //add to container---------------------------------------------------
        mModelNodes[SYSTEM][systemInfo.Number] = systemModelNode;
        //add to map --------------------------------------------------------
        mMapController->addNode(systemModelNode);
    }
    //update information-----------------------------------------------------
    systemModelNode->setInformation(systemInfo);
    //add update list view-----------------------------------------------------------------
    if (mDataManager)
    {
        mDataManager->setSystemInfo(systemInfo);
    }
}

void DefenseModelLayer::addUpdateStation(StationInfo stationInfo)
{
    osg::ref_ptr<StationModelNode> stationModelNode;
    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
                                          stationInfo.Longitude, stationInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mModelNodes.contains(STATION) && mModelNodes[STATION].contains(stationInfo.Number))
    {
        stationModelNode = dynamic_cast<StationModelNode*>(mModelNodes[STATION][stationInfo.Number]);
    }
    else
    {
        //create and setting model-------------------------------------------
        stationModelNode = new StationModelNode(mMapController, mQmlEngine, mUIHandle);
        stationModelNode->setQStringName(stationInfo.Name);
        stationModelNode->setGeographicPosition(geographicPosition, 0.0);
        //add to container---------------------------------------------------
        mModelNodes[STATION][stationInfo.Number] = stationModelNode;
        //add to map --------------------------------------------------------
        mMapController->addNode(stationModelNode);
    }
    //update information-----------------------------------------------------
    stationModelNode->setInformation(stationInfo);
    //add update list view-----------------------------------------------------------------
    if (mDataManager)
    {
        mDataManager->setStationInfo(stationInfo);
    }

}

//void DefenseModelLayer::clickedTrackNode(QString type, QString name, bool isClick)
//{
//    if (isClick){
//        //        osg::Node* node =mModels[type][name];
//        mMapController->setTrackNode(mModelNodes[type][name]->getGeoTransform());
//        //qDebug()<<"fgd";

//        //        if(type == AIRPLANE) {
//        //            mMap3dWidget->setObjectInfoWidgetVisible(true);
//        //            mMap3dWidget->setSelectedAirplane(mModels[type][name]);
//        //        } else {
//        //            mMap3dWidget->setObjectInfoWidgetVisible(false);
//        //        }

//    } else {

//        mMapController->untrackNode();
//        //        mMap3dWidget->setObjectInfoWidgetVisible(false);

//    }
//}

void DefenseModelLayer::positionChanged(QString /*type*/, QString /*name*/, osgEarth::GeoPoint /*position*/)
{
}

//void DefenseModelLayer::onMessageReceived(const QJsonDocument &message)
//{
//    if(message.object().value("Name").toString() == "Aircraft")
//    {
//        QJsonObject data = message.object().value("Data").toObject();
//        AircraftInfo aircraftInfo;
//        aircraftInfo.fromJson(QJsonDocument(data));
//        //qDebug()<<"target:"<< data;
//        addUpdateAircraft(aircraftInfo);
//    }
//    if(message.object().value("Name").toString() == "Station")
//    {
//        QJsonObject data = message.object().value("Data").toObject();
//        StationInfo stationInfo;
//        stationInfo.fromJson(QJsonDocument(data));
//        //        qDebug()<<"station:"<< data;
//        addUpdateStation(stationInfo);
//    }
//    if(message.object().value("Name").toString() == "System")
//    {
//        QJsonObject data = message.object().value("Data").toObject();
//        SystemInfo systemInfo;
//        systemInfo.fromJson(QJsonDocument(data));
//        //        qDebug()<<"station:"<< data;
//        addUpdateSystem(systemInfo);
//    }

//}

void DefenseModelLayer::onAircraftInfoChanged(AircraftInfo &aircraftInfo)
{
    addUpdateAircraft(aircraftInfo);
}

void DefenseModelLayer::onSystemInfoChanged(SystemInfo &systemInfo)
{
    addUpdateSystem(systemInfo);
}

void DefenseModelLayer::onSystemStatusInfoChanged(SystemStatusInfo &systemStatusInfo)
{
    if(mModelNodes.contains(SYSTEM) && mModelNodes[SYSTEM].contains(systemStatusInfo.Number))
    {
        auto systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][systemStatusInfo.Number]);
        //update information-----------------------------------------------------
        systemModelNode->setSystemStatusInfo(systemStatusInfo);
        //add update list view-----------------------------------------------------------------
        if (mDataManager)
        {
            mDataManager->setSystemInfo(systemInfo);
        }
    }
}

void DefenseModelLayer::onSystemCambatInfoChanged(SystemCambatInfo &systemCambatInfo)
{
    if(mModelNodes.contains(SYSTEM) && mModelNodes[SYSTEM].contains(systemCambatInfo.Number))
    {
        auto systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][systemCambatInfo.Number]);
        //update information-----------------------------------------------------
        systemModelNode->setSystemCambatInfo(systemCambatInfo);
        //add update list view-----------------------------------------------------------------
        if (mDataManager)
        {
            mDataManager->setSystemInfo(systemInfo);
        }
    }
}

void DefenseModelLayer::onStationInfoChanged(StationInfo &stationInfo)
{
    addUpdateStation(stationInfo);
}

void DefenseModelLayer::onClearAircraft(int tn)
{
    if(mModelNodes.contains(AIRCRAFT) && mModelNodes[AIRCRAFT].contains(tn))
    {
        auto aircraftModelNode = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][tn]);
        aircraftModelNode->onLeftButtonClicked(false);
        aircraftModelNode->setNodeMask(false);
        //        mMapController->removeNode(aircraftModelNode);
        //        mModelNodes[AIRCRAFT].remove(tn);
        mDataManager->deleteAircraftInfo(tn);
    }
}

void DefenseModelLayer::frameEvent()
{
    //    findSceneModels(mMapController->getViewer());
    for(auto modelNodeList: mModelNodes)
        for(auto modelNode: modelNodeList)
            modelNode->frameEvent();
    //    if(mLastSelectedModel)
    //        mLastSelectedModel->frameEvent();
}

void DefenseModelLayer::mousePressEvent(QMouseEvent *event)
{

    DefenseModelNode* modelNode = pick(event->x(), event->y());
    if(modelNode)
    {
        modelNode->mousePressEvent(event, true);
    }
    if(mSelectedModelNode && mSelectedModelNode != modelNode)
        mSelectedModelNode->mousePressEvent(event, false);
    if(modelNode)
        mSelectedModelNode = modelNode;
    //--drag aircraft---------------------------------------
    if(event->button() == Qt::LeftButton)
    {
        auto aircraftModelNode  = dynamic_cast<AircraftModelNode*>(modelNode);
        if(aircraftModelNode)
        {
            mDragAircraftModelNode = aircraftModelNode->getDragModelNode();
            mMapController->addNode(mDragAircraftModelNode);
        }
    }

}

void DefenseModelLayer::mouseReleaseEvent(QMouseEvent *event)
{
    //--drag aircraft--------------------------------------------
    if(event->button() == Qt::LeftButton && mDragAircraftModelNode)
    {
        auto systemModelNode  = dynamic_cast<SystemModelNode*>(mOnMoveModelNode);
        if(systemModelNode)
        {
            auto aircraftModelNode  = dynamic_cast<AircraftModelNode*>(mSelectedModelNode);
            systemModelNode->setAssignedModelNode(aircraftModelNode);
        }
        mMapController->removeNode(mDragAircraftModelNode);
        mDragAircraftModelNode = nullptr;
    }
}

void DefenseModelLayer::mouseMoveEvent(QMouseEvent *event)
{
    DefenseModelNode* modelNode = pick(event->x(), event->y());
    if(modelNode)
    {
        modelNode->mouseMoveEvent(event, true);
    }
    if(mOnMoveModelNode && mOnMoveModelNode != modelNode)
        mOnMoveModelNode->mouseMoveEvent(event, false);
    if(modelNode)
        mOnMoveModelNode = modelNode;
    //--drag aircraft---------------------------------------
    if(mDragAircraftModelNode)
    {
        osgEarth::GeoPoint mouseGeoPoint = mMapController->screenToGeoPoint(event->x(), event->y());
        mDragAircraftModelNode->setPosition(mouseGeoPoint);
    }
}

DefenseModelNode *DefenseModelLayer::pick(float x, float y)
{
    DefenseModelNode* defenseModelNode = nullptr;
    osgViewer::Viewer * viewer = mMapController->getViewer();
    float height = static_cast<float>(viewer->getCamera()->getViewport()->height());
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (viewer->computeIntersections(x, height - y, intersections))
    {
        for(osgUtil::LineSegmentIntersector::Intersection hit : intersections)
        {
            const osg::NodePath& nodePath = hit.nodePath;
            for(osg::NodePath::const_iterator nitr=nodePath.begin();
                nitr!=nodePath.end();
                ++nitr)
            {
                defenseModelNode = dynamic_cast<DefenseModelNode*>(*nitr);
                if (defenseModelNode)
                    return defenseModelNode;
            }
        }
    }
    return defenseModelNode;
}
void DefenseModelLayer::findSceneModels(osgViewer::Viewer *viewer)
{
    osgEarth::Util::EarthManipulator*camera = dynamic_cast<osgEarth::Util::EarthManipulator*>(viewer->getCameraManipulator());
    if(!camera)
        return;
    int range = static_cast<int>(camera->getViewpoint().getRange());
    if(range != mPreCameraRange && range < 12000)
    {
        mPreCameraRange = range;
        osg::Viewport* viewport = viewer->getCamera()->getViewport();
        osg::ref_ptr<osgUtil::PolytopeIntersector> intersector{nullptr};
        intersector = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, viewport->x(), viewport->y(),
                                                       viewport->x() + viewport->width(), viewport->y() + viewport->height());

        intersector->setPrimitiveMask(osgUtil::PolytopeIntersector::ALL_PRIMITIVES);
        intersector->setIntersectionLimit( osgUtil::Intersector::LIMIT_ONE_PER_DRAWABLE );

        osgUtil::IntersectionVisitor iv(intersector);
        //        iv.setTraversalMask(NODE_MASK);
        //        iv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
        //        iv.setTraversalNumber(1000);
        viewer->getCamera()->accept(iv);

        if(intersector->containsIntersections())
        {
            auto intersections = intersector->getIntersections();
            //qDebug() <<"intersections: "<<intersections.size();
            for(auto hit : intersections)
            {

                const osg::NodePath& nodePath = hit.nodePath;
                //qDebug() <<"nodePath: "<<nodePath.size();
                for(osg::NodePath::const_iterator nitr=nodePath.begin();
                    nitr!=nodePath.end();
                    ++nitr)
                {
                    DefenseModelNode* defenseModelNode = dynamic_cast<DefenseModelNode*>(*nitr);
                    if (defenseModelNode)
                    {
                        //qDebug() <<model->getQStringName();
                        //qDebug() <<"range: "<<camera->getViewpoint().getRange();
                        //qDebug() <<"z: "<<model->getPosition().z();
                        double distance = 0;
                        if(camera->getViewpoint().getRange() < defenseModelNode->getPosition().z())///for track node
                            distance = camera->getViewpoint().getRange();
                        else
                            distance = camera->getViewpoint().getRange() - defenseModelNode->getPosition().z();
                        //                        model->cameraRangeChanged(distance);
                        //qDebug() <<"camera->getViewpoint().getRange(): "<<camera->getViewpoint().getRange();
                        //qDebug() <<"model.getRange(): "<<camera->getViewpoint().getRange() - model->getPosition().z();
                    }
                }
            }

        }
    }
}

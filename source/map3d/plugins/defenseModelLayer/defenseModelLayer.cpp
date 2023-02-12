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
#include <osgEarth/ModelLayer>

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
#include <thread>


#include "aircraftTableModel.h"
#include "contextMenu.h"
#include "aircraftInformation.h"
#include "stationTableModel.h"
#include "stationInformation.h"
#include "systemInformation.h"
#include "systemTableModel.h"
#include "assignmentModel.h"

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
    qmlRegisterType<AssignmentModel>("Crystal", 1, 0, "AssignmentModel");
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
            auto systemModelNode = dynamic_cast<SystemModelNode*>(modelNode.get());
            if(systemModelNode && systemModelNode->getAssignedModelNode()){
                SystemCambatInfo cambatInfo;
                cambatInfo.Phase = SystemCambatInfo::Fire;
                systemModelNode->setCambatInfo(cambatInfo);
            }
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
        systemStatusInfo.BCCStatus = SystemStatusInfo::S;
        systemStatusInfo.RadarSearchStatus = SystemStatusInfo::US;
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
            AircraftModelNode* aircraftModelNode = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][TN].get());
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
            StationModelNode* stationModelNode = dynamic_cast<StationModelNode*>(mModelNodes[STATION][number].get());
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
            SystemModelNode* systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][number].get());
            if(mSelectedModelNode)
                mSelectedModelNode->select(false);
            systemModelNode->onLeftButtonClicked(true);
            systemModelNode->goOnTrack();
            mSelectedModelNode = systemModelNode;
        }
    });

    connect(mMapController, &MapController::mapSRSChanged, this, &DefenseModelLayer::onClear);

    osgEarth::ModelLayer *systemsModelLayer = new osgEarth::ModelLayer();
    systemsModelLayer->setName(SYSTEMS_LAYER_NAME);
    mMapController->addLayer(systemsModelLayer);

    osgEarth::ModelLayer *stationsModelLayer = new osgEarth::ModelLayer();
    stationsModelLayer->setName(STATIONS_LAYER_NAME);
    mMapController->addLayer(stationsModelLayer);

    osgEarth::ModelLayer *aircraftsModelLayer = new osgEarth::ModelLayer();
    aircraftsModelLayer->setName(AIRCRAFTS_LAYER_NAME);
    mMapController->addLayer(aircraftsModelLayer);
}

void DefenseModelLayer::setDefenseDataManager(DefenseDataManager *defenseDataManager)
{
    mDefenseDataManager = defenseDataManager;
    //--aircraft--------------------------------------------------------
    QObject::connect(defenseDataManager, &DefenseDataManager::aircraftInfoChanged,this ,&DefenseModelLayer::onAircraftInfoChanged);
    QObject::connect(defenseDataManager, &DefenseDataManager::clearAircraft,this ,&DefenseModelLayer::onClearAircraft);
    QObject::connect(defenseDataManager, &DefenseDataManager::aircraftAssignedResponse,this ,&DefenseModelLayer::onAircraftAssignedResponse);
    //--system----------------------------------------------------------
    QObject::connect(defenseDataManager, &DefenseDataManager::systemInfoChanged,this ,&DefenseModelLayer::onSystemInfoChanged);
    QObject::connect(defenseDataManager, &DefenseDataManager::systemStatusInfoChanged,this ,&DefenseModelLayer::onSystemStatusInfoChanged);
    QObject::connect(defenseDataManager, &DefenseDataManager::systemCambatInfoChanged,this ,&DefenseModelLayer::onSystemCambatInfoChanged);
    //--station---------------------------------------------------------
    QObject::connect(defenseDataManager, &DefenseDataManager::stationInfoChanged,this ,&DefenseModelLayer::onStationInfoChanged);

}

void DefenseModelLayer::addUpdateAircraft(AircraftInfo aircraftInfo)
{
    osg::ref_ptr<AircraftModelNode> aircraftModelNode;
    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
                                          aircraftInfo.Longitude, aircraftInfo.Latitude, aircraftInfo.Altitude);

    if(mModelNodes.contains(AIRCRAFT) && mModelNodes[AIRCRAFT].contains(aircraftInfo.TN))
    {
        aircraftModelNode = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][aircraftInfo.TN].get());
        aircraftModelNode->flyTo(geographicPosition, aircraftInfo.Heading, aircraftInfo.Speed);

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
        //mMapController->addNode(aircraftModelNode);

        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(AIRCRAFTS_LAYER_NAME);
        if (layer) {
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if (group) {
                group->addChild(aircraftModelNode);
            }
        }

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

void DefenseModelLayer::addUpdateSystem(SystemInfo systemInfo)
{
    osg::ref_ptr<SystemModelNode> systemModelNode;
    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
                                          systemInfo.Longitude, systemInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mModelNodes.contains(SYSTEM) && mModelNodes[SYSTEM].contains(systemInfo.Number))
    {
        systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][systemInfo.Number].get());
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
        //mMapController->addNode(systemModelNode);

        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(SYSTEMS_LAYER_NAME);
        if (layer) {
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if (group) {
                group->addChild(systemModelNode);
            }
        }
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
        stationModelNode = dynamic_cast<StationModelNode*>(mModelNodes[STATION][stationInfo.Number].get());
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
        //mMapController->addNode(stationModelNode);

        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(STATIONS_LAYER_NAME);
        if (layer) {
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if (group) {
                group->addChild(stationModelNode);
            }
        }
    }
    //update information-----------------------------------------------------
    stationModelNode->setInformation(stationInfo);
    //add update list view-----------------------------------------------------------------
    if (mDataManager)
    {
        mDataManager->setStationInfo(stationInfo);
    }

}

void DefenseModelLayer::positionChanged(QString /*type*/, QString /*name*/, osgEarth::GeoPoint /*position*/)
{
}

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
        auto systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][systemStatusInfo.Number].get());
        //update information-----------------------------------------------------
        systemModelNode->setStatusInfo(systemStatusInfo);
        //add update list view-----------------------------------------------------------------
        if (mDataManager)
        {
            mDataManager->setSystemStatusInfo(systemStatusInfo);
        }
    }
}

void DefenseModelLayer::onSystemCambatInfoChanged(SystemCambatInfo &systemCambatInfo)
{
    if(mModelNodes.contains(SYSTEM) && mModelNodes[SYSTEM].contains(systemCambatInfo.Number))
    {
        auto systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][systemCambatInfo.Number].get());
        //update information-----------------------------------------------------
        systemModelNode->setCambatInfo(systemCambatInfo);
        //add update list view-----------------------------------------------------------------
        if (mDataManager)
        {
            mDataManager->setSystemCombatInfo(systemCambatInfo);
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
        auto aircraftModelNode = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][tn].get());
        aircraftModelNode->onLeftButtonClicked(false);
        aircraftModelNode->setNodeMask(false);
        //        mMapController->removeNode(aircraftModelNode);
        //        mModelNodes[AIRCRAFT].remove(tn);
        mDataManager->deleteAircraftInfo(tn);
    }
}

void DefenseModelLayer::onAircraftAssignedResponse(int tn, int systemNo, bool result)
{
//    qDebug()<<"onAircraftAssignedResponse:"<<tn<< ", "<< systemNo<<", "<<result;
    if(mModelNodes.contains(SYSTEM) && mModelNodes[SYSTEM].contains(systemNo))
    {
        auto systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][systemNo].get());
        if(systemModelNode)
            systemModelNode->acceptAssignedModelNode(result);
    }
//---if rejected then unassinment from aircraft----------------------------------------------------
    if(!result && mModelNodes.contains(AIRCRAFT) && mModelNodes[AIRCRAFT].contains(tn))
    {
        auto aircraftModelNode = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][tn].get());
        if(aircraftModelNode)
            aircraftModelNode->setAssignmentModelNode(nullptr);
    }
}

void DefenseModelLayer::onClear()
{
    mModelNodes.clear();
    mOnMoveModelNode = nullptr;
    mSelectedModelNode = nullptr;
    //--clear list-----------------------------------------
    //--add layer------------------------------------------
    osgEarth::ModelLayer *systemsModelLayer = new osgEarth::ModelLayer();
    systemsModelLayer->setName(SYSTEMS_LAYER_NAME);
    mMapController->addLayer(systemsModelLayer);

    osgEarth::ModelLayer *stationsModelLayer = new osgEarth::ModelLayer();
    stationsModelLayer->setName(STATIONS_LAYER_NAME);
    mMapController->addLayer(stationsModelLayer);

    osgEarth::ModelLayer *aircraftsModelLayer = new osgEarth::ModelLayer();
    aircraftsModelLayer->setName(AIRCRAFTS_LAYER_NAME);
    mMapController->addLayer(aircraftsModelLayer);
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
            aircraftModelNode->setAssignmentModelNode(systemModelNode);
            //--TODO manage memory---------------------------------------
            std::thread* t1 = new std::thread([=](){
                if(mDefenseDataManager)
                    emit mDefenseDataManager->aircraftAssigned(aircraftModelNode->getInformation().TN,
                                                          systemModelNode->getInformation().Number);
            });
        }
        mMapController->removeNode(mDragAircraftModelNode);
        mDragAircraftModelNode = nullptr;
    }
}

void DefenseModelLayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        auto aircraftModelNode  = dynamic_cast<AircraftModelNode*>(mSelectedModelNode);
        if(aircraftModelNode)
        {
            auto systemModelNode = aircraftModelNode->getAssignmentModelNode();
            if(systemModelNode)
            {
                emit mDefenseDataManager->cancelAircraftAssigned(aircraftModelNode->getInformation().TN,
                        systemModelNode->getInformation().Number);
                systemModelNode->unassignedModelNode();
                event->accept();
            }
        }
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

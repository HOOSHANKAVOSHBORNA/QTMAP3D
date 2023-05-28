#include "defenseModelLayer.h"
#include "draw.h"
#include "truck.h"
#include "rocket.h"
#include "systemModelNode.h"
#include "stationModelNode.h"
#include "mapItem.h"
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
#include "aircraftInfoItem.h"
#include "stationTableModel.h"
#include "stationInformation.h"
#include "systemInfoItem.h"
#include "systemTableModel.h"
#include "dataManager.h"

const QString AIRCRAFT = "Aircraft";
const QString SYSTEM = "System";
const QString STATION = "Station";
//----------------------------------------------
const QString CATEGORY = "Defense Model";
const QString ADD_ROCKET = "Fire";
const QString KILL_ROCKET = "Kill";

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

    pDesc->toolboxItemsList.push_back(new ItemDesc{AIRCRAFT, CATEGORY, "qrc:/resources/plane.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{SYSTEM, CATEGORY, "qrc:/resources/systems.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{STATION, CATEGORY, "qrc:/resources/stations.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{ADD_ROCKET, CATEGORY, "qrc:/resources/system_1.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{KILL_ROCKET, CATEGORY, "qrc:/resources/system_1.png", false, false, ""});
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
        aircraftInfo.TN = 0;
        aircraftInfo.Longitude = 52.8601;
        aircraftInfo.Latitude = 35.277;
        aircraftInfo.Altitude = 9100;
        aircraftInfo.Heading = 30;
        aircraftInfo.IFFCode = "a12345";
        aircraftInfo.CallSign = "cls";
        aircraftInfo.Type = AircraftInfo::AircraftType::Fighter;
        aircraftInfo.MasterRadar = "radar2";
        aircraftInfo.Identification = AircraftInfo::Identify::Z;
        aircraftInfo.IdentificationMethod = "mtd";
        aircraftInfo.Time = 16675478754;
        aircraftInfo.Pos = "pos1";
        mDataManager->onAircraftInfoChanged(aircraftInfo);
        //addUpdateAircraft(aircraftInfo);
        //demo();
    }
    if(CATEGORY == category && name == ADD_ROCKET)
    {
//        for(auto modelNode:mModelNodes[AIRCRAFT])
//        {
//            auto aircrafModelNode = dynamic_cast<AircraftModelNode*>(modelNode.get());
//            if(aircrafModelNode)
//            {
//                if(aircrafModelNode->getAssignments().size() > 0)
//                {
//                    auto systemModelNode = aircrafModelNode->getAssignments().first();
//                    if(systemModelNode){
//                        SystemCombatInfo CombatInfo;
//                        CombatInfo.Phase = SystemCombatInfo::Fire;
//                        CombatInfo.TN = aircrafModelNode->getInformation().TN;
//                        systemModelNode->setCombatInfo(CombatInfo);
//                    }
//                }
//            }
//        }
    }
    else if(CATEGORY == category && name == KILL_ROCKET)
    {
//        for(auto modelNode:mModelNodes[AIRCRAFT])
//        {
//            auto aircrafModelNode = dynamic_cast<AircraftModelNode*>(modelNode.get());
//            if(aircrafModelNode)
//            {
//                if(aircrafModelNode->getAssignments().size() > 0)
//                {
//                    auto systemModelNode = aircrafModelNode->getAssignments().first();
//                    if(systemModelNode){
//                        SystemCombatInfo CombatInfo;
//                        CombatInfo.Phase = SystemCombatInfo::Kill;
//                        CombatInfo.TN = aircrafModelNode->getInformation().TN;
//                        systemModelNode->setCombatInfo(CombatInfo);
//                    }
//                }
//            }
//        }
    }
    else if(CATEGORY == category && name == SYSTEM)
    {
        SystemInfo systemInfo;
        systemInfo.Name = SYSTEM + QString::number(0);
        systemInfo.Longitude = 54.2;
        systemInfo.Latitude = 35.3;
        systemInfo.Number = 1234567;
//        addUpdateSystem(systemInfo);
        mDataManager->onSystemInfoChanged(systemInfo);

        SystemStatusInfo systemStatusInfo;
        systemStatusInfo.Number = systemInfo.Number;
        systemStatusInfo.BCCStatus = SystemStatusInfo::S;
        systemStatusInfo.RadarSearchStatus = SystemStatusInfo::US;
        systemStatusInfo.MissileCount = 3;
        mDataManager->onSystemStatusInfoChanged(systemStatusInfo);

    }
    else if(CATEGORY == category && name == STATION)
    {
        StationInfo stationInfo;
        stationInfo.Name = STATION + QString::number(0);
        stationInfo.Longitude = 52;
        stationInfo.Latitude = 35.2;
        stationInfo.Number = 1234567;
        stationInfo.PrimSec = "primary";
        stationInfo.CycleTime = 10000;
        mDataManager->onStationInfoChanged(stationInfo);
//        addUpdateStation(stationInfo);
    }
}

bool DefenseModelLayer::setup(MapItem *mapItem,
                              UIHandle *uiHandle)
{
    mMapController = mapItem;
    mUIHandle = uiHandle;

    connect(mMapController, &MapItem::mapCleared, this, &DefenseModelLayer::onMapClear);

    osgEarth::ModelLayer *systemsModelLayer = new osgEarth::ModelLayer();
    systemsModelLayer->setName(SYSTEMS_LAYER_NAME);
    mMapController->addLayer(systemsModelLayer);

    osgEarth::ModelLayer *stationsModelLayer = new osgEarth::ModelLayer();
    stationsModelLayer->setName(STATIONS_LAYER_NAME);
    mMapController->addLayer(stationsModelLayer);

    osgEarth::ModelLayer *aircraftsModelLayer = new osgEarth::ModelLayer();
    aircraftsModelLayer->setName(AIRCRAFTS_LAYER_NAME);
    mMapController->addLayer(aircraftsModelLayer);

    return true;
}

void DefenseModelLayer::setDefenseDataManager(DefenseDataManager *defenseDataManager)
{
//    auto listManager = new ListManager(mQmlEngine, mUIHandle, this);
    mDataManager = new DataManager(defenseDataManager, this);
}

//void DefenseModelLayer::addUpdateAircraft(AircraftInfo aircraftInfo)
//{
////    osg::ref_ptr<AircraftModelNode> aircraftModelNode;
////    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
////                                          aircraftInfo.Longitude, aircraftInfo.Latitude, aircraftInfo.Altitude);

////    if(mModelNodes.contains(AIRCRAFT) && mModelNodes[AIRCRAFT].contains(aircraftInfo.TN))
////    {
////        aircraftModelNode = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][aircraftInfo.TN].get());
////        aircraftModelNode->flyTo(geographicPosition, aircraftInfo.Heading, aircraftInfo.Speed);

////    }
////    else
////    {
////        //create and model node------------------------------------------------
////        aircraftModelNode = new AircraftModelNode(mMapController, aircraftInfo.Type, mQmlEngine,mUIHandle);
////        aircraftModelNode->setQStringName(QString::number(aircraftInfo.TN));
////        aircraftModelNode->setGeographicPosition(geographicPosition, aircraftInfo.Heading);

////        //add to container-----------------------------------------------------
////        mModelNodes[AIRCRAFT][aircraftInfo.TN] = aircraftModelNode;
////        //add to map ---------------------------------------------------------
////        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(AIRCRAFTS_LAYER_NAME);
////        if (layer) {
////            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
////            if (group) {
////                group->addChild(aircraftModelNode);
////            }
////        }
////    }
////    //update information------------------------------------------------------------------
////    aircraftModelNode->setInformation(aircraftInfo);

//}

//void DefenseModelLayer::addUpdateSystem(SystemInfo systemInfo)
//{
//    osg::ref_ptr<SystemModelNode> systemModelNode;
//    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
//                                          systemInfo.Longitude, systemInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
//    if(mModelNodes.contains(SYSTEM) && mModelNodes[SYSTEM].contains(systemInfo.Number))
//    {
//        systemModelNode = dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][systemInfo.Number].get());
//    }
//    else
//    {
//        //create and setting model-------------------------------------------
//        systemModelNode = new SystemModelNode(mMapController, mQmlEngine, mUIHandle);
//        systemModelNode->setQStringName(systemInfo.Name);
//        systemModelNode->setGeographicPosition(geographicPosition, 0.0);
//        //add to container---------------------------------------------------
//        mModelNodes[SYSTEM][systemInfo.Number] = systemModelNode;
//        //add to map --------------------------------------------------------
//        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(SYSTEMS_LAYER_NAME);
//        if (layer) {
//            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
//            if (group) {
//                group->addChild(systemModelNode);
//            }
//        }
//    }
//    //update information-----------------------------------------------------
//    systemModelNode->setInformation(systemInfo);
//}

//void DefenseModelLayer::addUpdateStation(StationInfo stationInfo)
//{
//    osg::ref_ptr<StationModelNode> stationModelNode;
//    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
//                                          stationInfo.Longitude, stationInfo.Latitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
//    if(mModelNodes.contains(STATION) && mModelNodes[STATION].contains(stationInfo.Number))
//    {
//        stationModelNode = dynamic_cast<StationModelNode*>(mModelNodes[STATION][stationInfo.Number].get());
//    }
//    else
//    {
//        //create and setting model-------------------------------------------
//        stationModelNode = new StationModelNode(mMapController, mQmlEngine, mUIHandle);
//        stationModelNode->setQStringName(stationInfo.Name);
//        stationModelNode->setGeographicPosition(geographicPosition, 0.0);
//        //add to container---------------------------------------------------
//        mModelNodes[STATION][stationInfo.Number] = stationModelNode;
//        //add to map --------------------------------------------------------
//        auto layer = mMapController->getMapNode()->getMap()->getLayerByName(STATIONS_LAYER_NAME);
//        if (layer) {
//            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
//            if (group) {
//                group->addChild(stationModelNode);
//            }
//        }
//    }
//    //update information-----------------------------------------------------
//    stationModelNode->setInformation(stationInfo);

//}

//SystemModelNode *DefenseModelLayer::getSystemModelNode(int number) const
//{
//    if(mModelNodes.contains(SYSTEM) && mModelNodes[SYSTEM].contains(number))
//    {
//        return  dynamic_cast<SystemModelNode*>(mModelNodes[SYSTEM][number].get());
//    }
//    return nullptr;
//}

//AircraftModelNode *DefenseModelLayer::getAircraftModelNode(int tn) const
//{
//    if(mModelNodes.contains(AIRCRAFT) && mModelNodes[AIRCRAFT].contains(tn))
//    {
//        return  dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][tn].get());
//    }
//    return nullptr;
//}

//StationModelNode *DefenseModelLayer::getStationModelNode(int number) const
//{
//    if(mModelNodes.contains(STATION) && mModelNodes[STATION].contains(number))
//    {
//        return  dynamic_cast<StationModelNode*>(mModelNodes[STATION][number].get());
//    }
//    return nullptr;
//}

void DefenseModelLayer::selectModelNode(DefenseModelNode *defenseModelNode)
{
    AircraftModelNode* aircraftModelNode = dynamic_cast<AircraftModelNode*>(defenseModelNode);
    if(aircraftModelNode){
        if(mSelectedModelNode) {
            //mSelectedModelNode->select(false);
            mSelectedModelNode->setSelectionMode(DefenseModelNode::SelectionMode::UNSELECTED);
            mSelectedModelNode->updateColors();
        }
        mSelectedModelNode = aircraftModelNode;
        aircraftModelNode->onLeftButtonClicked(true);
        aircraftModelNode->goOnTrack();
        mSelectedModelNode->setSelectionMode(DefenseModelNode::SelectionMode::SELECTED);
        mSelectedModelNode->updateColors();
    }

    StationModelNode* stationModelNode = dynamic_cast<StationModelNode*>(defenseModelNode);
    if(stationModelNode){
        if(mSelectedModelNode) {
            //mSelectedModelNode->select(false);
            mSelectedModelNode->setSelectionMode(DefenseModelNode::SelectionMode::UNSELECTED);
            mSelectedModelNode->updateColors();
        }
        mSelectedModelNode = stationModelNode;
        stationModelNode->onLeftButtonClicked(true);
        stationModelNode->goOnTrack();
        mSelectedModelNode->setSelectionMode(DefenseModelNode::SelectionMode::SELECTED);
        mSelectedModelNode->updateColors();
    }

    SystemModelNode* systemModelNode = dynamic_cast<SystemModelNode*>(defenseModelNode);
    if(systemModelNode){
        if(mSelectedModelNode) {
            //mSelectedModelNode->select(false);
            mSelectedModelNode->setSelectionMode(DefenseModelNode::SelectionMode::UNSELECTED);
            mSelectedModelNode->updateColors();
        }
        mSelectedModelNode = systemModelNode;
        systemModelNode->onLeftButtonClicked(true);
        systemModelNode->goOnTrack();
        mSelectedModelNode->setSelectionMode(DefenseModelNode::SelectionMode::SELECTED);
        mSelectedModelNode->updateColors();
    }
}

//void DefenseModelLayer::clearAircraft(int tn)
//{
////    auto aircraftModelNode = getAircraftModelNode(tn);
////    if(aircraftModelNode){
////        aircraftModelNode->onLeftButtonClicked(false);
////        aircraftModelNode->setNodeMask(false);
////    }
//}

void DefenseModelLayer::onMapClear()
{
//    mModelNodes.clear();
    mOnMoveModelNode = nullptr;
    mSelectedModelNode = nullptr;
    //--clear list-----------------------------------------
    mDataManager->clear();
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
    for(auto data: mDataManager->aircraftDataManager()->getAircraftsData())
        if(data->modelNode.valid())
            data->modelNode->frameEvent();
    for(auto data: mDataManager->systemDataManager()->getSystemsData())
        if(data->systemModelNode.valid())
            data->systemModelNode->frameEvent();
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
        auto systemModelNode  = dynamic_cast<SystemModelNode*>(mOnMoveModelNode.get());
        if(systemModelNode)
        {
            auto aircraftModelNode  = dynamic_cast<AircraftModelNode*>(mSelectedModelNode.get());
            mDataManager->assignAircraft2System(aircraftModelNode->getData().info.TN, systemModelNode->getData()->information->systemInfo.Number);
        }
        mMapController->removeNode(mDragAircraftModelNode);
        mDragAircraftModelNode = nullptr;
    }
}

void DefenseModelLayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        auto aircraftModelNode  = dynamic_cast<AircraftModelNode*>(mSelectedModelNode.get());
        if(aircraftModelNode /*&& aircraftModelNode->hasAssignment()*/)
        {
            mDataManager->cancelAircraftAssignments(aircraftModelNode->getData().info.TN);
            event->accept();
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
//    int range = static_cast<int>(camera->getViewpoint().getRange());
//    if(range != mPreCameraRange && range < 12000)
//    {
//        mPreCameraRange = range;
        osg::Viewport* viewport = viewer->getCamera()->getViewport();
        osg::ref_ptr<osgUtil::PolytopeIntersector> intersector{nullptr};
        intersector = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, viewport->x(), viewport->y(),
                                                       viewport->x() + viewport->width(), viewport->y() + viewport->height());

        intersector->setPrimitiveMask(osgUtil::PolytopeIntersector::TRIANGLE_PRIMITIVES);
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
//                        double distance = 0;
//                        if(camera->getViewpoint().getRange() < defenseModelNode->getPosition().z())///for track node
//                            distance = camera->getViewpoint().getRange();
//                        else
//                            distance = camera->getViewpoint().getRange() - defenseModelNode->getPosition().z();
                        //                        model->cameraRangeChanged(distance);
                        //qDebug() <<"camera->getViewpoint().getRange(): "<<camera->getViewpoint().getRange();
                        //qDebug() <<"model.getRange(): "<<camera->getViewpoint().getRange() - model->getPosition().z();
                        qDebug()<<defenseModelNode->getQStringName();
                    }
                }
            }

        }
//    }
}

osgEarth::Symbology::Style &DefenseModelLayer::getDefaultStyle()
{
    static osgEarth::Symbology::Style _style;
    static bool bFirst = true;
    if (bFirst) {
        static osg::Node *node = new osg::Node;
        _style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);
        bFirst = false;
    }

    return _style;
}


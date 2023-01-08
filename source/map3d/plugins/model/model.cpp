#include "model.h"
#include "draw.h"
#include "truck.h"
#include "rocket.h"
#include "system.h"
#include "stationmodelnode.h"
#include "mapcontroller.h"
#include "networkmanager.h"
#include "websocketclient.h"

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


#include "aircrafttablemodel.h"
#include "aircraftcontextmenumodel.h"
#include "aircraftInformation.h"
#include "stationtablemodel.h"
#include "stationinformation.h"
#include "systeminformation.h"

//const QString FLYING = "Flying";
const QString AIRCRAFT = "Aircraft";
const QString SYSTEM = "System";
const QString STATION = "Station";
const QString ROCKET = "Rocket";
const QString TRUCK = "Truck";
//----------------------------------------------
const QString CATEGORY = "Model";
const QString ADD_AIRCRAFT = "Add Aircraft";
const QString ADD_ROCKET = "Add Rocket";
const QString ADD_TRUCK = "Add Truck";
const QString ADD_STATION = "Add Station";
const QString ADD_SYSTEM = "Add System";

Model::Model(QObject *parent)
    : PluginInterface(parent)
{
    //    Q_INIT_RESOURCE(modelqml);
    Q_INIT_RESOURCE(model);
    Q_INIT_RESOURCE(modelqml);
}

bool Model::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *pDesc)
{
    //    pDesc->pluginHasSideItem = true;
    //    pDesc->sideItemMenuBarTitle = "Layers";
    //    pDesc->sideItemMenuBarIconUrl = "qrc:///test1plugin/resources/Layers.png";
    //    pDesc->sideItemUrl = "qrc:///test1plugin/Layers.qml";

    qmlRegisterType<AircraftTableModel>("Crystal", 1, 0, "AircraftTableModel");
    qmlRegisterType<AircraftContextMenumodel>("Crystal", 1, 0, "AircraftContextMenumodel");
    qmlRegisterType<AircraftInfoModel>("Crystal", 1, 0, "AircraftInfoModel");
    qmlRegisterType<StationTableModel>("Crystal", 1, 0, "StationTableModel");
    qmlRegisterType<StationInfoModel>("Crystal", 1, 0, "StationInfoModel");
    qmlRegisterType<SystemInfoModel>("Crystal", 1, 0, "SystemInfoModel");
    mQmlEngine = engine;

    QString cat = "model";
    pDesc->toolboxItemsList.push_back(new ItemDesc{ADD_AIRCRAFT, CATEGORY, "qrc:/resources/airplan.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{ADD_SYSTEM, CATEGORY, "qrc:/resources/system_1.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{ADD_STATION, CATEGORY, "qrc:/resources/station_lV.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{ADD_TRUCK, CATEGORY, "qrc:/resources/truck.png", false, false, ""});
    pDesc->toolboxItemsList.push_back(new ItemDesc{ADD_ROCKET, CATEGORY, "", false, false, ""});
    return true;
}

void Model::onSideItemCreated(int /*index*/, QObject */*pSideItem*/)
{
}

void Model::onToolboxItemClicked(const QString &name, const QString &category)
{
    if(CATEGORY == category && name == ADD_AIRCRAFT)
    {
        AircraftInfo aircraftInfo;
        aircraftInfo.TN = AIRCRAFT + QString::number(mModelNodes[AIRCRAFT].count());
        aircraftInfo.Latitude = 52.8601;
        aircraftInfo.Longitude = 35.277;
        aircraftInfo.Altitude = 9100;
        aircraftInfo.Heading = 30;
        addUpdateAircraft(aircraftInfo);
        //demo();
    }
    if(CATEGORY == category && name == ADD_ROCKET)
    {
        // fallow racket
        if(!mModelNodes[AIRCRAFT].isEmpty())
        {
            auto truckNames = mModelNodes[TRUCK].keys();
            for(auto truckName: truckNames)
            {
                auto modeltruck = dynamic_cast<Truck*>(mModelNodes[TRUCK][truckName]);
                if(modeltruck->hasRocket())
                {
                    //                        addRocketModel(modeltruck->getPosition().vec3d());
                    //                        auto modelRocket = dynamic_cast<Rocket*>(mModels[ROCKET].last());
                    auto activeRocket = modeltruck->getActiveRocket();
                    auto modelAirplane = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT].last());
                    modelAirplane->stop();//
                    activeRocket->setFollowModel(modelAirplane);
                    //modelRocket->setTruckModel(modeltruck);

                    //                        modeltruck->aimTarget(modelAirplane->getPosition().vec3d());

                    modelAirplane->setFollowModel(activeRocket);
                    modelAirplane->setTruckModel(modeltruck);

                    modeltruck->shoot(modelAirplane->getPosition().vec3d(), 2000);//1000 m/s
                    //                        activeRocket->setPosition(modeltruck->getPosition());
                    //                        mMap3dWidget->addNode(activeRocket);
                    //                        activeRocket->shoot(modelAirplane->getPosition().vec3d(), 3000);

                    mMapController->setTrackNode(activeRocket->getGeoTransform());

                    //draw line
                    //                        osg::Vec3d truckPosition;
                    //                        modeltruck->getPosition().toWorld(truckPosition);
                    //                        osg::Vec3d wPoint1;
                    //                        modelAirplane->getPosition().toWorld(wPoint1);
                    //                        osg::Vec3Array* keyPoint = new osg::Vec3Array;
                    //                        keyPoint->push_back(truckPosition + osg::Vec3d(5, 0, -2.6));
                    //                        keyPoint->push_back(wPoint1);
                    //                        mMap3dWidget->mMapRoot->addChild(drawLine(keyPoint, 1.0));
                    break;
                }
            }
        }
    }
    else if(CATEGORY == category && name == ADD_TRUCK)
    {
        addTruckModel();
    }
    else if(CATEGORY == category && name == ADD_SYSTEM)
    {
        osg::Vec3d position(52.9, 35.3, 842.5);
        addSystemModel(position);
    }
    else if(CATEGORY == category && name == ADD_STATION)
    {
        StationInfo stationInfo;
        stationInfo.Name = STATION + QString::number(mModelNodes[STATION].count());
        stationInfo.Latitude = 53;
        stationInfo.Longitude = 35.2;
        addUpdateStation(stationInfo);
    }
}

bool Model::setup(MapController *mapController,
                  NetworkManager *networkManager,
                  UIHandle *uiHandle)
{
    mMapController = mapController;
    mUIHandle = uiHandle;

    mDataManager = new DataManager(mQmlEngine, mUIHandle, this);
    connect(mDataManager, &DataManager::aircraftDoubleClicked,[=](const QString& TN){

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
    connect(mDataManager, &DataManager::stationDoubleClicked,[=](const QString& Name){

        if(mModelNodes[STATION].contains(Name))
        {
            StationModelNode* stationModelNode = dynamic_cast<StationModelNode*>(mModelNodes[STATION][Name]);
            if(mSelectedModelNode)
                mSelectedModelNode->select(false);
            stationModelNode->onLeftButtonClicked(true);
            stationModelNode->goOnTrack();
            mSelectedModelNode = stationModelNode;
        }
    });

    ////--websocket data-------------------------------------------------------------------
    QObject::connect(networkManager->webSocketClient(), &WebSocketClient::messageReceived,this ,&Model::onMessageReceived);
}

void Model::demo()
{
    //    int index = 0;
    auto airplaneNames = mModelNodes[AIRCRAFT].keys();
    for (auto name: airplaneNames)
    {
        auto model = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][name]);
        auto mapPoint = model->getPosition();
        osgEarth::GeoPoint  latLongPoint;
        //latLongPoint.altitudeMode() = osgEarth::AltitudeMode::ALTMODE_ABSOLUTE;
        //mapPoint.transform(osgEarth::SpatialReference::get("wgs84"), latLongPoint);
        osg::Vec3d currentPos;
        mapPoint.toWorld(currentPos);
        //osg::Vec3d currentPos(latLongPoint.vec3d());
        //-- 500 km/h ~ 139 m/s ------------------
        //    int randomX = 10*(138 + (qrand() % 139));
        //    int randomY = 10*(138 + (qrand() % 139));
        //    int val = qrand() % 4;
        //    if(val == 1)
        //        currentPos += osg::Vec3d(randomX, randomY, 0.0);
        //    else if(val == 2)
        //        currentPos += osg::Vec3d(randomX, -randomY, 0.0);
        //    else if(val == 3)
        //        currentPos += osg::Vec3d(-randomX, randomY, 0.0);
        //    else
        //        currentPos += osg::Vec3d(-randomX, -randomY, 0.0);

        int randomX = (100 + (qrand() % 19));
        int randomY = (100 + (qrand() % 19));
        int val = qrand() % 4;
        if(val == 1)
            mapPoint.vec3d() += osg::Vec3d(randomX/10000.0, randomY/10000.0, 0.0);
        else if(val == 2)
            mapPoint.vec3d() += osg::Vec3d(randomX/10000.0, -randomY/10000.0, 0.0);
        else if(val == 3)
            mapPoint.vec3d() += osg::Vec3d(-randomX/10000.0, randomY/10000.0, 0.0);
        else
            mapPoint.vec3d() += osg::Vec3d(-randomX/10000.0, -randomY/10000.0, 0.0);

        //    setPosition(mCurrentWorldPoint, 138);
        latLongPoint.fromWorld(osgEarth::SpatialReference::get("wgs84"), currentPos);
        //qDebug()<<QString::fromUtf8(latLongPoint.toString().c_str());
        model->flyTo(mapPoint.vec3d(),30, 138);

        // fallow racket
        //        auto truckNames = mModels[TRUCK].keys();
        //        for(auto truckName: truckNames)
        //        {
        //            auto modeltruck = dynamic_cast<Truck*>(mModels[TRUCK][truckName]);
        //            if(modeltruck->shoot())
        //            {
        //                addRocketModel(modeltruck->getPosition().vec3d());
        //                auto modelRocket = dynamic_cast<FlyingModel*>(mModels[ROCKET].last());
        //                modelRocket->flyTo(mapPoint.vec3d(), 120);
        //                model->setFollowingModel(modelRocket);
        //                break;
        //            }
        //        }
        //        index += 1;
    }
}

void Model::addTruckModel()
{
    osgEarth::GeoPoint position(mMapController->getMapSRS()->getGeographicSRS(),52.8603, 35.274, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    //create and setting model--------------------------------------------
    osg::ref_ptr<Truck> model = new Truck(mMapController->getMapNode());
    QString name = TRUCK + QString::number(mModelNodes[TRUCK].count());
    model->setName(name.toStdString());
    model->setGeographicPosition(position, 0.0);
    //model->setLocalRotation(osg::Quat(osg::inDegrees(-30.0),osg::Z_AXIS));
    model->setScale(osg::Vec3(1,1,1));



    //    QObject::connect(model.get(), &FlyingModel::positionChanged, [=](osgEarth::GeoPoint position){
    //        positionChanged(ROCKET, name, position);
    //    });

    //add to container-----------------------------------------------------
    mModelNodes[TRUCK][name] = model;

    //add to map ---------------------------------------------------------
    mMapController->addNode(model);
    mMapController->goToPosition(position.x(), position.y(), position.z() + 600);

    //create random position ---------------------------------------------
    //    double rnd = QRandomGenerator::global()->generateDouble();
    double rndRotate = qrand() % 360;
    model->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(rndRotate), osg::Z_AXIS));

    double rndPX = (qrand() % 200)/100000.0;
    double rndPY = (qrand() % 200)/100000.0;
    osg::Vec3d nPosition(rndPX, rndPY, 0.0);


//    nPosition += position;
    //model->setLatLongPosition(nPosition);
    //mMap3dWidget->goPosition(nPosition.x(), nPosition.y(), nPosition.z() + 500);
    //move
    //model->moveTo(nPosition,10);
}

void Model::addUpdateAircraft(AircraftInfo aircraftInfo)
{
    osg::ref_ptr<AircraftModelNode> aircraftModelNode;
    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
                                          aircraftInfo.Latitude, aircraftInfo.Longitude, aircraftInfo.Altitude);

    if(mModelNodes.contains(AIRCRAFT) && mModelNodes[AIRCRAFT].contains(aircraftInfo.TN))
    {
        aircraftModelNode = dynamic_cast<AircraftModelNode*>(mModelNodes[AIRCRAFT][aircraftInfo.TN]);
        aircraftModelNode->flyTo(geographicPosition.vec3d(), aircraftInfo.Heading, aircraftInfo.Speed);

    }
    else
    {
        //create and model node------------------------------------------------
        aircraftModelNode = new AircraftModelNode(mMapController, mQmlEngine,mUIHandle);
        aircraftModelNode->setQStringName(aircraftInfo.TN);
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
    }
    //update information------------------------------------------------------------------
    aircraftModelNode->setInformation(aircraftInfo);
    //add update list view-----------------------------------------------------------------
    if (mDataManager)
    {
        mDataManager->setAircraftInfo(aircraftInfo);
    }

}

void Model::addRocketModel(osg::Vec3d position)
{
    osgEarth::GeoPoint geoposition(mMapController->getMapSRS()->getGeographicSRS(),position);
    //create and setting model--------------------------------------------
    osg::ref_ptr<Rocket> model = new Rocket(mMapController->getMapNode());
    QString name = ROCKET + QString::number(mModelNodes[ROCKET].count());
    model->setName(name.toStdString());
    model->setGeographicPosition(geoposition, 0.0);
    model->setScale(osg::Vec3(1,1,1));

    QObject::connect(model.get(), &BaseModel::positionChanged, [=](osgEarth::GeoPoint position){
        positionChanged(ROCKET, name, position);
    });

    //add to container-----------------------------------------------------
    mModelNodes[ROCKET][name] = model;


    //add to map ---------------------------------------------------------
    mMapController->addNode(model);
    //mMap3dWidget->goPosition(position.x(), position.y(), position.z() + 500);
    mMapController->setTrackNode(model->getGeoTransform());
    //    mMap3dWidget->setObjectInfoWidgetVisible(false);

    //    double rnd = QRandomGenerator::global()->generateDouble();
    //    double rnd = qrand() % 360;
    //    model->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(rnd), osg::Z_AXIS));
    //hit------------------------------------------------------------------
    QObject::connect(model.get(), &BaseModel::hit, [=](BaseModel */*other*/){

        mModelNodes[ROCKET].remove(QString(model->getName().c_str()));
    });
}

void Model::addSystemModel(osg::Vec3d position)
{
    osgEarth::GeoPoint geoposition(mMapController->getMapSRS()->getGeographicSRS(),position);
    //create and setting model--------------------------------------------
    osg::ref_ptr<System> model = new System(mMapController);
    QString name = SYSTEM + QString::number(mModelNodes["System"].count());
    model->setQStringName(name);
    model->setGeographicPosition(geoposition, 0.0);
    model->setScale(osg::Vec3(1,1,1));
    //add to container-----------------------------------------------------
    mModelNodes[SYSTEM][name] = model;


    //add to map ---------------------------------------------------------
    mMapController->addNode(model);
}

void Model::addUpdateStation(StationInfo stationInfo)
{
    osg::ref_ptr<StationModelNode> stationModelNode;
    osgEarth::GeoPoint geographicPosition(mMapController->getMapSRS()->getGeographicSRS(),
                                          stationInfo.Latitude, stationInfo.Longitude, 0, osgEarth::AltitudeMode::ALTMODE_RELATIVE);
    if(mModelNodes.contains(STATION) && mModelNodes[STATION].contains(stationInfo.Name))
    {
        stationModelNode = dynamic_cast<StationModelNode*>(mModelNodes[STATION][stationInfo.Name]);
    }
    else
    {
        //create and setting model-------------------------------------------
        stationModelNode = new StationModelNode(mMapController, mQmlEngine, mUIHandle);
        stationModelNode->setQStringName(stationInfo.Name);
        stationModelNode->setGeographicPosition(geographicPosition, 0.0);
        //add to container---------------------------------------------------
        mModelNodes[STATION][stationInfo.Name] = stationModelNode;
        //add to map --------------------------------------------------------
        mMapController->addNode(stationModelNode);
    }
    //update information-----------------------------------------------------
    stationModelNode->setInformation(stationInfo);
    //    //add update list view-----------------------------------------------------------------
        if (mDataManager)
        {
            mDataManager->setStationInfo(stationInfo);
        }

}

void Model::clickedTrackNode(QString type, QString name, bool isClick)
{
    if (isClick){
        //        osg::Node* node =mModels[type][name];
        mMapController->setTrackNode(mModelNodes[type][name]->getGeoTransform());
        //qDebug()<<"fgd";

        //        if(type == AIRPLANE) {
        //            mMap3dWidget->setObjectInfoWidgetVisible(true);
        //            mMap3dWidget->setSelectedAirplane(mModels[type][name]);
        //        } else {
        //            mMap3dWidget->setObjectInfoWidgetVisible(false);
        //        }

    } else {

        mMapController->untrackNode();
        //        mMap3dWidget->setObjectInfoWidgetVisible(false);

    }
}

void Model::positionChanged(QString /*type*/, QString /*name*/, osgEarth::GeoPoint /*position*/)
{
}

void Model::onMessageReceived(const QJsonDocument &message)
{
    if(message.object().value("Name").toString() == "Aircraft")
    {
        QJsonObject data = message.object().value("Data").toObject();
        AircraftInfo aircraftInfo;
        aircraftInfo.fromJson(QJsonDocument(data));
        //qDebug()<<"target:"<< data;
        addUpdateAircraft(aircraftInfo);
    }
    if(message.object().value("Name").toString() == "Station")
    {
        QJsonObject data = message.object().value("Data").toObject();
        StationInfo stationInfo;
        stationInfo.fromJson(QJsonDocument(data));
//        qDebug()<<"station:"<< data;
        addUpdateStation(stationInfo);
    }

}

void Model::frameEvent()
{
    //    findSceneModels(mMapController->getViewer());
    for(auto modelNodeList: mModelNodes)
        for(auto modelNode: modelNodeList)
            modelNode->frameEvent();
    //    if(mLastSelectedModel)
    //        mLastSelectedModel->frameEvent();
}

void Model::mousePressEvent(QMouseEvent *event)
{

    BaseModel* modelNode = pick(event->x(), event->y());
    if(modelNode)
    {
        modelNode->mousePressEvent(event, true);
    }
    if(mSelectedModelNode && mSelectedModelNode != modelNode)
        mSelectedModelNode->mousePressEvent(event, false);
    if(modelNode)
        mSelectedModelNode = modelNode;

}

void Model::mouseMoveEvent(QMouseEvent *event)
{
    BaseModel* modelNode = pick(event->x(), event->y());
    if(modelNode)
    {
        modelNode->mouseMoveEvent(event, true);
    }
    if(mOnMoveModelNode && mOnMoveModelNode != modelNode)
        mOnMoveModelNode->mouseMoveEvent(event, false);
    if(modelNode)
        mOnMoveModelNode = modelNode;
}

BaseModel* Model::pick(float x, float y)
{
    BaseModel* model = nullptr;
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
                model = dynamic_cast<BaseModel*>(*nitr);
                if (model)
                    return model;
            }
        }
    }
    return model;
}
void Model::findSceneModels(osgViewer::Viewer *viewer)
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
                    BaseModel* model = dynamic_cast<BaseModel*>(*nitr);
                    if (model)
                    {
                        //qDebug() <<model->getQStringName();
                        //qDebug() <<"range: "<<camera->getViewpoint().getRange();
                        //qDebug() <<"z: "<<model->getPosition().z();
                        double distance = 0;
                        if(camera->getViewpoint().getRange() < model->getPosition().z())///for track node
                            distance = camera->getViewpoint().getRange();
                        else
                            distance = camera->getViewpoint().getRange() - model->getPosition().z();
                        //                        model->cameraRangeChanged(distance);
                        //qDebug() <<"camera->getViewpoint().getRange(): "<<camera->getViewpoint().getRange();
                        //qDebug() <<"model.getRange(): "<<camera->getViewpoint().getRange() - model->getPosition().z();
                    }
                }
            }

        }
    }
}

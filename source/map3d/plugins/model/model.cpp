#include "model.h"
#include "draw.h"
#include "truck.h"
#include "rocket.h"
#include "system.h"
#include "station.h"
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

//const QString FLYING = "Flying";
const QString AIRCRAFT = "Aircraft";
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
        osg::Vec3d position(52.8601, 35.277, 9100);
        QString name = AIRCRAFT + QString::number(mModels[AIRCRAFT].count());
        addAircraftModel(name, position, -30);
        //demo();
    }
    if(CATEGORY == category && name == ADD_ROCKET)
    {
        // fallow racket
        if(!mModels[AIRCRAFT].isEmpty())
        {
            auto truckNames = mModels[TRUCK].keys();
            for(auto truckName: truckNames)
            {
                auto modeltruck = dynamic_cast<Truck*>(mModels[TRUCK][truckName]);
                if(modeltruck->hasRocket())
                {
                    //                        addRocketModel(modeltruck->getPosition().vec3d());
                    //                        auto modelRocket = dynamic_cast<Rocket*>(mModels[ROCKET].last());
                    auto activeRocket = modeltruck->getActiveRocket();
                    auto modelAirplane = dynamic_cast<Aircraft*>(mModels[AIRCRAFT].last());
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
        osg::Vec3d position(53, 35.2, 842.5);
        addStationModel(position);
    }
}

bool Model::setup(MapController *pMapController,
                  NetworkManager *networkManager,
                  UIHandle *uiHandle)
{
    mMapController = pMapController;
    mUIHandle = uiHandle;

    ////--websocket data-------------------------------------------------------------------
    QObject::connect(networkManager->webSocketClient(), &WebSocketClient::messageReceived,this ,&Model::onMessageReceived);


    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
        qDebug() << comp->errorString();

        if (comp->status() == QQmlComponent::Ready) {
            QQuickItem *item = (QQuickItem*) comp->create(nullptr);
            mAircraftTableModel = new AircraftTableModel;

            QObject::connect(item,
                             SIGNAL(filterTextChanged(const QString&)),
                             mAircraftTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            item->setProperty("model", QVariant::fromValue<AircraftTableModel*>(mAircraftTableModel));
            mUIHandle->lwAddTab("Aircrafts", item);
        }

    });

    comp->loadUrl(QUrl("qrc:///modelplugin/AircraftTableView.qml"));





}

void Model::demo()
{
    //    int index = 0;
    auto airplaneNames = mModels[AIRCRAFT].keys();
    for (auto name: airplaneNames)
    {
        auto model = dynamic_cast<Aircraft*>(mModels[AIRCRAFT][name]);
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
    osg::Vec3d position(52.8603, 35.274, 842.5);
    //create and setting model--------------------------------------------
    osg::ref_ptr<Truck> model = new Truck(mMapController->getMapNode());
    QString name = TRUCK + QString::number(mModels[TRUCK].count());
    model->setName(name.toStdString());
    model->setGeographicPosition(position, 0.0);
    //model->setLocalRotation(osg::Quat(osg::inDegrees(-30.0),osg::Z_AXIS));
    model->setScale(osg::Vec3(1,1,1));



    //    QObject::connect(model.get(), &FlyingModel::positionChanged, [=](osgEarth::GeoPoint position){
    //        positionChanged(ROCKET, name, position);
    //    });

    //add to container-----------------------------------------------------
    mModels[TRUCK][name] = model;

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


    nPosition += position;
    //model->setLatLongPosition(nPosition);
    //mMap3dWidget->goPosition(nPosition.x(), nPosition.y(), nPosition.z() + 500);
    //move
    //model->moveTo(nPosition,10);
}

void Model::addAircraftModel(QString name, osg::Vec3d geographicPosition, double heading)
{
    //    osg::Vec3d position(52.8601, 35.277, 9100);
    //osg::Vec3d position(52.8601, 35.277, 844);

    //create and setting model--------------------------------------------

    osg::ref_ptr<Aircraft> model = new Aircraft(mMapController, mQmlEngine,mUIHandle);
    //    QString name = AIRPLANE + QString::number(mModels[AIRPLANE].count());
    model->setQStringName(name);
    model->setGeographicPosition(geographicPosition, heading);
    //    model->setScale(osg::Vec3(0.09f,0.09f,0.09f));

    QObject::connect(model.get(), &BaseModel::positionChanged, [=](osgEarth::GeoPoint position){
        //positionChanged(AIRPLANE, name, position);

        auto truckNames = mModels[TRUCK].keys();
        for(auto truckName: truckNames)
        {
            auto truck = dynamic_cast<Truck*>(mModels[TRUCK][truckName]);
            if(truck->hasRocket())
            {
                //                osg::Vec3d wPoint;
                //                position.toWorld(wPoint);
                truck->aimTarget(position.vec3d());
            }
        }
    });
    //add to container-----------------------------------------------------
    mModels[AIRCRAFT][name] = model;


    //add to map ---------------------------------------------------------
    mMapController->addNode(model);
    //mMap3dWidget->goPosition(position.x(), position.y(), position.z() + 500);

    //    double rnd = QRandomGenerator::global()->generateDouble();
    //    double rnd = qrand() % 360;
    //    model->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(rnd), osg::Z_AXIS));

    //hit------------------------------------------------------------------
    QObject::connect(model.get(), &BaseModel::hit, [=](BaseModel */*other*/){

        mModels[AIRCRAFT].remove(QString(model->getName().c_str()));
    });
}

void Model::addRocketModel(osg::Vec3d position)
{

    //create and setting model--------------------------------------------
    osg::ref_ptr<Rocket> model = new Rocket(mMapController->getMapNode());
    QString name = ROCKET + QString::number(mModels[ROCKET].count());
    model->setName(name.toStdString());
    model->setGeographicPosition(position, 0.0);
    model->setScale(osg::Vec3(1,1,1));

    QObject::connect(model.get(), &BaseModel::positionChanged, [=](osgEarth::GeoPoint position){
        positionChanged(ROCKET, name, position);
    });

    //add to container-----------------------------------------------------
    mModels[ROCKET][name] = model;


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

        mModels[ROCKET].remove(QString(model->getName().c_str()));
    });
}

void Model::addSystemModel(osg::Vec3d position)
{
    //create and setting model--------------------------------------------
    osg::ref_ptr<System> model = new System(mMapController);
    QString name = "System" + QString::number(mModels["System"].count());
    model->setQStringName(name);
    model->setGeographicPosition(position, 0.0);
    model->setScale(osg::Vec3(1,1,1));
    //add to container-----------------------------------------------------
    mModels["System"][name] = model;


    //add to map ---------------------------------------------------------
    mMapController->addNode(model);
}

void Model::addStationModel(osg::Vec3d position)
{
    //create and setting model--------------------------------------------
    osg::ref_ptr<Station> model = new Station(mMapController);
    QString name = "Station" + QString::number(mModels["Station"].count());
    model->setQStringName(name);
    model->setGeographicPosition(position, 0.0);
    model->setScale(osg::Vec3(1,1,1));
    //add to container-----------------------------------------------------
    mModels["Station"][name] = model;


    //add to map ---------------------------------------------------------
    mMapController->addNode(model);
}

void Model::clickedTrackNode(QString type, QString name, bool isClick)
{
    if (isClick){
        //        osg::Node* node =mModels[type][name];
        mMapController->setTrackNode(mModels[type][name]->getGeoTransform());
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
    if(message.object().value("Name").toString() == "Target")
    {
        QJsonObject data = message.object().value("Data").toObject();
        //qDebug()<<"target:"<< data;

        double latitude = data.value("Latitude").toDouble();
        double longitude = data.value("Longitude").toDouble();
        double altitude = data.value("Altitude").toDouble();
        double speed = data.value("Speed").toDouble();
        double heading = data.value("Heading").toDouble();
        osg::Vec3d position(latitude, longitude, altitude);
        QString name = QString::number(data.value("TN").toInt());
        QString txtMessage = QString::fromUtf8(message.toJson(QJsonDocument::Compact));
        if(mModels.contains(AIRCRAFT) && mModels[AIRCRAFT].contains(name))
        {
            Aircraft* model = dynamic_cast<Aircraft*>(mModels[AIRCRAFT][name]);
            model->flyTo(position, heading, speed);

            model->setInformation(txtMessage);
        }
        else
        {
            addAircraftModel(name, position, -heading);
            Aircraft* model = dynamic_cast<Aircraft*>(mModels[AIRCRAFT][name]);
            model->setInformation(txtMessage);
        }
        AircraftInfo airInfo;
        airInfo.TN = name;
        airInfo.Latitude = QString::number(latitude);
        airInfo.Altitude = QString::number(altitude);
        airInfo.Speed = QString::number(speed);
        airInfo.Heading = QString::number(heading);
        mAircraftTableModel->updateItemData(airInfo);

    }

}

void Model::frameEvent()
{
//    findSceneModels(mMapController->getViewer());
    for(auto model: mModels[AIRCRAFT])
    {
        model->frameEvent();
    }
//    if(mLastSelectedModel)
//        mLastSelectedModel->frameEvent();
}

void Model::mousePressEvent(QMouseEvent *event)
{

    BaseModel* model = pick(event->x(), event->y());
    if(model)
    {
        model->mousePressEvent(event, true);
    }
    if(mLastSelectedModel && mLastSelectedModel != model)
        mLastSelectedModel->mousePressEvent(event, false);
    if(model)
        mLastSelectedModel = model;

}

void Model::mouseMoveEvent(QMouseEvent *event)
{
    BaseModel* model = pick(event->x(), event->y());
    if(model)
    {
        model->mouseMoveEvent(event, true);
    }
    if(mLastMoveModel && mLastMoveModel != model)
        mLastMoveModel->mouseMoveEvent(event, false);
    if(model)
        mLastMoveModel = model;
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

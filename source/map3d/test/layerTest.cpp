#include "layerTest.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>

LayerTest::LayerTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    createLayers();
    if (mServiceManager->isReadyForSendData())
        for(auto& layerdata: LayerDataList)
            mServiceManager->sendLayer(layerdata);
    else
        QObject::connect(mServiceManager, &ServiceManager::readyForSendData,[this](){
            for(auto& layerdata: LayerDataList)
                mServiceManager->sendLayer(layerdata);
        });

   QTimer *timerUpdateMovable = new QTimer();
   QObject::connect(timerUpdateMovable, &QTimer::timeout, [this](){
        updateLayers();
   });
   timerUpdateMovable->setSingleShot(true);
   // timerUpdateMovable->start(15000);
}

void LayerTest::createLayers()
{
    //--flayable node layer------------------------------------------------
    LayerData flyableLayer;
    flyableLayer.id = 100;
    flyableLayer.parentId = -1;
    flyableLayer.text  = "Flyable";
    flyableLayer.isComposite = true;
    flyableLayer.order = 1;
    flyableLayer.command = Command::Add;
    {
        LayerData flyableLayer1;
        flyableLayer1.id = 101;
        flyableLayer1.parentId = 100;
        flyableLayer1.text  = "Flyable";
        flyableLayer1.isComposite = true;
        flyableLayer1.order = 1;
        flyableLayer1.command = Command::Add;
        {
            LayerData unknownLayer;
            unknownLayer.id = 102;
            unknownLayer.parentId = 101;
            unknownLayer.text  = "Unknown";
            unknownLayer.order = 1;
            unknownLayer.command = Command::Add;
            flyableLayer1.children.push_back(unknownLayer);

            LayerData speedUp200Layer;
            speedUp200Layer.id = 103;
            speedUp200Layer.parentId = 101;
            speedUp200Layer.text  = "SpeedUp200";
            speedUp200Layer.order = 2;
            speedUp200Layer.command = Command::Add;
            flyableLayer1.children.push_back(speedUp200Layer);

            LayerData speedDown200Layer;
            speedDown200Layer.id = 104;
            speedDown200Layer.parentId = 101;
            speedDown200Layer.text  = "SpeedDown200";
            speedDown200Layer.order = 3;
            speedDown200Layer.command = Command::Add;
            flyableLayer1.children.push_back(speedDown200Layer);
        }
        flyableLayer.children.push_back(flyableLayer1);

        LayerData routeLayer;
        routeLayer.id = 105;
        routeLayer.parentId = 100;
        routeLayer.text  = "Route";
        routeLayer.order = 2;
        routeLayer.command = Command::Add;
        flyableLayer.children.push_back(routeLayer);

        LayerData statusLayer;
        statusLayer.id = 106;
        statusLayer.parentId = 100;
        statusLayer.text  = "Status";
        statusLayer.order = 3;
        statusLayer.command = Command::Add;
        flyableLayer.children.push_back(statusLayer);
    }
    LayerDataList.push_back(flyableLayer);
    //--moveable layer------------------------------------------------------
    LayerData movableLayer;
    movableLayer.id = 200;
    movableLayer.parentId = -1;
    movableLayer.text  = "Movable";
    movableLayer.isComposite = true;
    movableLayer.order = 2;
    movableLayer.command = Command::Add;
    {
        LayerData movableLayer1;
        movableLayer1.id = 201;
        movableLayer1.parentId = 200;
        movableLayer1.text  = "Movable";
        movableLayer1.isComposite = true;
        movableLayer1.order = 1;
        movableLayer1.command = Command::Add;
        {
            LayerData unknownLayer;
            unknownLayer.id = 202;
            unknownLayer.parentId = 201;
            unknownLayer.text  = "Unknown";
            unknownLayer.order = 1;
            unknownLayer.command = Command::Add;
            movableLayer1.children.push_back(unknownLayer);

            LayerData speedUp200Layer;
            speedUp200Layer.id = 203;
            speedUp200Layer.parentId = 201;
            speedUp200Layer.text  = "SpeedUp200";
            speedUp200Layer.order = 2;
            speedUp200Layer.command = Command::Add;
            movableLayer1.children.push_back(speedUp200Layer);

            LayerData speedDown200Layer;
            speedDown200Layer.id = 204;
            speedDown200Layer.parentId = 201;
            speedDown200Layer.text  = "SpeedDown200";
            speedDown200Layer.order = 3;
            speedDown200Layer.command = Command::Add;
            movableLayer1.children.push_back(speedDown200Layer);
        }
        movableLayer.children.push_back(movableLayer1);

        LayerData routeLayer;
        routeLayer.id = 205;
        routeLayer.parentId = 200;
        routeLayer.text  = "Route";
        routeLayer.order = 2;
        routeLayer.command = Command::Add;
        movableLayer.children.push_back(routeLayer);

        LayerData statusLayer;
        statusLayer.id = 206;
        statusLayer.parentId = 200;
        statusLayer.text  = "Status";
        statusLayer.order = 3;
        statusLayer.command = Command::Add;
        movableLayer.children.push_back(statusLayer);
    }
    LayerDataList.push_back(movableLayer);
    //--node layer---------------------------------------------------------------
    LayerData nodeLayer;
    nodeLayer.id = 300;
    nodeLayer.parentId = -1;
    nodeLayer.text  = "Node";
    nodeLayer.isComposite = true;
    nodeLayer.order = 3;
    nodeLayer.command = Command::Add;
    {
        LayerData nodeLayer1;
        nodeLayer1.id = 301;
        nodeLayer1.parentId = 300;
        nodeLayer1.text  = "Node";
        nodeLayer1.isComposite = true;
        nodeLayer1.order = 1;
        nodeLayer1.command = Command::Add;
        {
            LayerData activeLayer;
            activeLayer.id = 302;
            activeLayer.parentId = 301;
            activeLayer.text  = "Active";
            activeLayer.order = 1;
            activeLayer.command = Command::Add;
            nodeLayer1.children.push_back(activeLayer);

            LayerData inactiveLayer;
            inactiveLayer.id = 303;
            inactiveLayer.parentId = 301;
            inactiveLayer.text  = "Inactive";
            inactiveLayer.order = 2;
            inactiveLayer.command = Command::Add;
            nodeLayer1.children.push_back(inactiveLayer);
        }
        nodeLayer.children.push_back(nodeLayer1);

        LayerData circleLayer;
        circleLayer.id = 304;
        circleLayer.parentId = 300;
        circleLayer.text  = "Circle";
        circleLayer.order = 2;
        circleLayer.command = Command::Add;
        nodeLayer.children.push_back(circleLayer);

        LayerData statusLayer;
        statusLayer.id = 306;
        statusLayer.parentId = 300;
        statusLayer.text  = "Status";
        statusLayer.order = 3;
        statusLayer.command = Command::Add;
        nodeLayer.children.push_back(statusLayer);

        LayerData polygonLayer;
        polygonLayer.id = 305;
        polygonLayer.parentId = 300;
        polygonLayer.text  = "Polygon";
        polygonLayer.order = 4;
        polygonLayer.command = Command::Add;
        nodeLayer.children.push_back(polygonLayer);
    }
    LayerDataList.push_back(nodeLayer);
    //--combat layer-------------------------------------------------------
    LayerData combatLayer;
    combatLayer.id = 400;
    combatLayer.parentId = -1;
    combatLayer.text  = "Combat";
    combatLayer.isComposite = true;
    combatLayer.order = 4;
    combatLayer.command = Command::Add;
    {
        LayerData AssignmentLayer;
        AssignmentLayer.id = 401;
        AssignmentLayer.parentId = 400;
        AssignmentLayer.text  = "Assignment";
        AssignmentLayer.order = 1;
        AssignmentLayer.command = Command::Add;
        combatLayer.children.push_back(AssignmentLayer);

        LayerData ExplosionLayer;
        ExplosionLayer.id = 402;
        ExplosionLayer.parentId = 400;
        ExplosionLayer.text  = "Explosion";
        ExplosionLayer.order = 2;
        ExplosionLayer.command = Command::Add;
        combatLayer.children.push_back(ExplosionLayer);
    }
    LayerDataList.push_back(combatLayer);

    //    std::ofstream myfile ("example.txt");
    //    myfile << doc.toJson(QJsonDocument::Indented).toStdString();
    //    myfile.close();
}

void LayerTest::updateLayers()
{
    LayerData circleLayer;
    circleLayer.id = 304;
    circleLayer.parentId = 300;
    circleLayer.command = Command::Remove;
    mServiceManager->sendLayer(circleLayer);

    LayerData movableLayer1;
    movableLayer1.id = 201;
    movableLayer1.parentId = 200;
    movableLayer1.command = Command::Remove;
    mServiceManager->sendLayer(movableLayer1);


    LayerData speedDown200Layer;
    speedDown200Layer.id = 104;
    speedDown200Layer.parentId = 301;
    speedDown200Layer.text  = "updateTest";
    speedDown200Layer.order = 3;
    speedDown200Layer.command = Command::Add;
    mServiceManager->sendLayer(speedDown200Layer);
}

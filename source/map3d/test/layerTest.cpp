#include "layerTest.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>

LayerTest::LayerTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    createLayers();
    for(auto& layerdata: LayerDataList)
        mServiceManager->sendLayer(layerdata);
//    QTimer *timerUpdateMovable = new QTimer();
//    QObject::connect(timerUpdateMovable, &QTimer::timeout, [this](){
//        QJsonDocument layerDoc = updateLayers();
//        mNetworkManager->sendData(layerDoc.toJson(QJsonDocument::Compact));
//    });
//    timerUpdateMovable->start(10000);
}

void LayerTest::createLayers()
{
    //--flayable node layer------------------------------------------------
    LayerData flyableLayer;
    flyableLayer.id = 100;
    flyableLayer.parentId = -1;
    flyableLayer.text  = "Flyable";
    flyableLayer.order = 1;
    flyableLayer.command = Command::Add;
    {
        LayerData flyableLayer1;
        flyableLayer1.id = 101;
        flyableLayer1.parentId = 100;
        flyableLayer1.text  = "Flyable";
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
    movableLayer.order = 2;
    movableLayer.command = Command::Add;
    {
        LayerData movableLayer1;
        movableLayer1.id = 201;
        movableLayer1.parentId = 200;
        movableLayer1.text  = "Movable";
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
    nodeLayer.order = 3;
    nodeLayer.command = Command::Add;
    {
        LayerData nodeLayer1;
        nodeLayer1.id = 301;
        nodeLayer1.parentId = 300;
        nodeLayer1.text  = "Node";
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
        statusLayer.id = 305;
        statusLayer.parentId = 300;
        statusLayer.text  = "Status";
        statusLayer.order = 3;
        statusLayer.command = Command::Add;
        nodeLayer.children.push_back(statusLayer);

        LayerData polygonLayer;
        polygonLayer.id = 306;
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

QJsonDocument LayerTest::updateLayers()
{
    QJsonObject layers;
    QJsonArray layersData;
    layers.insert("Type", "Layer");
    //--flayable node layer------------------------------------------------
    QJsonObject flyable;
    flyable.insert("Id", 100);
    flyable.insert("ParentId", 0);
    flyable.insert("Text", "Flyable");
    flyable.insert("Order", 1);
    flyable.insert("Command", "ADD");
    QJsonArray flableChildren;
    {
        QJsonObject flyableNode;
        flyableNode.insert("Id", 101);
        flyableNode.insert("ParentId", 100);
        flyableNode.insert("Text", "Flyable");
        flyableNode.insert("Order", 1);
        flyableNode.insert("Command", "ADD");
        QJsonArray flyableNodeChildren;
        {
            QJsonObject unknown;
            unknown.insert("Id", 102);
            unknown.insert("ParentId", 101);
            unknown.insert("Text", "Unknown");
            unknown.insert("Order", 1);
            unknown.insert("Command", "ADD");
            flyableNodeChildren.push_back(unknown);

            QJsonObject speedUp200;
            speedUp200.insert("Id", 103);
            speedUp200.insert("ParentId", 101);
            speedUp200.insert("Text", "SpeedUp200");
            speedUp200.insert("Order", 2);
            speedUp200.insert("Command", "ADD");
            flyableNodeChildren.push_back(speedUp200);

            QJsonObject speedDown200;
            speedDown200.insert("Id", 104);
            speedDown200.insert("ParentId", 101);
            speedDown200.insert("Text", "SpeedDown200");
            speedDown200.insert("Order", 3);
            speedDown200.insert("Command", "ADD");
            flyableNodeChildren.push_back(speedDown200);

            QJsonObject route;
            route.insert("Id", 105);
            route.insert("ParentId", 100);
            route.insert("Text", "Route");
            route.insert("Order", 2);
            route.insert("Command", "ADD");
            flyableNodeChildren.push_back(route);
        }
        flyableNode.insert("Children", flyableNodeChildren);
        flableChildren.push_back(flyableNode);

        // QJsonObject route;
        // route.insert("Id", 105);
        // route.insert("ParentId", 100);
        // route.insert("Text", "Route");
        // route.insert("Order", 2);
        // route.insert("Command", "ADD");
        // flableChildren.push_back(route);

        // QJsonObject status;
        // status.insert("Id", 106);
        // status.insert("ParentId", 100);
        // status.insert("Text", "Status");
        // status.insert("Order", 3);
        // status.insert("Command", "ADD");
        // flableChildren.push_back(status);
    }
    flyable.insert("Children", flableChildren);
    layersData.push_back(flyable);
    //--moveable layer------------------------------------------------------
    QJsonObject movable;
    movable.insert("Id", 200);
    movable.insert("ParentId", 0);
    movable.insert("Text", "Movable");
    movable.insert("Order", 1);
    movable.insert("Command", "ADD");
    QJsonArray movableChildren;
    {
        QJsonObject movableNode;
        movableNode.insert("Id", 201);
        movableNode.insert("ParentId", 200);
        movableNode.insert("Text", "Movable");
        movableNode.insert("Order", 1);
        movableNode.insert("Command", "ADD");
        QJsonArray movableNodeChildren;
        {
            QJsonObject unknown;
            unknown.insert("Id", 202);
            unknown.insert("ParentId", 201);
            unknown.insert("Text", "Unknown");
            unknown.insert("Order", 1);
            unknown.insert("Command", "ADD");
            movableNodeChildren.push_back(unknown);

            QJsonObject speedUp200;
            speedUp200.insert("Id", 203);
            speedUp200.insert("ParentId", 201);
            speedUp200.insert("Text", "SpeedUp200");
            speedUp200.insert("Order", 2);
            speedUp200.insert("Command", "ADD");
            movableNodeChildren.push_back(speedUp200);

            QJsonObject speedDown200;
            speedDown200.insert("Id", 204);
            speedDown200.insert("ParentId", 201);
            speedDown200.insert("Text", "SpeedDown200");
            speedDown200.insert("Order", 3);
            speedDown200.insert("Command", "ADD");
            movableNodeChildren.push_back(speedDown200);
        }
        movableNode.insert("Children", movableNodeChildren);
        movableChildren.push_back(movableNode);

        QJsonObject route;
        route.insert("Id", 205);
        route.insert("ParentId", 200);
        route.insert("Text", "Route");
        route.insert("Order", 2);
        route.insert("Command", "ADD");
        movableChildren.push_back(route);

        // QJsonObject status;
        // status.insert("Id", 206);
        // status.insert("ParentId", 200);
        // status.insert("Text", "Status");
        // status.insert("Order", 3);
        // status.insert("Command", "ADD");
        // movableChildren.push_back(status);
    }
    movable.insert("Children", movableChildren);
    layersData.push_back(movable);

    //--node layer---------------------------------------------------------------
    QJsonObject node;
    node.insert("Id", 300);
    node.insert("ParentId", 0);
    node.insert("Text", "Node");
    node.insert("Order", 3);
    node.insert("Command", "ADD");
    QJsonArray nodeChildren;
    {
        QJsonObject nodeChild;
        nodeChild.insert("Id", 301);
        nodeChild.insert("ParentId", 300);
        nodeChild.insert("Text", "Node");
        nodeChild.insert("Order", 1);
        nodeChild.insert("Command", "ADD");
        QJsonArray nodeChildChildren;
        {
            QJsonObject active;
            active.insert("Id", 302);
            active.insert("ParentId", 301);
            active.insert("Text", "Active");
            active.insert("Order", 1);
            active.insert("Command", "ADD");
            nodeChildChildren.push_back(active);

            QJsonObject inactive;
            inactive.insert("Id", 303);
            inactive.insert("ParentId", 301);
            inactive.insert("Text", "Inactive");
            inactive.insert("Order", 2);
            inactive.insert("Command", "ADD");
            nodeChildChildren.push_back(inactive);
        }
        nodeChild.insert("Children", nodeChildChildren);
        nodeChildren.push_back(nodeChild);

        QJsonObject circle;
        circle.insert("Id", 304);
        circle.insert("ParentId", 300);
        circle.insert("Text", "Circle");
        circle.insert("Order", 2);
        circle.insert("Command", "ADD");
        nodeChildren.push_back(circle);

        // QJsonObject status;
        // status.insert("Id", 305);
        // status.insert("ParentId", 300);
        // status.insert("Text", "Status");
        // status.insert("Order", 3);
        // status.insert("Command", "ADD");
        // nodeChildren.push_back(status);

        QJsonObject polygon;
        polygon.insert("Id", 306);
        polygon.insert("ParentId", 300);
        polygon.insert("Text", "Polygon");
        polygon.insert("Order", 4);
        polygon.insert("Command", "ADD");
        nodeChildren.push_back(polygon);
    }
    node.insert("Children", nodeChildren);
    layersData.push_back(node);
    //---------------------------------------------------------------------------
    layers.insert("Data", layersData);
    QJsonDocument doc;
    doc.setObject(layers);
    //    std::ofstream myfile ("example.txt");
    //    myfile << doc.toJson(QJsonDocument::Indented).toStdString();
    //    myfile.close();
    return doc;
}

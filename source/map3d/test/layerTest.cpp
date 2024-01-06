#include "layerTest.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>

LayerTest::LayerTest(NetworkManager *networkManager):
    mNetworkManager(networkManager)
{
    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]
                     {
                         QJsonDocument layerDoc = createLayers();
                         mNetworkManager->sendData(layerDoc.toJson(QJsonDocument::Compact));

                         QTimer *timerUpdateMovable = new QTimer();
                         QObject::connect(timerUpdateMovable, &QTimer::timeout, [this](){
                             QJsonDocument layerDoc = updateLayers();
                             mNetworkManager->sendData(layerDoc.toJson(QJsonDocument::Compact));
                         });
                         timerUpdateMovable->start(10000);
                     });
}

QJsonDocument LayerTest::createLayers()
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
        }
        flyableNode.insert("Children", flyableNodeChildren);
        flableChildren.push_back(flyableNode);

        QJsonObject route;
        route.insert("Id", 105);
        route.insert("ParentId", 100);
        route.insert("Text", "Route");
        route.insert("Order", 2);
        route.insert("Command", "ADD");
        flableChildren.push_back(route);

        QJsonObject status;
        status.insert("Id", 106);
        status.insert("ParentId", 100);
        status.insert("Text", "Status");
        status.insert("Order", 3);
        status.insert("Command", "ADD");
        flableChildren.push_back(status);
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

        QJsonObject status;
        status.insert("Id", 206);
        status.insert("ParentId", 200);
        status.insert("Text", "Status");
        status.insert("Order", 3);
        status.insert("Command", "ADD");
        movableChildren.push_back(status);
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

        QJsonObject status;
        status.insert("Id", 305);
        status.insert("ParentId", 300);
        status.insert("Text", "Status");
        status.insert("Order", 3);
        status.insert("Command", "ADD");
        nodeChildren.push_back(status);

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

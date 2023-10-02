#include "layerTest.h"

#include <QJsonArray>
#include <QJsonObject>

#include <fstream>
LayerTest::LayerTest(NetworkManager *networkManager):
    mNetworkManager(networkManager)
{
    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]
                     {
                         QJsonDocument layerDoc = createLayers();
                         mNetworkManager->sendData(layerDoc.toJson(QJsonDocument::Compact));
                     });
}

QJsonDocument LayerTest::createLayers()
{
    QJsonObject layers;
    QJsonObject layersData;
    layers.insert("Type", "Layer");
    //--flayable layer------------------------------------------------
    QJsonObject flyable;
    flyable.insert("Id", 100);
    flyable.insert("ParentId", 0);
    flyable.insert("Text", "Flyable");
    flyable.insert("Order", 1);
    QJsonArray flableChildren;
    {
        QJsonObject flyableNode;
        flyableNode.insert("Id", 101);
        flyableNode.insert("ParentId", 100);
        flyableNode.insert("Text", "Flyable");
        flyableNode.insert("Order", 1);
        QJsonArray flyableNodeChildren;
        {
            QJsonObject unknown;
            unknown.insert("Id", 102);
            unknown.insert("ParentId", 101);
            unknown.insert("Text", "Unknown");
            unknown.insert("Order", 1);
            flyableNodeChildren.push_back(unknown);

            QJsonObject speedUp200;
            speedUp200.insert("Id", 103);
            speedUp200.insert("ParentId", 101);
            speedUp200.insert("Text", "SpeedUp200");
            speedUp200.insert("Order", 2);
            flyableNodeChildren.push_back(speedUp200);

            QJsonObject speedDown200;
            speedDown200.insert("Id", 104);
            speedDown200.insert("ParentId", 101);
            speedDown200.insert("Text", "SpeedDown200");
            speedDown200.insert("Order", 3);
            flyableNodeChildren.push_back(speedDown200);
        }
        flyableNode.insert("Children", flyableNodeChildren);
        flableChildren.push_back(flyableNode);

        QJsonObject route;
        route.insert("Id", 105);
        route.insert("ParentId", 100);
        route.insert("Text", "Route");
        route.insert("Order", 2);
        flableChildren.push_back(route);

        QJsonObject status;
        status.insert("Id", 106);
        status.insert("ParentId", 100);
        status.insert("Text", "Status");
        status.insert("Order", 3);
        flableChildren.push_back(status);
    }
    flyable.insert("Children", flableChildren);
    layersData.insert("Flyable", flyable);
    //--moveable layer------------------------------------------------------
    QJsonObject movable;
    movable.insert("Id", 200);
    movable.insert("ParentId", 0);
    movable.insert("Text", "Movable");
    movable.insert("Order", 1);
    QJsonArray movableChildren;
    {
        QJsonObject movableNode;
        movableNode.insert("Id", 201);
        movableNode.insert("ParentId", 200);
        movableNode.insert("Text", "Movable");
        movableNode.insert("Order", 1);
        QJsonArray movableNodeChildren;
        {
            QJsonObject unknown;
            unknown.insert("Id", 202);
            unknown.insert("ParentId", 201);
            unknown.insert("Text", "Unknown");
            unknown.insert("Order", 1);
            movableNodeChildren.push_back(unknown);

            QJsonObject speedUp200;
            speedUp200.insert("Id", 203);
            speedUp200.insert("ParentId", 201);
            speedUp200.insert("Text", "SpeedUp200");
            speedUp200.insert("Order", 2);
            movableNodeChildren.push_back(speedUp200);

            QJsonObject speedDown200;
            speedDown200.insert("Id", 204);
            speedDown200.insert("ParentId", 201);
            speedDown200.insert("Text", "SpeedDown200");
            speedDown200.insert("Order", 3);
            movableNodeChildren.push_back(speedDown200);
        }
        movableNode.insert("Children", movableNodeChildren);
        movableChildren.push_back(movableNode);

        QJsonObject route;
        route.insert("Id", 205);
        route.insert("ParentId", 200);
        route.insert("Text", "Route");
        route.insert("Order", 2);
        movableChildren.push_back(route);

        QJsonObject status;
        status.insert("Id", 206);
        status.insert("ParentId", 200);
        status.insert("Text", "Status");
        status.insert("Order", 3);
        movableChildren.push_back(status);
    }
    movable.insert("Children", movableChildren);
    layersData.insert("Movable", movable);
    //--fixed layer---------------------------------------------------------


    layers.insert("Data", layersData);
    QJsonDocument doc;
    doc.setObject(layers);
//    std::ofstream myfile ("example.txt");
//    myfile << doc.toJson(QJsonDocument::Indented).toStdString();
//    myfile.close();
    return doc;
}

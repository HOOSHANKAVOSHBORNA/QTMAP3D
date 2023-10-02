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
    //--flayable node layer------------------------------------------------
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

    //--moveable node layer------------------------------------------------------

    //--node layer---------------------------------------------------------------
    QJsonObject node;
    node.insert("Id", 300);
    node.insert("ParentId", 0);
    node.insert("Text", "Node");
    node.insert("Order", 3);
    QJsonArray nodeChildren;
    {
        QJsonObject nodeChild;
        nodeChild.insert("Id", 301);
        nodeChild.insert("ParentId", 300);
        nodeChild.insert("Text", "Node");
        nodeChild.insert("Order", 1);
        QJsonArray nodeChildChildren;
        {
            QJsonObject active;
            active.insert("Id", 302);
            active.insert("ParentId", 301);
            active.insert("Text", "Active");
            active.insert("Order", 1);
            nodeChildChildren.push_back(active);

            QJsonObject inactive;
            inactive.insert("Id", 303);
            inactive.insert("ParentId", 301);
            inactive.insert("Text", "Inactive");
            inactive.insert("Order", 2);
            nodeChildChildren.push_back(inactive);
        }
        nodeChild.insert("Children", nodeChildChildren);
        nodeChildren.push_back(nodeChild);

        QJsonObject range;
        range.insert("Id", 304);
        range.insert("ParentId", 300);
        range.insert("Text", "Range");
        range.insert("Order", 2);
        nodeChildren.push_back(range);

        QJsonObject status;
        status.insert("Id", 305);
        status.insert("ParentId", 300);
        status.insert("Text", "Status");
        status.insert("Order", 3);
        nodeChildren.push_back(status);
    }
    node.insert("Children", nodeChildren);
    layersData.insert("Node", node);
    //---------------------------------------------------------------------------
    layers.insert("Data", layersData);
    QJsonDocument doc;
    doc.setObject(layers);
//    std::ofstream myfile ("example.txt");
//    myfile << doc.toJson(QJsonDocument::Indented).toStdString();
//    myfile.close();
    return doc;
}

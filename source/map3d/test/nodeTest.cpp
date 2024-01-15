#include "nodeTest.h"
#include <QJsonObject>
#include <QObject>
#include <QColor>
#include <QRandomGenerator>
#include <QTimer>
#include <QJsonArray>

NodeTest::NodeTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    //--create and update aircraft info------------------------
    QTimer *timerUpdate = new QTimer();
    QObject::connect(timerUpdate, &QTimer::timeout, [this](){
        createInfo();
        updateInfo();
        for(auto& nodeInfo: mNodeInfoList){
            mServiceManager->sendNode(nodeInfo.nodeData);
            mServiceManager->sendStatusNode(nodeInfo.statusNodeData);
            mServiceManager->sendCircle(nodeInfo.circleData);
            mServiceManager->sendPolygon(nodeInfo.polygonData);
            mServiceManager->sendPolyLine(nodeInfo.polyLineData);
        }
    });
    timerUpdate->start(500);

    QTimer *timerRemove = new QTimer();
    QObject::connect(timerRemove, &QTimer::timeout, [this](){
        if (mNodeInfoList.size() > 0){
            // QJsonObject jsonObject = mNodeDataList[0].nodeDoc.object();
            // QJsonObject jsonStatusObject = mNodeDataList[0].statusDoc.object();
            // QJsonObject jsonCircleObject = mNodeDataList[0].circleDoc.object();
            // QJsonObject jsonPolygonObject = mNodeDataList[0].polygonDoc.object();

            // jsonObject.remove("COMMAND");
            // jsonObject.insert("COMMAND", "REMOVE");
            // jsonStatusObject.remove("COMMAND");
            // jsonStatusObject.insert("COMMAND", "REMOVE");
            // jsonCircleObject.remove("COMMAND");
            // jsonCircleObject.insert("COMMAND", "REMOVE");
            // jsonPolygonObject.remove("COMMAND");
            // jsonPolygonObject.insert("COMMAND", "REMOVE");

            // mNodeDataList.pop_front();

            // QJsonDocument jsonDoc(jsonObject);
            // QJsonDocument jsonStatusDoc(jsonStatusObject);
            // QJsonDocument jsonCircleDoc(jsonCircleObject);
            // QJsonDocument jsonPolygonDoc(jsonPolygonObject);

            // mNetworkManager->sendData(jsonDoc.toJson(QJsonDocument::Compact));
            // mNetworkManager->sendData(jsonStatusDoc.toJson(QJsonDocument::Compact));
            // mNetworkManager->sendData(jsonCircleDoc.toJson(QJsonDocument::Compact));
            // mNetworkManager->sendData(jsonPolygonDoc.toJson(QJsonDocument::Compact));
        }
    });
    timerRemove->start(7000);
}

void NodeTest::createInfo()
{
    if(mCount >= mMaxNumber)
        return;
    //---------------------------------------------------------
    QVector<QString> colors{"yellow", "red", "blue", "green"};
    QVector<QString> types{NodeType::Fixed, NodeType::Movable, NodeType::Flyable};
    QString name = "Node" + QString::number(mCount);
    int id = 30000 + mCount++;
    QColor color(colors[QRandomGenerator::global()->generate() % 4]);
    double longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
    double latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
    double altitude =0;/* (2000 + (QRandomGenerator::global()->generate() % (9000 - 2000)));*/
    //--------------------------------------------------------
    NodeInfo nodInfo;
    NodeData nodData;
    nodData.id = id;
    nodData.name = name;
    nodData.type = types[QRandomGenerator::global()->generate() % 3];
    nodData.category = nodData.type == NodeType::Fixed ? "Station" :
                           nodData.type == NodeType::Movable ? "Car": "Aircraft";
    nodData.command = Command::Add;
    nodData.iconInfoUrl = nodData.type == NodeType::Fixed ? "../data/models/station/station.png"
                          : nodData.type == NodeType::Movable
                              ? "../data/models/car/car.png"
                              : "qrc:/Resources/aircraft.png";

    nodData.imgInfoUrl = nodData.type == NodeType::Fixed ? "qrc:/Resources/system.jpg"
                         : nodData.type == NodeType::Movable
                             ? "qrc:/Resources/station.jpg"
                             : "qrc:/Resources/airplane1.jpg";

    nodData.url2D = nodData.type == NodeType::Fixed ? "../data/models/station/station.png"
                    : nodData.type == NodeType::Movable
                        ? "../data/models/car/car.png"
                        : "../data/models/airplane/airplane.png";

    nodData.url3D = nodData.type == NodeType::Fixed ? "../data/models/station/station.osgb"
                    : nodData.type == NodeType::Movable
                        ? "../data/models/car/car.osgb"
                        : "../data/models/airplane/airplane.osgb";

    nodData.color = color.name();
    nodData.isAttacker = false;
    nodData.latitude = latitude;
    nodData.longitude = longitude;
    nodData.altitude = altitude;

    double rand = (QRandomGenerator::global()->generate() % (2));
    if(rand < 1)
        nodData.layersId.push_back(302);
    else
        nodData.layersId.push_back(303);

    NodeFieldData nameField;
    nameField.name = "Name";
    nameField.value = name;
    nameField.category = "Main Information";
    nodData.fieldData.push_back(nameField);

    NodeFieldData idField;
    idField.name = "Id";
    idField.value = id;
    idField.category = "Main Information";
    nodData.fieldData.push_back(idField);

    NodeFieldData colorField;
    colorField.name = "Color";
    colorField.value = color.name();
    colorField.category = "Main Information";
    nodData.fieldData.push_back(colorField);

    NodeFieldData latField;
    latField.name = "Latitude";
    latField.value = latitude;
    latField.category = "Location Information";
    nodData.fieldData.push_back(latField);

    NodeFieldData longField;
    longField.name = "Longitude";
    longField.value = longitude;
    longField.category = "Location Information";
    nodData.fieldData.push_back(longField);

    NodeFieldData altField;
    altField.name = "Altitude";
    altField.value = altitude;
    altField.category = "Location Information";
    nodData.fieldData.push_back(altField);

    nodData.command = Command::Add;
    nodInfo.nodeData = nodData;
    //--status node-----------------------------------------------
    StatusNodeData statusNodeData;
    statusNodeData.id = id;
    statusNodeData.name = "Status" + QString::number(mCount);
    statusNodeData.latitude = latitude;
    statusNodeData.longitude = longitude;
    statusNodeData.altitude = altitude;
    statusNodeData.command = Command::Add;
    statusNodeData.layerId = 305;

    NodeFieldData info1Field;
    info1Field.name = "Info1";
    info1Field.value = 200;
    info1Field.category = "";
    statusNodeData.fieldData.push_back(info1Field);

    NodeFieldData info2Field;
    info2Field.name = "Info2";
    info2Field.value = "StatusInfo";
    info2Field.category = "";
    statusNodeData.fieldData.push_back(info2Field);
    statusNodeData.fieldData.push_back(colorField);

    statusNodeData.command = Command::Add;
    nodInfo.statusNodeData = statusNodeData;

    if (nodData.type == NodeType::Fixed) {
        //--circle node------------------------------------------------
        QColor colorCircle("red");
        colorCircle.setAlpha(100);
        CircleData circleData;
        circleData.id = id;
        circleData.name = "Circle" + QString::number(mCount);
        circleData.radius = 1000 + (QRandomGenerator::global()->generate() % (5000 - 1000));
        circleData.color = colorCircle.name();
        circleData.strokeColor = colorCircle.name();
        circleData.strokeWidth = 0;
        circleData.altitude = altitude;
        circleData.latitude = latitude;
        circleData.longitude = longitude;
        circleData.command = Command::Add;
        circleData.layerId = 304;

        circleData.command = Command::Add;
        nodInfo.circleData = circleData;
        //--polygon----------------------------------------------------
        QColor colorPolygon("green");
        colorPolygon.setAlpha(50);
        QColor colorStroke("blue");

        PolygonData polygonData;
        polygonData.id = id;
        polygonData.name = "Polygon" + QString::number(mCount);
        polygonData.fillColor = colorPolygon.name(QColor::HexArgb);
        polygonData.color = colorStroke.name(QColor::HexArgb);
        polygonData.width = 7;
        polygonData.command = Command::Add;
        polygonData.layerId = 306;

        double step = 0.01;
        QVector3D point1;
        point1.setX(longitude - step);
        point1.setY(latitude - step);
        point1.setZ(altitude);
        polygonData.points.push_back(point1);

        QVector3D point2;
        point2.setX(longitude + step);
        point2.setY(latitude - step);
        point2.setZ(altitude);
        polygonData.points.push_back(point2);

        QVector3D point3;
        point3.setX(longitude + step);
        point3.setY(latitude + step);
        point3.setZ(altitude);
        polygonData.points.push_back(point3);

        QVector3D point4;
        point4.setX(longitude - step);
        point4.setY(latitude + step);
        point4.setZ(altitude);
        polygonData.points.push_back(point4);

        polygonData.command = Command::Add;
        nodInfo.polygonData = polygonData;
    }
    else {
        //--polyline----------------------------------------------------
        QColor colorPolyLine("blue");
        colorPolyLine.setAlpha(50);

        PolyLineData polyLineData;
        polyLineData.id = id;
        polyLineData.name = "PolyLine" + QString::number(mCount);
        polyLineData.color = colorPolyLine.name(QColor::HexArgb);
        polyLineData.width = 7;
        polyLineData.command = Command::Add;
        polyLineData.layerId = 306;

        double step = 10.0/10000.0;

        QVector3D point;
        point.setX(longitude + step);
        point.setY(latitude + step);
        point.setZ((nodData.type != NodeType::Flyable) ? 0 : altitude + step);
        polyLineData.points.push_back(point);

        nodInfo.polyLineData = polyLineData;
    }

    mNodeInfoList.append(nodInfo);
}

void NodeTest::updateInfo()
{
    for(auto& nodeInfo: mNodeInfoList)
    {
        int rn = (0 + (QRandomGenerator::global()->generate() % 10000));
        if(rn < 1)
            nodeInfo.nodeData.heading = (0 + (QRandomGenerator::global()->generate() % 361));

        nodeInfo.nodeData.speed = (138 + (QRandomGenerator::global()->generate() % 137));

        double pi = 3.14159265359;
        double teta = (90 - nodeInfo.nodeData.heading)* (pi / 180);
        double step = 10.0/10000.0;
        nodeInfo.nodeData.longitude += step * std::cos(teta);
        nodeInfo.nodeData.latitude += step * std::sin(teta);
        nodeInfo.nodeData.altitude = 2000;
        // double altitude = nodeInfo.nodeData.altitude;
        // rn = (0 + (QRandomGenerator::global()->generate() % 2));
        // nodeInfo.nodeData.altitude += (nodeInfo.nodeData.type != NodeType::Flyable) ? 0 : ((rn > 0) * 2 - 1) * 5;
        //        if(rn < 1)
        //            altitude += altitude - 5;
        //        else
        //            altitude += altitude + 5;

        //--status node-----------------------------------------------
        nodeInfo.statusNodeData.longitude = nodeInfo.nodeData.longitude;
        nodeInfo.statusNodeData.latitude = nodeInfo.nodeData.latitude;
        nodeInfo.statusNodeData.altitude = nodeInfo.nodeData.altitude;

        //--route node------------------------------------------------

        QVector3D point;
        point.setX(nodeInfo.nodeData.longitude);
        point.setY(nodeInfo.nodeData.latitude);
        point.setX(nodeInfo.nodeData.altitude);
        nodeInfo.polyLineData.points.push_back(point);

        if (nodeInfo.polyLineData.points.size() > 100)
            nodeInfo.polyLineData.points.pop_back();
    }
}

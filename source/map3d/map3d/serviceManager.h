#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "qjsonarray.h"
#include "qjsonobject.h"
#include "qvectornd.h"
#include <QJsonDocument>
#include <osg/Referenced>
#include <osg/Vec3d>

class NetworkManager;

struct Command
{
    static inline QString Add{"Add"};
    static inline QString Update{"Update"};
    static inline QString Remove{"Remove"};
};

struct NodeType
{
    static inline QString Fixed{"Fixed"};
    static inline QString Movable{"Movable"};
    static inline QString Flyable{"Flyable"};
};

struct AssignmentState
{
    static inline QString Assinged{"Assigned"};
    static inline QString Search{"Search"};
    static inline QString Lock{"Lock"};
    static inline QString Fire{"Fire"};
    static inline QString Success{"Success"};
    static inline QString Failed{"Failed"};
};

struct NodeFieldData
{
    QString name;
    QVariant value;
    QString category;
    QString categoryIconUrl;

    bool operator==(const QString &n) const { return n == name; }

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Name", name);
        jsonObject.insert("Value", value.toJsonValue());
        jsonObject.insert("Category", category);
        jsonObject.insert("CategoryIconUrl", categoryIconUrl);

        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        name = json["Name"].toString();
        value = json["Value"].toVariant();
        category = json["Category"].toString();
        categoryIconUrl = json["CategoryIconUrl"].toString();
    }
};

struct NodeData: public osg::Referenced
{
    NodeData(){

    }
    NodeData(const NodeData& nodeData) {
        id= nodeData.id;
        name= nodeData.name;
        type= nodeData.type;
        category = nodeData.category;
        url2D= nodeData.url2D;
        url3D= nodeData.url3D;
        imgInfoUrl = nodeData.imgInfoUrl;
        iconInfoUrl = nodeData.iconInfoUrl;
        color= nodeData.color;
        isAttacker = nodeData.isAttacker;
        latitude = nodeData.latitude;
        longitude = nodeData.longitude;
        altitude = nodeData.altitude;
        speed= nodeData.speed;
        command= nodeData.command;

        layersId = nodeData.layersId;
        fieldData = nodeData.fieldData;
    }
//    NodeData(const NodeData&& nodData){

//    }
    void operator= (const NodeData& nodeData){
        id= nodeData.id;
        name= nodeData.name;
        type= nodeData.type;
        category = nodeData.category;
        url2D= nodeData.url2D;
        url3D= nodeData.url3D;
        imgInfoUrl = nodeData.imgInfoUrl;
        iconInfoUrl = nodeData.iconInfoUrl;
        color= nodeData.color;
        isAttacker = nodeData.isAttacker;
        latitude = nodeData.latitude;
        longitude = nodeData.longitude;
        altitude = nodeData.altitude;
        speed= nodeData.speed;
        command= nodeData.command;

        layersId = nodeData.layersId;
        fieldData = nodeData.fieldData;
    }
    int id;
    QString name;
    QString type{NodeType::Fixed};
    QString category;
    QString url2D;
    QString url3D;
    QString imgInfoUrl;
    QString iconInfoUrl;
    QString color;
    bool isAttacker{false};
    double latitude;
    double longitude;
    double altitude;
    double speed{0};
    QString command{Command::Add};
    std::vector<int> layersId;
    std::vector<NodeFieldData> fieldData;


    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("Name", name);
        jsonObject.insert("Type", type);
        jsonObject.insert("Category", category);
        jsonObject.insert("Url2D", url2D);
        jsonObject.insert("Url3D", url3D);
        jsonObject.insert("ImgInfoUrl", imgInfoUrl);
        jsonObject.insert("IconInfoUrl", iconInfoUrl);
        jsonObject.insert("Color", color);
        jsonObject.insert("IsAttacker", isAttacker);
        jsonObject.insert("Latitude", latitude);
        jsonObject.insert("Longitude", longitude);
        jsonObject.insert("Altitude", altitude);
        jsonObject.insert("Speed", speed);
        jsonObject.insert("Command", command);

        QJsonArray jsonArrayLayer;
        for(auto& layerId: layersId)
            jsonArrayLayer.append(layerId);
        jsonObject.insert("LayersId", jsonArrayLayer);

        QJsonArray jsonArrayField;
        for(auto& field: fieldData)
            jsonArrayField.append(field.toJson());
        jsonObject.insert("FieldData", jsonArrayField);

        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        id = json["Id"].toInt();
        name = json["Name"].toString();
        type = json["Type"].toString();
        category = json["Category"].toString();
        url2D = json["Url2D"].toString();
        url3D = json["Url3D"].toString();
        imgInfoUrl = json["ImgInfoUrl"].toString();
        iconInfoUrl = json["IconInfoUrl"].toString();
        color = json["Color"].toString();
        isAttacker = json["IsAttacker"].toBool();
        latitude = json["Latitude"].toDouble();
        longitude = json["Longitude"].toDouble();
        altitude = json["Altitude"].toDouble();
        speed = json["Speed"].toDouble();
        command = json["Command"].toString();

        for (const QJsonValue &value : json["LayersId"].toArray()) {
            layersId.push_back(value.toInt());
        }

        for (const QJsonValue &value : json["FieldData"].toArray()) {
            NodeFieldData newFieldData;
            newFieldData.fromJson(value.toObject());
            fieldData.push_back(newFieldData);
        }
    }
};

struct StatusNodeData
{
    int id{-1};
    QString name;
    double longitude;
    double latitude;
    double altitude;
    QString command{Command::Add};
    int layerId;
    std::vector<NodeFieldData> fieldData;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("Name", name);
        jsonObject.insert("Longitude", longitude);
        jsonObject.insert("Latitude", latitude);
        jsonObject.insert("Altitude", altitude);
        jsonObject.insert("Command", command);
        jsonObject.insert("LayerId", layerId);

        QJsonArray jsonArrayField;
        for(auto& field: fieldData)
            jsonArrayField.append(field.toJson());
        jsonObject.insert("FieldData", jsonArrayField);

        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        id = json["Id"].toInt();
        name = json["Name"].toString();
        longitude = json["Longitude"].toDouble();
        latitude = json["Latitude"].toDouble();
        altitude = json["Altitude"].toDouble();
        command = json["Command"].toString();
        layerId = json["LayerId"].toInt();

        for (const QJsonValue &value : json["FieldData"].toArray()) {
            NodeFieldData newFieldData;
            newFieldData.fromJson(value.toObject());
            fieldData.push_back(newFieldData);
        }
    }
};

struct AssignmentData
{
    int attackerId;
    int targetId;
    QString command{Command::Add};
    QString state{AssignmentState::Assinged};
    int layerId;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("AttackerId", attackerId);
        jsonObject.insert("TargetId", targetId);
        jsonObject.insert("Command", command);
        jsonObject.insert("State", state);
        jsonObject.insert("LayerId", layerId);

        return jsonObject;
    }

    void fromJson(QJsonObject jsonObject)
    {
        attackerId = jsonObject.value("AttackerId").toInt();
        targetId = jsonObject.value("TargetId").toInt();
        state = jsonObject.value("Command").toString();
        command = jsonObject.value("State").toString();
        layerId = jsonObject.value("LayerId").toInt();
    }
};

struct PolyLineData
{
    int id;
    QString name;
    QString color;
    int width;
    QString command{Command::Add};
    int layerId;
    std::vector<osg::Vec3d> points;

    virtual QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("Name", name);
        jsonObject.insert("Color", color);
        jsonObject.insert("Width", width);
        jsonObject.insert("Command", command);
        jsonObject.insert("LayerId", layerId);

        QJsonArray pointArray;
        for(auto& point: points){
            QJsonObject pointObject;
            pointObject.insert("Longitude", point.x());
            pointObject.insert("Latitude", point.y());
            pointObject.insert("Altitude", point.z());
            pointArray.push_back(pointObject);
        }
        jsonObject.insert("Points", pointArray);

        return jsonObject;
    }

    virtual void fromJson(const QJsonObject &json)
    {
        id = json["Id"].toInt();
        name = json["Name"].toString();
        color = json["Color"].toString();
        width = json["Width"].toInt();
        command = json["Command"].toString();
        layerId = json["LayerId"].toInt();

        for (const QJsonValue &value : json["Points"].toArray()) {
            float longitude = value["Longitude"].toDouble();
            float latitude = value["Latitude"].toDouble();
            float altitude = value["Altitude"].toDouble();
            points.push_back(osg::Vec3d{longitude, latitude, altitude});
        }
    }
};

struct PolygonData: public PolyLineData
{
    QString fillColor;

    QJsonObject toJson() const{
        QJsonObject jsonObject = PolyLineData::toJson();
        jsonObject.insert("FillColor", fillColor);

        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        PolyLineData::fromJson(json);
        fillColor = json["FillColor"].toString();
    }
};

struct CircleData
{
    int id;
    QString name;
    QString color;
    QString strokeColor;
    double strokeWidth;
    double latitude;
    double longitude;
    double altitude;
    double radius;
    QString command{Command::Add};
    int layerId;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("Name", name);
        jsonObject.insert("Color", color);
        jsonObject.insert("StrokeColor", strokeColor);
        jsonObject.insert("StrokeWidth", strokeWidth);
        jsonObject.insert("Latitude", latitude);
        jsonObject.insert("Longitude", longitude);
        jsonObject.insert("Altitude", altitude);
        jsonObject.insert("Radius", radius);
        jsonObject.insert("Command", command);
        jsonObject.insert("LayerId", layerId);

        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        id = json["Id"].toInt();
        name = json["Name"].toString();
        color = json["Color"].toString();
        strokeColor = json["StrokeColor"].toString();
        strokeWidth = json["StrokeWidth"].toDouble();
        latitude = json["Latitude"].toDouble();
        longitude = json["Longitude"].toDouble();
        altitude = json["Altitude"].toDouble();
        radius = json["Radius"].toDouble();
        command = json["Command"].toString();
        layerId = json["LayerId"].toInt();
    }
};

struct ExplosionData
{
    int id;
    QString name;
    double latitude;
    double longitude;
    double altitude;
    double duration;
    double scale;
    QString command{Command::Add};
    int layerId;
    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("Name", name);
        jsonObject.insert("Latitude", latitude);
        jsonObject.insert("Longitude", longitude);
        jsonObject.insert("Altitude", altitude);
        jsonObject.insert("Duration", duration);
        jsonObject.insert("Scale", scale);
        jsonObject.insert("Command", command);
        jsonObject.insert("LayerId", layerId);

        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        id = json["Id"].toInt();
        name = json["Name"].toString();
        latitude = json["Latitude"].toDouble();
        longitude = json["Longitude"].toDouble();
        altitude = json["Altitude"].toDouble();
        duration = json["Duration"].toDouble();
        scale = json["Scale"].toDouble();
        command = json["Command"].toString();
        layerId = json["LayerId"].toInt();
    }
};

struct LayerData {
    int id;
    int parentId;
    QString text;
    int order;
    bool isComposite{false};
    QString command{Command::Add};
    std::vector<LayerData> children;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("ParentId", parentId);
        jsonObject.insert("Text", text);
        jsonObject.insert("Order", order);
        jsonObject.insert("IsComposite", isComposite);
        jsonObject.insert("Command", command);
        QJsonArray childrenArray;
        for(const LayerData& child: children){
            childrenArray.push_back(child.toJson());
        }
        jsonObject.insert("Children", childrenArray);
        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        id = json["Id"].toInt();
        parentId = json["ParentId"].toInt();
        text = json["Text"].toString();
        order = json["Order"].toInt();
        isComposite = json["IsComposite"].toBool();
        command = json["Command"].toString();

        for (const QJsonValue &child : json["Children"].toArray()) {
            LayerData newLayerData;
            newLayerData.fromJson(child.toObject());
            children.push_back(newLayerData);
        }
    }
};

class ServiceManager: public QObject
{
    Q_OBJECT
public:
    ServiceManager(NetworkManager *networkManager, QObject *parent = nullptr);
    void sendAssignment(const AssignmentData &assignmentData);
    //    void signInData(QJsonObject jsonObject);
    //    void signUpData(QJsonObject jsonObject);
private slots:
    void onMessageReceived(const QString &message);
signals:
    void layerDataReceived(const LayerData &layerData);
    void nodeDataReceived(const NodeData &nodeData);
    void statusNodeDataReceived(const StatusNodeData &statusNodeData);
    void assignmentDataReceived(const AssignmentData &assignmentData);
    void polyLineDataReceived(const PolyLineData &polyLineData);
    void polygonDataReceived(const PolygonData &polygonData);
    void circleDataReceived(const CircleData &circleData);
    void explosionDataReceived(const ExplosionData &explosionData);

    //    void signUpResponseReceived(bool status);
    //    void signInResponseReceived(bool status, int role);
private:
    NetworkManager *mNetworkManager;
};

#endif // DATAMANAGER_H

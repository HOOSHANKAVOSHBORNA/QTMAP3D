#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "qjsonarray.h"
#include "qjsonobject.h"
#include <QJsonDocument>
#include <osg/Referenced>
#include <osg/Vec3d>

class NetworkManager;

struct ReadyForData {
    QString message{"Ready"};

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Message", message);
        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        message = json["Message"].toString();
    }
};

struct Command
{
    enum Cmd{
        Add,
        Update,
        Remove
    };
    Command(){command = Add;}
    Command(Cmd){command = Add;}
    Command(QString str){fromString(str);}
    Cmd command;
    QString toString() const{
        switch (command) {
        case Add:
            return "Add";
            break;
        case Update:
            return "Update";
            break;
        case Remove:
            return "Remove";
            break;
        default:
            return "Add";
            break;
        }
    }
    void fromString(const QString& str){
        if(str == "Add")
            command = Add;
        else if(str == "Update")
            command = Update;
        else if (str == "Remove")
            command = Remove;
        else
            command = Add;
    }
    bool operator== (const Cmd& c)const{
        return c == command;
    }
    void operator= (const Cmd& c){
        command = c;
    }
};

struct NodeType
{
    enum Type{
        Fixed,
        Movable,
        Flyable
    };
    NodeType(){this->type =  Fixed;}
    NodeType(Type type){this->type =  type;}
    NodeType(QString str){ fromString(str);}
    Type type;
    QString toString() const{
        switch (type) {
        case Fixed:
            return "Fixed";
            break;
        case Movable:
            return "Movable";
            break;
        case Flyable:
            return "Flyable";
            break;
        default:
            return "Fixed";
            break;
        }
    }
    void fromString(const QString& str){
        if(str == "Fixed")
            type = Fixed;
        else if(str == "Movable")
            type = Movable;
        else if (str == "Flyable")
            type = Flyable;
        else
            type = Fixed;
    }
    bool operator== (const Type& t)const{
        return t == type;
    }
    void operator= (const Type& t){
        type = t;
    }
};

struct AssignmentState
{
    enum State{
        Assigned,
        Search,
        Lock,
        Fire,
        Success,
        Failed
    };
    AssignmentState(){this->state =  Assigned;}
    AssignmentState(State state){this->state =  state;}
    AssignmentState(QString str){ fromString(str);}
    State state;
    QString toString() const{
        switch (state) {
        case Assigned:
            return "Assigned";
            break;
        case Search:
            return "Search";
            break;
        case Lock:
            return "Lock";
            break;
        case Fire:
            return "Fire";
            break;
        case Success:
            return "Success";
            break;
        case Failed:
            return "Failed";
            break;
        default:
            return "Assigned";
            break;
        }
    }
    void fromString(const QString& str){
        if(str == "Assigned")
            state = Assigned;
        else if(str == "Search")
            state = Search;
        else if (str == "Lock")
            state = Lock;
        else if (str == "Fire")
            state = Fire;
        else if (str == "Success")
            state = Success;
        else if (str == "Failed")
            state = Failed;
        else
            state = Assigned;
    }
    bool operator== (const State& s)const{
        return s == state;
    }
    void operator= (const State& s){
        state = s;
    }
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
    NodeType type{NodeType::Fixed};
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
    Command command{Command::Add};
    std::vector<int> layersId;
    std::vector<NodeFieldData> fieldData;


    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("Name", name);
        jsonObject.insert("Type", type.toString());
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
        jsonObject.insert("Command", command.toString());

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

        auto layers = json["LayersId"].toArray();
        for (const QJsonValue &value : layers) {
            layersId.push_back(value.toInt());
        }

        auto fieldDataList = json["FieldData"].toArray();
        for (const QJsonValue &value : fieldDataList) {
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
    Command command{Command::Add};
    int layerId;
    std::vector<NodeFieldData> fieldData;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("Name", name);
        jsonObject.insert("Longitude", longitude);
        jsonObject.insert("Latitude", latitude);
        jsonObject.insert("Altitude", altitude);
        jsonObject.insert("Command", command.toString());
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

        auto fieldDataList = json["FieldData"].toArray();
        for (const QJsonValue &value : fieldDataList) {
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
    Command command{Command::Add};
    AssignmentState state{AssignmentState::Assigned};
    int layerId;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("AttackerId", attackerId);
        jsonObject.insert("TargetId", targetId);
        jsonObject.insert("Command", command.toString());
        jsonObject.insert("State", state.toString());
        jsonObject.insert("LayerId", layerId);

        return jsonObject;
    }

    void fromJson(QJsonObject jsonObject)
    {
        attackerId = jsonObject.value("AttackerId").toInt();
        targetId = jsonObject.value("TargetId").toInt();
        command = jsonObject.value("Command").toString();
        state = jsonObject.value("State").toString();
        layerId = jsonObject.value("LayerId").toInt();
    }
};

struct PolyLineData
{
    int id;
    QString name;
    QString color;
    int width;
    Command command{Command::Add};
    int layerId;
    std::vector<osg::Vec3d> points;

    virtual QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("Name", name);
        jsonObject.insert("Color", color);
        jsonObject.insert("Width", width);
        jsonObject.insert("Command", command.toString());
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

        auto pointsArray = json["Points"].toArray();
        for (const QJsonValue &value : pointsArray) {
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
    Command command{Command::Add};
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
        jsonObject.insert("Command", command.toString());
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
    Command command{Command::Add};
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
        jsonObject.insert("Command", command.toString());
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
    Command command{Command::Add};
    std::vector<LayerData> children;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("ParentId", parentId);
        jsonObject.insert("Text", text);
        jsonObject.insert("Order", order);
        jsonObject.insert("IsComposite", isComposite);
        jsonObject.insert("Command", command.toString());
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

        auto chArray = json["Children"].toArray();
        for (const QJsonValue &child : chArray) {
            LayerData newLayerData;
            newLayerData.fromJson(child.toObject());
            children.push_back(newLayerData);
        }
    }
};

struct Response
{
    struct Status
    {
        static inline const QString Success{"Success"};
        static inline const QString Failed{"Failed"};
    };

    QString status;
    QString message;
    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Status", status);
        jsonObject.insert("Message", message);
        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        status = json["Status"].toString();
        message = json["Message"].toString();
    }
};

struct UserData
{
    struct UserCommand
    {
        static inline const QString Login{"Login"};
        static inline const QString Logout{"Logout"};
        static inline const QString SelectRole{"SelectRole"};
    };
    QString name;
    QString userName;
    QString password;
    Response response;
    QString command;
    QVector<QString> roles;
    int selectRoleIndex;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Name", name);
        jsonObject.insert("UserName", userName);
        jsonObject.insert("Password", password);
        jsonObject.insert("Response", response.toJson());
        jsonObject.insert("Command", command);
        QJsonArray roleArray;
        for(const auto& role: roles){
            roleArray.push_back(role);
        }
        jsonObject.insert("Roles", roleArray);
        jsonObject.insert("SelectRoleIndex", selectRoleIndex);
        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        name = json["Name"].toString();
        userName = json["UserName"].toString();
        password = json["Password"].toString();
        Response res;
        res.fromJson(json["Response"].toObject());
        response = res;
        command = json["Command"].toString();

        auto roleArray = json["Roles"].toArray();
        for (const QJsonValue &role : roleArray) {
            roles.push_back(role.toString());
        }
        selectRoleIndex = json["SelectRoleIndex"].toInt();
    }

};

class ServiceManager: public QObject
{
    Q_OBJECT
public:
    ServiceManager(NetworkManager *networkManager, QObject *parent = nullptr);
    void sendAssignment(const AssignmentData &assignmentData);
    void sendUser(const UserData &UserData);
    void sendReady(const ReadyForData &readyForData);

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
    void userDataReceived(const UserData &UserData);

private:
    NetworkManager *mNetworkManager;
};

#endif // DATAMANAGER_H

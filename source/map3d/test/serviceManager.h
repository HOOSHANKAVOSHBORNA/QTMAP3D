#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "networkManager.h"
#include "qjsonarray.h"
#include "qvectornd.h"
#include <QJsonDocument>
#include <QJsonObject>

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

struct AssignmentState{
    static inline QString Assinged{"Assigned"};
    static inline QString Search{"Search"};
    static inline QString Lock{"Lock"};
    static inline QString Fire{"Fire"};
    static inline QString Succeed{"Succeed"};
    static inline QString Failed{"Failed"};
};

struct NodeFieldData
{
    QString name;
    QVariant value;
    QString category;
    QString categoryIconUrl;
    bool operator==(const QString& n) const{
        return n == name;
    }

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Name", name);
        jsonObject.insert("Value", value.toJsonValue());
        jsonObject.insert("Category", category);
        jsonObject.insert("CategoryIconUrl", categoryIconUrl);

        return jsonObject;
    }
};

struct NodeData
{
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
    double heading{0};
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
        jsonObject.insert("Heading", heading);
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
    void fromJson(QJsonObject jsonObject){
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
    std::vector<QVector3D> points;

    QJsonObject toJson() const{
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
};

struct PolygonData: public PolyLineData
{
    QString fillColor;

    QJsonObject toJson() const{
        QJsonObject jsonObject = PolyLineData::toJson();
        jsonObject.insert("FillColor", fillColor);

        return jsonObject;
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
};

struct LayerData {
    int id;
    int parentId;
    QString text;
    int order;
    QString command{Command::Add};
    bool isComposite{false};
    std::vector<LayerData> children;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Id", id);
        jsonObject.insert("ParentId", parentId);
        jsonObject.insert("Text", text);
        jsonObject.insert("Order", order);
        jsonObject.insert("Command", command);
        jsonObject.insert("IsComposite", isComposite);
        QJsonArray childrenArray;
        for(const LayerData& child: children){
            childrenArray.push_back(child.toJson());
        }
        jsonObject.insert("Children", childrenArray);
        return jsonObject;
    }
};

struct Response
{
    struct Status
    {
        QString Success{"Success"};
        QString Failed{"Failed"};
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
        QString Login{"Login"};
        QString Logout{"Logout"};
        QString Update{"Update"};
    };
    QString name;
    QString userName;
    QString password;
    QString confirmPasword;
    QString token;
    Response response;
    QString command;
    std::vector<QString> roles;

    QJsonObject toJson() const{
        QJsonObject jsonObject;
        jsonObject.insert("Name", name);
        jsonObject.insert("UserName", userName);
        jsonObject.insert("Password", password);
        jsonObject.insert("ConfirmPasword", confirmPasword);
        jsonObject.insert("Token", token);
        jsonObject.insert("Response", response.toJson());
        jsonObject.insert("Command", command);
        QJsonArray roleArray;
        for(const auto& role: roles){
            roleArray.push_back(role);
        }
        jsonObject.insert("Roles", roleArray);
        return jsonObject;
    }

    void fromJson(const QJsonObject &json)
    {
        name = json["Name"].toString();
        userName = json["UserName"].toString();
        password = json["Password"].toString();
        confirmPasword = json["ConfirmPasword"].toString();
        token = json["Token"].toString();
        Response res;
        res.fromJson(json["Response"].toObject());
        response = res;
        command = json["Command"].toString();

        auto roleArray = json["Roles"].toArray();
        for (const QJsonValue &role : roleArray) {
            roles.push_back(role.toString());
        }
    }

};

class ServiceManager: public QObject
{
    Q_OBJECT
public:
    ServiceManager(NetworkManager *networkManager, QObject *parent = nullptr);

    void sendLayer(const LayerData &layerData);
    void sendNode(const NodeData &nodeData);
    void sendStatusNode(const StatusNodeData &statusNodeData);
    void sendAssignment(const AssignmentData &assignmentData);
    void sendPolyLine(const PolyLineData &polyLineData);
    void sendPolygon(const PolygonData &polygonData);
    void sendCircle(const CircleData &circleData);
    void sendExplosion(const ExplosionData &explosionData);
    void sendUser(const UserData &userData);

    bool isReadyForSendData() const;

private slots:
    void onMessageReceived(const QString &message);

signals:
    void assignmentDataReceived(const AssignmentData &assignmentData);
    void userDataReceived(const UserData &userData);
    void readyForSendData();

private:
    NetworkManager *mNetworkManager{nullptr};
    bool mIsReadyForSendData{false};

};

#endif // DATAMANAGER_H

#ifndef AIRCRAFTTABLEMODEL_H
#define AIRCRAFTTABLEMODEL_H

#include <QAbstractTableModel>
#include <deque>
#include <QSharedPointer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

struct AircraftInfo
{
    QString TN;
    QString IFFCode;
    QString CallSign;
    QString Type;
    QString MasterRadar;
    QString Identification;
    QString IdentificationMethod;
    QString Time;
    QString Pos;

    double Latitude;
    double Longitude;
    double Altitude;
    double Heading;
    double Speed;

    QStringList DetectionSystems;
    QStringList Sends;

    QString detectionSystemsToString()
    {
        QString result = "";
        for(auto detectSystem: DetectionSystems)
            result += detectSystem + ", ";
        return result;
    }
    QString sendsToString()
    {
        QString result = "";
        for(auto send: Sends)
            result += send + ", ";
        return result;
    }

    QJsonDocument toJson()
    {
        QJsonObject jsonObject;
        jsonObject.insert("TN", TN);
        jsonObject.insert("IFFCode", IFFCode);
        jsonObject.insert("CallSign", CallSign);
        jsonObject.insert("Type", Type);
        jsonObject.insert("MasterRadar", MasterRadar);
        jsonObject.insert("Identification", Identification);// F, K, Z, X, U, H
        jsonObject.insert("IdentificationMethod", IdentificationMethod);//3 char
        jsonObject.insert("Time", Time);//epoch
        jsonObject.insert("Pos", Pos);

        jsonObject.insert("Latitude", Latitude);
        jsonObject.insert("Longitude", Longitude);
        jsonObject.insert("Altitude", Altitude);//meter
        jsonObject.insert("Heading", Heading);
        jsonObject.insert("Speed", Speed);//m/s
        //
        QJsonArray detectSystems;
        for(auto detectSystem: DetectionSystems)
            detectSystems.push_back(detectSystem);
        jsonObject.insert("DetectionSystem", detectSystems);
        //
        QJsonArray sends;
        for(auto send: Sends)
            sends.push_back(send);
        jsonObject.insert("Send", sends);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonObject);
        return jsonDoc;
    }

    void fromJson(QJsonDocument jsonDoc)
    {
        QJsonObject data = jsonDoc.object();
        TN = QString::number(data.value("TN").toInt());
        IFFCode = data.value("IFFCode").toString();
        CallSign = data.value("CallSign").toString();
        Type = data.value("Type").toString();
        MasterRadar = data.value("MasterRadar").toString();
        Identification = data.value("Identification").toString();
        IdentificationMethod = data.value("IdentificationMethod").toString();
        Time = data.value("Time").toString();
        Pos = data.value("Pos").toString();

        Latitude = data.value("Latitude").toDouble();
        Longitude = data.value("Longitude").toDouble();
        Altitude = data.value("Altitude").toDouble();
        Heading = data.value("Heading").toDouble();
        Speed = data.value("Speed").toDouble();

        QJsonArray detectSystems = data.value("DetectionSystem").toArray();
        for(auto detectSystem: detectSystems)
            DetectionSystems.append(detectSystem.toString());

        QJsonArray sends = data.value("Send").toArray();
        for(auto send: sends)
            Sends.append(send.toString());
    }
};

class AircraftTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AircraftTableModel(QObject * parent = nullptr);

    enum CustomRoles {
        BackColorRole = Qt::UserRole + 100,
        TextColorRole = Qt::UserRole + 101,
        HeaderTextRole = Qt::UserRole + 102
    };

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE QString headerText(int column) const;

    Q_INVOKABLE QString getTN(int row) const;

public slots:
    void setFilterWildcard(const QString& wildcard);


public:
    void updateItemData(const QString& jsonStr);
    void updateItemData(const AircraftInfo& aircraftInfo);

private:
    std::deque<QSharedPointer<AircraftInfo>> mAircraftInfoList;
    std::deque<QSharedPointer<AircraftInfo>> mAircraftInfoListProxy;

    QString mFilter;
};

#endif // AIRCRAFTTABLEMODEL_H

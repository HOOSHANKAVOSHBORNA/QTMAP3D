#ifndef DEFENSEDATAMANAGER_H
#define DEFENSEDATAMANAGER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QObject>
#include <QColor>

struct SystemInfo;
struct AircraftInfo
{
    enum Identify{
        F,//green
        K,//yellow
        Z,//orange
        X,//red
        U,//white
        H//red
    };
    //-------------------------------------
    int TN;
    QString IFFCode;
    QString CallSign;
    QString Type;
    QString MasterRadar;
    Identify Identification;
    QString IdentificationMethod;
    long long int Time;
    QString Pos;

    double Latitude;
    double Longitude;
    double Altitude;
    double Heading;
    double Speed;

    QStringList DetectionSystems;
    QStringList Sends;

    QList<SystemInfo> assignedSystems;
    friend bool operator==(const AircraftInfo& lhs, const AircraftInfo& rhs) { return lhs.TN == rhs.TN; }
    //---------------------------------------------
public:
    QString identifyToString() const
    {
        QString result = "";
        switch (Identification) {
        case F:
            result = "F";
            break;
        case K:
            result = "K";
            break;
        case Z:
            result = "Z";
            break;
        case X:
            result = "X";
            break;
        case U:
            result = "U";
            break;
        case H:
            result = "H";
            break;
        }
        return result;
    }

    QColor aircraftColor() {
        QColor color = QColor(Qt::transparent);
        switch (Identification) {
        case F:
            color = QColor("green");
            break;
        case K:
            color = QColor("yellow");
            break;
        case Z:
            color = QColor("orange");
            break;
        case X:
            color = QColor("red");
            break;
        case U:
            color = QColor("white");
            break;
        case H:
            color = QColor("red");
            break;
        }
        return color;
    }

    QColor aircraftHoverColor() {
        QColor color = QColor(Qt::transparent);
        switch (Identification) {
        case F:
            color = QColor("darkgreen");
            break;
        case K:
            color = QColor("darkyellow");
            break;
        case Z:
            color = QColor("darkorange");
            break;
        case X:
            color = QColor("darkred");
            break;
        case U:
            color = QColor("gray");
            break;
        case H:
            color = QColor("darkred");
            break;
        }
        return color;
    }

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


};

struct StationInfo
{
    QString Name;
    QString Type;
    QString PrimSec;
    bool Active;

    int Number;
    double Latitude;
    double Longitude;
    double Radius{500000};
    int CycleTime;

    QJsonDocument toJson()
    {
        QJsonObject jsonObject;
        jsonObject.insert("Name", Name);
        jsonObject.insert("Type", Type);
        jsonObject.insert("Primary/Secondary", PrimSec);
        jsonObject.insert("Active", Active);

        jsonObject.insert("Number", Number);
        jsonObject.insert("Latitude", Latitude);
        jsonObject.insert("Longitude", Longitude);
        jsonObject.insert("Radius", Radius);
        jsonObject.insert("CycleTime", CycleTime);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonObject);
        return jsonDoc;
    }

    void fromJson(QJsonDocument jsonDoc)
    {
        QJsonObject data = jsonDoc.object();
        Name = data.value("Name").toString();
        Type = data.value("Type").toString();
        PrimSec = data.value("Primary/Secondary").toString();
        Active = data.value("Active").toBool();

        Number = data.value("Number").toInt();
        Latitude = data.value("Latitude").toDouble();
        Longitude = data.value("Longitude").toDouble();
        Radius = data.value("Radius").toDouble();
        CycleTime = data.value("CycleTime").toInt();

    }

};

struct SystemInfo
{
    QString Terminal;
    QString Name;
    int Number;
    QString Type;
    double Latitude;
    double Longitude;
    double Altitude;
    double ViewRange{300000};
    double MezRange{250000};
    bool Active;

    QList<AircraftInfo> assignedAircrafts;

    friend bool operator==(const SystemInfo& lhs, const SystemInfo& rhs) { return lhs.Number == rhs.Number; }
};

struct SystemStatusInfo
{
    enum RadarStatus{
        US,//disconnect
        S,//connect
        NoData
    };

    int Number;
    //status info
    QString ReceiveTime;
    QString Simulation;
    RadarStatus BCCStatus;
    RadarStatus RadarSearchStatus;
    QString Operational;
    int MissileCount;
    QString RadarMode;

    QString radarStatusToString(RadarStatus status) const
    {
        QString result = "";
        switch (status) {
        case US:
            result = "US";
            break;
        case S:
            result = "S";
            break;
        case NoData:
            result = "NoData";
            break;
        }
        return result;
    }
};
struct SystemCambatInfo
{
    enum Phases{
        Search,
        Lock,
        Fire,
        Kill,
        NoKill
    };
    int Number;
    //combat info
    int TN;
    QString Acceptance;
    Phases Phase;
    double Antenna;
    QString ChanelNo;
    QString Inrange;

    QString phaseToString() const
    {
        QString result = "";
        switch (Phase) {
        case Search:
            result = "Search";
            break;
        case Lock:
            result = "Lock";
            break;
        case Fire:
            result = "Fire";
            break;
        case Kill:
            result = "Kill";
            break;
        case NoKill:
            result = "NoKill";
            break;
        }
        return result;
    }

    QString phaseToColor() const
    {
        QString result = "white";
        switch (Phase) {
        case Search:
            result = "yellow";
            break;
        case Lock:
            result = "orange";
            break;
        case Fire:
            result = "red";
            break;
        case Kill:
            result = "black";
            break;
        case NoKill:
            result = "brown";
            break;
        }
        return result;
    }
};

class DefenseDataManager: public QObject
{
    Q_OBJECT
public:
    DefenseDataManager(QObject* parent = nullptr);
signals:
    //--aircraft-----------------------------------------------
    void aircraftInfoChanged(AircraftInfo& aircraftInfo);
    void clearAircraft(int tn);
    void aircraftAssignedResponse(int tn, int systemNo, bool result);
    //--send command---------------------------------------------
    void aircraftAssigned(int tn, int systemNo);
    void cancelAircraftAssigned(int tn, int systemNo);
    //--system-------------------------------------------------
    void systemInfoChanged(SystemInfo& systemInfo);
    void systemStatusInfoChanged(SystemStatusInfo& systemStatusInfo);
    void systemCambatInfoChanged(SystemCambatInfo& systemCambatInfo);
    //--station------------------------------------------------
    void stationInfoChanged(StationInfo& stationInfo);
public slots:
};

class Demo
{
public:
    Demo(DefenseDataManager* defenseDataManager);
    ~Demo();
    AircraftInfo createAircraftInfo();
    void updateAircraftInfo();
    void createStationInfo();
    void createSystemInfo();
    void updateSystemCambatInfo();
public:
    DefenseDataManager* mDefenseDataManager;
    QList<AircraftInfo> mAircraftList;
    QList<StationInfo> stationList;

    QList<SystemInfo> systemList;
    QList<SystemStatusInfo> systemStatusList;
    QList<SystemCambatInfo> SystemCambatList;
};

#endif // DEFENSEDATAMANAGER_H

#ifndef DEFENSEDATAMANAGER_H
#define DEFENSEDATAMANAGER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QObject>
#include <QColor>
#include <osgEarth/Color>
#include <QMap>

struct SystemInfo;
struct AircraftInfo
{
    enum Identify{
		Begin, //for iterate on enum
		F = Begin,//green
        K,//yellow
        Z,//orange
        X,//red
        U,//white
		H,//red
		End = H //for iterate on enum
    };
    enum AircraftType{
        Fighter = 1,
        Missile = 2,
        Helicopter = 3,
        Drone = 4,
        Aircraft = 5
    };

	//-------------------------------------QColor
    int TN = 0;
    QString IFFCode = "------";
    QString CallSign = "------";
    AircraftType Type;
    QString MasterRadar = "------";
    Identify Identification;
    QString IdentificationMethod = "------";
    long long int Time;
    QString Pos = "------";

    double Latitude = -1;
    double Longitude = -1;
    double Altitude = -1;
    double Heading = -1;
    double Speed = -1;

    QStringList DetectionSystems;
    QStringList Sends;

    QList<SystemInfo> assignedSystems;
    friend bool operator==(const AircraftInfo& lhs, const AircraftInfo& rhs) { return lhs.TN == rhs.TN; }
    //---------------------------------------------
public:
    QString aircraftTypeToString() const
    {
        QString result = "------";
        switch (Type) {
        case Fighter:
            result = "Fighter";
            break;
        case Missile:
            result = "Missile";
            break;
        case Helicopter:
            result = "Helicopter";
            break;
        case Drone:
            result = "Drone";
            break;
        case Aircraft:
            result = "Aircraft";
            break;
        }
        return result;
    }

    QString identifyToString() const
    {
        QString result = "------";
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

	static QColor osgEarthColor2QColor(osgEarth::Color color)  {
		return QColor(color.r()*255, color.g()*255, color.b()*255, color.a()*255);
	}
	static osgEarth::Color qColor2osgEarthColor(QColor color){
		return osgEarth::Color(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	}
	static QColor aircraftColor(Identify identification) {
        QColor color = QColor(Qt::transparent);
		switch (identification) {
        case F:
			color = QColor(51, 204, 51, 255);//green
            break;
        case K:
			color = QColor(204, 204, 51, 255);//yellow
            break;
        case Z:
			color = QColor(204, 127, 51, 255);//orange
            break;
        case X:
			color = QColor(204, 51, 51, 255);//red
            break;
        case U:
			color = QColor(204, 204, 204, 255);//white
            break;
        case H:
			color = QColor(204, 51, 51, 255);//red
            break;
        }
        return color;
    }

	QColor aircraftColor() const{
		return aircraftColor(Identification);
	}

    QColor aircraftHoverColor() const {
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

    QString detectionSystemsToString() const
    {
        QString result = "";
        for(auto detectSystem: DetectionSystems)
            result += detectSystem + ", ";
        return result;
    }
    QString sendsToString() const
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

    enum RadarStatus{
        US,//disconnect
        S,//connect
        NoData
    };

    QString Name = "------";
    QString Type = "------";
    QString PrimSec = "------";
//    bool Active = false;

    int Number = -1;
    double Latitude = -1;
    double Longitude = -1;
    double Altitude = -1;
    double Radius = 0;
    int CycleTime = -1;
    RadarStatus RadarSearchStatus;

    QColor statusToColor(RadarStatus status) const {
        switch (status) {
            case S: return QColor("green");
            case US: return QColor("red");
            default: return QColor("red");
        }
    }

    QString radarStatusToString(RadarStatus status) const
    {
        QString result = "------";
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

    QJsonDocument toJson()
    {
        QJsonObject jsonObject;
        jsonObject.insert("Name", Name);
        jsonObject.insert("Type", Type);
        jsonObject.insert("Primary/Secondary", PrimSec);
//        jsonObject.insert("Active", Active);

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
//        Active = data.value("Active").toBool();

        Number = data.value("Number").toInt();
        Latitude = data.value("Latitude").toDouble();
        Longitude = data.value("Longitude").toDouble();
        Radius = data.value("Radius").toDouble();
        CycleTime = data.value("CycleTime").toInt();

    }

};

struct SystemInfo
{
    QString Terminal = "------";
    QString Name = "------";
    int Number = -1;
    QString Type = "------";
    double Latitude = -1;
    double Longitude = -1;
    double Altitude = -1;
    double ViewRange = -1;
    double MezRange = -1;
//    bool Active = true;

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

    enum OperationStatus {
        NOP = 0,
        OP1  = 1,
        OP2  = 2
    };


    int Number = -1;
    //status info
    QString ReceiveTime = "------";
    QString Simulation = "------";
    RadarStatus BCCStatus;
    RadarStatus RadarSearchStatus;
    OperationStatus Operational = NOP;
    int MissileCount = -1;
    QString RadarMode = "------";
    QString BCCStatusToString() const {
        return radarStatusToString(BCCStatus);
    }
    QColor statusToColor(RadarStatus status) const {
        switch (status) {
            case S: return QColor("green");
            case US: return QColor("red");
            default: return QColor("red");
        }
    }
    QString radarSearchStatusToString() const{
        return radarStatusToString(RadarSearchStatus);
    }
    QString radarStatusToString(RadarStatus status) const
    {
        QString result = "------";
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

    QString operationalToString() const {

        QString result = "NOP";
        switch(Operational) {
        case NOP:
            result = "NOP";
            break;
        case OP1:
            result = "OP1";
            break;
        case OP2:
            result = "OP2";
            break;
        }

        return result;
    }
};
struct SystemCombatInfo
{
    enum Phases{
        Search,
        Lock,
        Fire,
        Kill,
        NoKill
    };
    int Number = -1;
    //combat info
    int TN = -1;
    QString Acceptance = "------";
    Phases Phase = Search;
    double Antenna = -1;
    QString ChanelNo = "------";
    QString Inrange = "------";

    QString phaseToString() const
    {
        QString result = "------";
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

//    QString phaseToColor() const
//    {
//        QString result = "white";
//        switch (Phase) {
//        case Search:
//            result = "yellow";
//            break;
//        case Lock:
//            result = "orange";
//            break;
//        case Fire:
//            result = "red";
//            break;
//        case Kill:
//            result = "black";
//            break;
//        case NoKill:
//            result = "brown";
//            break;
//        }
//        return result;
//    }
	QColor phaseToColor() const
	{
		QColor result = QColor("white");
		switch (Phase) {
		case Search:
			result = QColor("yellow");
			break;
		case Lock:
			result = QColor("orange");
			break;
		case Fire:
			result = QColor("red");
			break;
		case Kill:
			result = QColor("black");
			break;
		case NoKill:
			result = QColor("brown");
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
    void systemCombatInfoChanged(SystemCombatInfo& systemCombatInfo);
    //--station------------------------------------------------
    void stationInfoChanged(StationInfo& stationInfo);
public slots:
};

class Demo
{
public:
    Demo(DefenseDataManager* defenseDataManager);
    ~Demo();
    void createAircraftInfo();
    void updateAircraftInfo();

    void createSystemInfo();
    void updateSystemCombatInfo();

    void createStationInfo();
public:
    struct SystemInformation{
        SystemInfo info;
        SystemStatusInfo statusInfo;
        SystemCombatInfo combatInfo;
    };

    DefenseDataManager* mDefenseDataManager;

    QMap<int, AircraftInfo> mAircrafts;
    QMap<int, QList<SystemInformation*>> mAircrafAssignment;

    QMap<int, SystemInformation> mSystems;
    QMap<int, QList<AircraftInfo*>> mSystemAssignment;
    QMap<int, StationInfo> mStations;

//    QList<AircraftInfo> mAircraftList;
    //QList<StationInfo> stationList;

    //QList<SystemInfo> systemList;
    //QList<SystemStatusInfo> systemStatusList;
    //QList<SystemCombatInfo> SystemCombatList;
};

#endif // DEFENSEDATAMANAGER_H

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include "aircraftTableModel.h"
#include "stationTableModel.h"
#include "systemTableModel.h"
#include "assignmentModel.h"

class UIHandle;
class QQmlEngine;
class AircraftTableModel;

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent = nullptr);

signals:
    void aircraftItemClicked(const QString& TN);
    void stationItemClicked(const QString& Name);


public slots:
    void setAircraftInfo(const AircraftInfo& aircraftInof);
    void deleteAircraftInfo(int TN);
    void setStationInfo(const StationInfo& stationInfo);
    void setSystemInfo(const SystemInfo& systemInfo);
    void setSystemCombatInfo(const SystemCambatInfo &systemCombatInfo);
    void setSystemStatusInfo(const SystemStatusInfo &systemStatusInfo);
    void assignAirToSystem(AircraftInfo aircraft, SystemInfo system);
    void cancelAssign(int TN, int Number);
    void clearAll();


signals:
    void aircraftDoubleClicked(int NT);
    void stationDoubleClicked(int number);
    void systemDoubleClicked(int number);
    void systemActiveToggled(bool act);


private:
    QQmlEngine *mQmlEngine = nullptr;
    UIHandle *mUiHandle = nullptr;
    AircraftTableModel *mAircraftTableModel = nullptr;
    StationTableModel *mStationTableModel = nullptr;
    SystemTableModel *mSystemTableModel = nullptr;
    AssignmentModel *mAssignModel = nullptr;

};

#endif // DATAMANAGER_H

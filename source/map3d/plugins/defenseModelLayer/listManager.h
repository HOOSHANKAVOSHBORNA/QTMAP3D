#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include <QObject>
#include "aircraftTableModel.h"
#include "stationTableModel.h"
#include "systemTableModel.h"

class UIHandle;
class QQmlEngine;
class AircraftTableModel;

class ListManager : public QObject
{
    Q_OBJECT
public:
    explicit ListManager(QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent = nullptr);

signals:
    void aircraftItemClicked(const QString& TN);
    void stationItemClicked(const QString& Name);


public slots:
    void setAircraftInfo(const AircraftInfo& aircraftInof);
    void deleteAircraftInfo(int TN);
    void setStationInfo(const StationInfo& stationInfo);
    void setSystemInfo(const SystemInfo& systemInfo);
    void setSystemCombatInfo(const SystemCombatInfo &systemCombatInfo);
    void setSystemStatusInfo(const SystemStatusInfo &systemStatusInfo);
    void assignAirToSystem(int TN, int Number);
    void cancelAssign(int TN, int Number);
    void acceptAssign(int TN, int Number, bool result);
    void cancelSystemAssignmentsExcept(int ExceptTN, int Number);
    void cancelAircraftAssignmentsExcept(int TN, int ExceptNum);
    void clearAllAssigns();
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

    AircraftTableModel *mAircraftAssignmentTableModel;
    SystemTableModel *mSystemAssignmentTableModel;

};

#endif // LISTMANAGER_H

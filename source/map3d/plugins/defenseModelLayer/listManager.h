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
    void setSystemCombatInfo(const SystemCambatInfo &systemCombatInfo);
    void setSystemStatusInfo(const SystemStatusInfo &systemStatusInfo);
    void assignAirToSystem(int TN, int Number);
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

};

#endif // LISTMANAGER_H

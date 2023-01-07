#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include "aircrafttablemodel.h"
#include "stationtablemodel.h"
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
    void stationDoubleClicked(const QString& Name);

public slots:
    void setAircraftInfo(const AircraftInfo& aircraftInof);
    void setStationInfo(const StationInfo& stationInfo);


signals:
    void aircraftDoubleClicked(const QString& NT);


private:
    QQmlEngine *mQmlEngine = nullptr;
    UIHandle *mUiHandle = nullptr;
    AircraftTableModel *mAircraftTableModel = nullptr;
    StationTableModel *mStationTableModel = nullptr;

};

#endif // DATAMANAGER_H

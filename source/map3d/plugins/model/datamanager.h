#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include "aircrafttablemodel.h"

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

public slots:
    void setAircraftInfo(const AircraftInfo& aircraftInof);


signals:
    void aircraftDoubleClicked(const QString& NT);


private:
    QQmlEngine *mQmlEngine = nullptr;
    UIHandle *mUiHandle = nullptr;
    AircraftTableModel *mAircraftTableModel = nullptr;

};

#endif // DATAMANAGER_H
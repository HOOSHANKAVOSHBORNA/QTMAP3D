#ifndef ASSIGNMENTTABLEMODEL_H
#define ASSIGNMENTTABLEMODEL_H

#include "defenseDataManager.h"
#include "aircraftDataManager.h"
#include "systemTableModel.h"

#include <QAbstractTableModel>
#include <deque>
#include <QSharedPointer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPair>

class AircraftDataManager;
class SystemDataManager;
class DefenseModelLayer;
namespace Aircraft {
    struct Data;
}
namespace System {
    struct Data;
}
class SystemModelNode;

class AssignmentTableModel : QObject
{
    Q_OBJECT
public:
    AssignmentTableModel(AircraftDataManager *aircraftDatamanager,
                         SystemDataManager *systemDataManager,
                         DefenseModelLayer *defenseModelLayer, QObject *parent = nullptr);
signals:
    void systemDoubleClicked(const int&);
    void aircraftDoubleClicked(const int&);
public slots:
    void onAircraftDoubleClicked(const int &tn);
    void onSystemDoubleClicked(const int &number);
private:
    AircraftTableModel *mAircraftTableModel;
    SystemTableModel *mSystemTableModel;
    DefenseModelLayer *mDefenseModelLayer;
    AircraftDataManager *mAircraftDataManager;
    SystemDataManager *mSystemDataManager;
};

#endif // ASSIGNMENTTABLEMODEL_H

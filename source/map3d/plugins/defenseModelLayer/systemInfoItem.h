#ifndef SYSTEMINFORMATION_H
#define SYSTEMINFORMATION_H
#include <QAbstractTableModel>
#include <QQuickItem>
#include "aircraftTableModel.h"
#include "systemTableModel.h"

class AircraftModelNode;
class UIHandle;
class QQmlEngine;

namespace Aircraft {
struct Data;
}
namespace System {
struct Data;
}

class SystemInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SystemInfoModel(QObject *parent = nullptr);

    enum MyRoles {
        Numberr = Qt::UserRole + 102,
        Active = Qt::UserRole + 123,
        MainInfo = Qt::UserRole + 200,
        MainInfoHeaders = Qt::UserRole + 201,
        LocationInfo = Qt::UserRole + 202,
        LocationInfoHeaders = Qt::UserRole + 203,
        StatusInfo = Qt::UserRole + 204,
        StatusInfoHeaders = Qt::UserRole + 205,
        CombatInfo = Qt::UserRole + 206,
        CombatInfoHeaders = Qt::UserRole + 207,
        AssignAircraftsName = Qt::UserRole + 208,
        AssignAircraftsType = Qt::UserRole + 209,
        SystemColor = Qt::UserRole + 210,
        BCCStatusColor = Qt::UserRole + 211,
        RadarStatusColor = Qt::UserRole + 212

    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    const System::Data* getStationInfo() {return mSystemInfo;}
    QHash<int, QByteArray> roleNames() const override;

    void setInformtion(const System::Data *info);
signals:
    void gotoButtonClicked();
    void rangeButtonClicked(bool check);
    void wezButtonClicked(bool checked);
    void mezButtonClicked(bool checked);
    void activeButtonToggled(bool checked);
    void moreButtonClicked();
private:
    QStringList getMainInfo() const;
    QStringList getMainInfoHeaders() const;
    QStringList getLocationInfo() const;
    QStringList getLocationInfoHeaders() const;
    QStringList getStatusInfo() const;
    QStringList getStatusInfoHeaders() const;
    QStringList getCombatInfo() const;
    QStringList getCombatInfoHeaders() const;
    QStringList getAssignmentsName() const;
    QStringList getAssignmentsType() const;
private:
    const System::Data* mSystemInfo;
};

class SystemInfoItem : public QObject
{
    Q_OBJECT

public:
    SystemInfoItem(DefenseModelLayer* defenseModelLayer, const System::Data *data, QObject *parent = nullptr);

    SystemInfoModel *getInfo() {return mInfoModel;}
    void setInfo(const System::Data *systemInfo);
    void hide();
    void show();
private:
    SystemInfoModel* mInfoModel;
    QQuickItem *mItem;
    const System::Data* mInformation;
    DefenseModelLayer* mDefenseModelLayer;
};

#endif

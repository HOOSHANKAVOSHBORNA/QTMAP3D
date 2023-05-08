#ifndef INFOMODEL_H
#define INFOMODEL_H
#include <QAbstractListModel>
#include <QQuickItem>
#include "defenseDataManager.h"
namespace Aircraft {
struct Data;
}
namespace System {
struct Data;
}
class SystemModelNode;
class UIHandle;
class QQmlEngine;

class AircraftInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum MyRoles {
        TN = Qt::UserRole + 200,
        MainInfo = Qt::UserRole + 207,
        MainInfoHeaders = Qt::UserRole + 208,
        LocationInfo = Qt::UserRole + 213,
        LocationInfoHeaders = Qt::UserRole + 214,
        DetectionSystems = Qt::UserRole + 215,
        Sends = Qt::UserRole + 216,
        AssignedSystemsName = Qt::UserRole + 217,
        AssignedSystemsNumber = Qt::UserRole + 218,
        AssignedSystemsPhase = Qt::UserRole + 219,
        SystemColor = Qt::UserRole + 220
    };

    AircraftInfoModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setAircraftInfo(const Aircraft::Data *a);
    QStringList getMainInfo() const;
    QStringList getmainInfoHeaders() const;
    QStringList getLocationInfo() const;
    QStringList getLocationInfoHeader() const;
    void addAssignment(int number, SystemModelNode* system);
    void removeAssignment(int systemNumber);

    QStringList getSystemsName() const;
    QStringList getSystemsNumber() const;
    QStringList getSystemsPhase() const;
    QStringList getSystemColor() const;

public slots:
    const Aircraft::Data* getAircraftInfo() {return mAircraftInfo;}
    QColor getAircraftColor();

Q_SIGNALS:
    void gotoButtonClicked();
    void routeButtonClicked(bool checked);
    void trackButtonClicked(bool checked);
    void latestPointsClicked(bool checked);
    void moreButtonClicked();

private:
    const Aircraft::Data *mAircraftInfo;
//    QMap<int, SystemModelNode*> mAssignedSystems;

};

class AircraftInfoItem : public QObject
{
    Q_OBJECT
public:
    explicit AircraftInfoItem(QQmlEngine *mQmlEngine, UIHandle *mUiHandle, const Aircraft::Data *mInformation, QObject *parent = nullptr);
    AircraftInfoModel* getInfo(){return mInfomodel;}
    void updateAircraft(const Aircraft::Data* mInformation);
    void show();

    void addAssignment(int number, SystemModelNode* system);
    void removeAssignment(int systemNumber);
    void setTrackOff();

private:
    const Aircraft::Data *mInformation;
    AircraftInfoModel *mInfomodel;
    UIHandle *mUiHandle = nullptr;
    QQuickItem *mItem{nullptr};
};

#endif // INFOMODEL_H

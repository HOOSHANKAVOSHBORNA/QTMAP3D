#ifndef INFOMODEL_H
#define INFOMODEL_H
#include <QAbstractListModel>
#include <QQuickItem>
#include "dataManager.h"
#include "defenseDataManager.h"

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
        Sends = Qt::UserRole + 216
    };

    AircraftInfoModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    AircraftInfo getAircraftInfo() {return mAircraftInfo;}
    void setAircraftInfo(AircraftInfo &a);
    QStringList getMainInfo() const;
    QStringList getmainInfoHeaders() const;
    QStringList getLocationInfo() const;
    QStringList getLocationInfoHeader() const;

Q_SIGNALS:
    void gotoButtonClicked();
    void routeButtonClicked(bool checked);
    void trackButtonClicked(bool checked);
    void moreButtonClicked();

private:
    AircraftInfo mAircraftInfo;

};

class AircraftInformation : public QObject
{
    Q_OBJECT
public:
    explicit AircraftInformation(QQmlEngine *mQmlEngine, UIHandle *mUiHandle, AircraftInfo mInformation, QObject *parent = nullptr);
    AircraftInfoModel* getInfo(){return mInfomodel;}
    void updateAircraft(AircraftInfo& mInformation);
    void show();
private:
    AircraftInfo mInformation;
    AircraftInfoModel *mInfomodel;
    UIHandle *mUiHandle = nullptr;
    QQuickItem *mItem;
};

#endif // INFOMODEL_H

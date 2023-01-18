#ifndef INFOMODEL_H
#define INFOMODEL_H
#include <QAbstractListModel>
#include <QQuickItem>
#include "dataManager.h"

class AircraftInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum MyRoles {
        TN = Qt::UserRole + 200,
        IFFCode = Qt::UserRole + 201,
        CallSign = Qt::UserRole + 202,
        Type = Qt::UserRole + 203,
        Master = Qt::UserRole + 204,
        Identification = Qt::UserRole + 205,
        IdentificationMethod = Qt::UserRole + 206,
        Time = Qt::UserRole + 207,
        Pos = Qt::UserRole + 208,

        Latitude = Qt::UserRole + 210,
        Longitude = Qt::UserRole + 211,
        Altitude = Qt::UserRole + 212,
        Heading = Qt::UserRole + 213,
        Speed = Qt::UserRole + 214,
        DetectionSystems = Qt::UserRole + 215,
        Sends = Qt::UserRole + 216
    };

    AircraftInfoModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    AircraftInfo getAircraftInfo() {return mAircraftInfo;}
    void setAircraftInfo(AircraftInfo &a);
    QHash<int, QByteArray> roleNames() const override;

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
    void show();
private:
    AircraftInfo mInformation;
    AircraftInfoModel *mInfomodel;
    UIHandle *mUiHandle = nullptr;
    QQuickItem *mItem;
};

#endif // INFOMODEL_H
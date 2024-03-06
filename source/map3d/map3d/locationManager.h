#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QStandardPaths>

#include "osgEarth/Viewpoint"
#include "mapItem.h"

class LocationManager;
class LocationProxyModel;
class LocationModel;
struct LocationItem;

enum {
    NameRole = Qt::UserRole,
    LonRole,
    LatRole,
    ZRole,
    HeadingRole,
    PitchRole,
    RangeRole,
    DescriptionRole,
    ImageSourceRole,
    ColorRole,
};

// ------------------------------------------------------------ model manager
class LocationManager : public QObject
{
    Q_OBJECT

public:
    explicit LocationManager(MapItem *mapItem, QObject *parent = nullptr);
    ~LocationManager();
    void myRemoveRow(int index);
    void addNewLocation(QString newName, QString newDescription, QString newImageSource, QString newColor);
    void editLocation(int index, QString newName, QString newDescription, QString newImageSource, QString newColor);

    Q_INVOKABLE LocationProxyModel *locationProxyModel();

    Q_INVOKABLE void savedModelToFile();
    Q_INVOKABLE void loadModelFromFile();

private:
    LocationProxyModel* mLocationProxyModel;
    LocationModel *mLocationModel;
};

// ------------------------------------------------------------ proxy model
class LocationProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString searchedName READ searchedName WRITE setSearchedName NOTIFY searchedNameChanged FINAL)

public:
    explicit LocationProxyModel();

    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

    Q_INVOKABLE void myRemoveRow(const QModelIndex &index);
    Q_INVOKABLE void goToLocation(const QModelIndex &index);
    Q_INVOKABLE void goToLocation(double lat, double lang);
    Q_INVOKABLE void printCurrentLocation();
    Q_INVOKABLE void addNewLocation(QString newName, QString newDescription, QString newImageSource, QString newColor);
    Q_INVOKABLE QVector3D getCurrentXYZ();
    Q_INVOKABLE void editLocation(const QModelIndex &index, QString newName, QString newDescription, QString newImageSource, QString newColor);

    QString searchedName() const;
    void setSearchedName(const QString &newSearchedName);

signals:
    void searchedNameChanged();

private:
    QString mSearchedWord;
};

// ------------------------------------------------------------ model
class LocationModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LocationModel(MapItem *mapItem, QObject* parent = nullptr);
    ~LocationModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void myRemoveRow(QModelIndex index);
    Q_INVOKABLE void goToLocation(QModelIndex index);
    Q_INVOKABLE void myAppendRow(const LocationItem &newLocationItem);
    Q_INVOKABLE void myAppendRow(QString name, double lon, double lat, double z,
                                 double heading, double pitch, double range,
                                 QString description, QString imageSource, QString color);
    Q_INVOKABLE void myEditRow(QModelIndex index, const LocationItem &newLocationItem);

    QVector<LocationItem *> locations() const;
    void setLocations(const QVector<LocationItem *> &newLocations);

    MapItem *mapItem() const;

    virtual QHash<int, QByteArray> roleNames() const override;

    bool appendLocationsFromJson(const QJsonObject &json);
    QJsonObject toJson();

    Q_INVOKABLE bool readFromFile();
    Q_INVOKABLE bool writeToFile();

public:
    QString appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString savedDir = QString("saved");
    const QString savedFileName = QString("locations.json");

private:
    MapItem *mMapItem;
    QVector<LocationItem *> mLocations;
};

// ------------------------------------------------------------ structs
struct LocationItem
{
public:
    ~LocationItem();
    osgEarth::Viewpoint *viewpoint;
    QString description;
    QString imageSource;
    QString color;

    static LocationItem *fromJson(const QJsonObject &json);
    QJsonObject toJson();
};

#endif // LOCATIONMANAGER_H

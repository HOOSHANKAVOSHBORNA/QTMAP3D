#ifndef LOCATIONMANAGERMODEL_H
#define LOCATIONMANAGERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>

#include "osgEarth/Viewpoint"
#include "mapItem.h"

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

struct LocationData
{
public:
    osgEarth::Viewpoint viewpoint;
    QString description;
    QString imageSource;
    QString color;
};

class LocationManagerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LocationManagerModel(MapItem *mapItem);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Q_INVOKABLE void myRemoveRow(QModelIndex index);
    Q_INVOKABLE void goToLocation(QModelIndex index);
    Q_INVOKABLE void myAppendRow(const LocationData &newLocationData);
    Q_INVOKABLE void myEditRow(QModelIndex index, const LocationData &newLocationData);

    QVector<LocationData> locations() const;
    void setLocations(const QVector<LocationData> &newLocations);

    virtual QHash<int, QByteArray> roleNames() const override;

    MapItem *mapItem() const;

private:
    MapItem *mMapItem;
    QVector<LocationData> mLocations;
};

// ------------------------------------------------------- proxy model
class LocationManagerProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString searchedName READ searchedName WRITE setSearchedName NOTIFY searchedNameChanged FINAL)

public:
    explicit LocationManagerProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void myRemoveRow(const QModelIndex &index);
    Q_INVOKABLE void goToLocation(const QModelIndex &index);
    Q_INVOKABLE void printCurrentLocation(); // for debugging
    Q_INVOKABLE void addNewLocation(QString newName, QString newDescription, QString newImageSource, QString newColor);
    Q_INVOKABLE QVector3D getCurrentXYZ();
    Q_INVOKABLE void editLocation(const QModelIndex &index, QString newName, QString newDescription, QString newImageSource, QString newColor);

    QString searchedName() const;
    void setSearchedName(const QString &newSearchedName);

signals:
    void searchedNameChanged();

private:
    QString mSearchedWord;

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // LOCATIONMANAGERMODEL_H
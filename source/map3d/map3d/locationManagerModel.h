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
    LocationData() {}

    LocationData(QString newDescription, QString newSourceImage, QString newColor) :
        description(newDescription), imageSource(newSourceImage), color(newColor)
    {}

    LocationData(QString newName, QString newDescription, QString newSourceImage, QString newColor) :
        description(newDescription), imageSource(newSourceImage), color(newColor)
    {
        viewpoint.name().init(newName.toStdString());
    }

    LocationData(QString newName, double newLon, double newLat, double newZ, double newHeading, double newPitch, double newRange, QString newDescription, QString newSourceImage, QString newColor) :
        description(newDescription), imageSource(newSourceImage), color(newColor)
    {}

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

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QVector<LocationData> locations() const;
    void setLocations(const QVector<LocationData> &newLocations);

    virtual QHash<int, QByteArray> roleNames() const override;

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

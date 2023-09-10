#ifndef LOCATIONMANAGERMODEL_H
#define LOCATIONMANAGERMODEL_H

#include <QAbstractListModel>
#include "osgEarth/Viewpoint"
#include "mapItem.h"

struct LocationData
{
public:
    LocationData() {}

    LocationData(QString iDescription, QString iImageSource, QString iColor) :
        description(iDescription), imageSource(iImageSource), color(iColor)
    {}

    LocationData(QString iName, QString iDescription, QString iImageSource, QString iColor) :
        description(iDescription), imageSource(iImageSource), color(iColor)
    {
        viewpoint.name().init(iName.toStdString());
    }

    LocationData(QString iName, double lon, double lat, double z, double heading, double pitch, double range, QString iDescription, QString iImageSource, QString iColor) :
        description(iDescription), imageSource(iImageSource), color(iColor)
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

#endif // LOCATIONMANAGERMODEL_H

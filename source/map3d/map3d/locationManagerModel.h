#ifndef LOCATIONMANAGERMODEL_H
#define LOCATIONMANAGERMODEL_H

#include <QAbstractListModel>
#include "osgEarth/Viewpoint"
#include "mapItem.h"

class LocationData
{
public:
    LocationData() {}
    LocationData(QString iname, QString iwhere, QString iimageSource, double ilat, double ilang) :
        name(iname), where(iwhere), lat(ilat), lang(ilang), imageSource(iimageSource)
    {
        vp.setHeading(45);
        vp.setPitch(45);
        vp.setRange(45);
    }

    QString name;
    QString where;
    QString imageSource;
    double lat;
    double lang;
    osgEarth::Viewpoint vp;
};

class LocationManagerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LocationManagerModel(MapItem *mapItem);

    enum {
        NameRole = Qt::UserRole,
        DescriptionRole,
        WhereRole,
        ImageSourceRole,
        LatRole,
        LangRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QVector<LocationData> locations() const;
    void setLocations(const QVector<LocationData> &newLocations);

    virtual QHash<int, QByteArray> roleNames() const override;

private:
    MapItem *mMapItem;
    QVector<LocationData> m_locations;
};

#endif // LOCATIONMANAGERMODEL_H

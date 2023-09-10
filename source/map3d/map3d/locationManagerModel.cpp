#include "locationManagerModel.h"

LocationManagerModel::LocationManagerModel(MapItem *mapItem)
{
    mMapItem = mapItem;
    m_locations.append(LocationData{"Eiffel Tower", "France, Paris", "qrc:/Resources/airplane1.jpg", 45.568075, 74.136643});
    m_locations.append(LocationData{"Old Trafford", "England, Manchester", "qrc:/Resources/airplane2.jpg", 45.568075, 74.136643});
}

int LocationManagerModel::rowCount(const QModelIndex &parent) const
{
    return m_locations.size();
}

QVariant LocationManagerModel::data(const QModelIndex &index, int role) const
{
    const LocationData ld = m_locations.at(index.row());

    QVariant vp;
    vp.setValue(ld.vp);

    switch (role) {
    case NameRole:
        return QVariant(ld.name);
    case DescriptionRole:
        return QVariant("description");
    case WhereRole:
        return QVariant(ld.where);
    case ImageSourceRole:
        return QVariant(ld.imageSource);
    case LatRole:
        return QVariant(ld.lat);
    case LangRole:
        return QVariant(ld.lang);
    default:
        break;
    }

    return QVariant(false);
}

bool LocationManagerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags LocationManagerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool LocationManagerModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool LocationManagerModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

QVector<LocationData> LocationManagerModel::locations() const
{
    return m_locations;
}

void LocationManagerModel::setLocations(const QVector<LocationData> &newLocations)
{
    m_locations = newLocations;
}

QHash<int, QByteArray> LocationManagerModel::roleNames() const
{
    QHash<int, QByteArray> locationFields;

    locationFields[NameRole] = "name";
    locationFields[WhereRole] = "where";
    locationFields[ImageSourceRole] = "imgsrc";
    locationFields[LatRole] = "lat";
    locationFields[LangRole] = "lang";
    locationFields[DescriptionRole] = "description";

    return locationFields;
}



#include "locationManagerModel.h"

LocationManagerModel::LocationManagerModel(MapItem *mapItem)
{
    mMapItem = mapItem;
    mLocations.append(LocationData{"Eiffel Tower", "France, Paris", "qrc:/Resources/airplane1.jpg", "red"});
    mLocations.append(LocationData{"Old Trafford", "England, Manchester", "qrc:/Resources/airplane2.jpg", "yellow"});
    mLocations.append(LocationData{"Eiffel Tower", "France, Paris", "qrc:/Resources/airplane1.jpg", "red"});
    mLocations.append(LocationData{"Old Trafford", "England, Manchester", "qrc:/Resources/airplane2.jpg", "yellow"});
    mLocations.append(LocationData{"Eiffel Tower", "France, Paris", "qrc:/Resources/airplane1.jpg", "red"});
    mLocations.append(LocationData{"Old Trafford", "England, Manchester", "qrc:/Resources/airplane2.jpg", "blue"});
    mLocations.append(LocationData{"Eiffel Tower", "France, Paris", "qrc:/Resources/airplane1.jpg", "red"});
    mLocations.append(LocationData{"Old Trafford", "England, Manchester", "qrc:/Resources/airplane2.jpg", "white"});
    mLocations.append(LocationData{"Eiffel Tower", "France, Paris", "qrc:/Resources/airplane1.jpg", "red"});
    mLocations.append(LocationData{"Old Trafford", "England, Manchester", "qrc:/Resources/airplane2.jpg", "yellow"});
    mLocations.append(LocationData{"Eiffel Tower", "France, Paris", "qrc:/Resources/airplane1.jpg", "red"});
    mLocations.append(LocationData{"Old Trafford", "England, Manchester", "qrc:/Resources/airplane2.jpg", "yellow"});
}

int LocationManagerModel::rowCount(const QModelIndex &parent) const
{
    return mLocations.size();
}

QVariant LocationManagerModel::data(const QModelIndex &index, int role) const
{
    const LocationData ld = mLocations.at(index.row());

    switch (role) {
    case NameRole:
        return QVariant(QString::fromStdString(ld.viewpoint.name().get()));
    case LonRole:
        return QVariant(ld.viewpoint.focalPoint().get().x());
    case LatRole:
        return QVariant(ld.viewpoint.focalPoint().get().y());
    case ZRole:
        return QVariant(ld.viewpoint.focalPoint().get().z());
    case HeadingRole:
        return QVariant(ld.viewpoint.heading()->as(osgEarth::Units::DEGREES));
    case PitchRole:
        return QVariant(ld.viewpoint.pitch()->as(osgEarth::Units::DEGREES));
    case RangeRole:
        return QVariant(ld.viewpoint.range()->as(osgEarth::Units::METERS));
    case DescriptionRole:
        return QVariant(ld.description);
    case ImageSourceRole:
        return QVariant(ld.imageSource);
    case ColorRole:
        return QVariant(ld.color);
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
    return mLocations;
}

void LocationManagerModel::setLocations(const QVector<LocationData> &newLocations)
{
    mLocations = newLocations;
}

QHash<int, QByteArray> LocationManagerModel::roleNames() const
{
    QHash<int, QByteArray> locationFields;

    locationFields[NameRole] = "name";
    locationFields[LonRole] = "lon";
    locationFields[LatRole] = "lat";
    locationFields[ZRole] = "z";
    locationFields[HeadingRole] = "heading";
    locationFields[PitchRole] = "pitch";
    locationFields[RangeRole] = "range";
    locationFields[DescriptionRole] = "description";
    locationFields[ImageSourceRole] = "imageSource";
    locationFields[ColorRole] = "color";
    \
    return locationFields;
}

// ------------------------------------------------------- proxy model methods
LocationManagerProxyModel::LocationManagerProxyModel(QObject *parent)
{}

QString LocationManagerProxyModel::searchedName() const
{
    return mSearchedWord;
}

void LocationManagerProxyModel::setSearchedName(const QString &newSearchedName)
{
    if (mSearchedWord == newSearchedName)
        return;
    mSearchedWord = newSearchedName;
    emit searchedNameChanged();

    invalidateFilter();
}

bool LocationManagerProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    const QString name = sourceModel()->data(index, NameRole).toString().toLower();

    return (name.contains(mSearchedWord.toLower()));
}

bool LocationManagerProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant leftData = sourceModel()->data(source_left, NameRole);
    QVariant rightData = sourceModel()->data(source_right, NameRole);

    return leftData.toString() < rightData.toString();
}

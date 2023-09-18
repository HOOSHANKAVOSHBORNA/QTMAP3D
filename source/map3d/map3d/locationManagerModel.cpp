#include "locationManagerModel.h"
#include "osgEarth/Viewpoint"

LocationManagerModel::LocationManagerModel(MapItem *mapItem)
{
    mMapItem = mapItem;

    osgEarth::GeoPoint gp{mapItem->getMapSRS(), -165, 90, 0};
    osgEarth::Viewpoint vp;
    vp.name() = "North";
    vp.setHeading(0);
    vp.setPitch(-20);
    vp.setRange(5000000);
    vp.focalPoint() = gp;
    LocationData ld1 = LocationData{vp, "France, Paris", "qrc:/Resources/airplane1.jpg", "red"};

    mLocations.append(ld1);
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

void LocationManagerModel::myRemoveRow(QModelIndex index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    mLocations.removeAt(index.row());
    endRemoveRows();
}

void LocationManagerModel::goToLocation(QModelIndex index)
{
    qDebug() << "goToLocation called!";
    mMapItem->getCameraController()->setViewpoint(mLocations.at(index.row()).viewpoint, 1);
}

void LocationManagerModel::myAppendRow(const LocationData &newLocationData)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mLocations.append(newLocationData);
    endInsertRows();
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

MapItem *LocationManagerModel::mapItem() const
{
    return mMapItem;
}

// ------------------------------------------------------- proxy model methods
LocationManagerProxyModel::LocationManagerProxyModel(QObject *parent)
{}

void LocationManagerProxyModel::myRemoveRow(const QModelIndex &index)
{
    dynamic_cast<LocationManagerModel*>(sourceModel())->myRemoveRow(mapToSource(index));
}

void LocationManagerProxyModel::goToLocation(const QModelIndex &index)
{
    dynamic_cast<LocationManagerModel*>(sourceModel())->goToLocation(mapToSource(index));
}

// for debug
void LocationManagerProxyModel::printCurrentLocation()
{
    osgEarth::Viewpoint vp = dynamic_cast<LocationManagerModel*>(sourceModel())->mapItem()->getCameraController()->getViewpoint();

    qDebug() << "vp.name(): " << QString::fromStdString(vp.name().get());
    qDebug() << "vp.focalPoint().value().x(): " << vp.focalPoint().value().x();
    qDebug() << "vp.focalPoint().value().y(): " << vp.focalPoint().value().y();
    qDebug() << "vp.focalPoint().value().z(): " << vp.focalPoint().value().z();
    qDebug() << "vp.heading(): " << vp.heading()->as(osgEarth::Units::DEGREES);
    qDebug() << "vp.pitch(): " << vp.pitch()->as(osgEarth::Units::DEGREES);
    qDebug() << "vp.range(): " << vp.range()->as(osgEarth::Units::METERS);
}

void LocationManagerProxyModel::addNewLocation(QString newName, QString newDescription, QString newImageSource, QString newColor)
{
    osgEarth::Viewpoint vp = dynamic_cast<LocationManagerModel*>(sourceModel())->mapItem()->getCameraController()->getViewpoint();
    vp.name() = newName.toStdString();

    dynamic_cast<LocationManagerModel*>(sourceModel())->myAppendRow(LocationData{vp, newDescription, newImageSource, newColor});
}

QVector3D LocationManagerProxyModel::getCurrentXYZ()
{
    osgEarth::Viewpoint vp = dynamic_cast<LocationManagerModel*>(sourceModel())->mapItem()->getCameraController()->getViewpoint();

    return QVector3D{vp.focalPoint().value().x(), vp.focalPoint().value().y(), vp.focalPoint().value().z()};
}

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

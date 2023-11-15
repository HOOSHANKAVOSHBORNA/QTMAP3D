#include "locationManager.h"
#include "osgEarth/Viewpoint"

LocationModel::LocationModel(MapItem *mapItem)
{
    mMapItem = mapItem;

    osgEarth::GeoPoint gp{mapItem->getMapSRS(), -165, 90, 0};
    osgEarth::Viewpoint vp;
    vp.name() = "North";
    vp.setHeading(0);
    vp.setPitch(-20);
    vp.setRange(5000000);
    vp.focalPoint() = gp;
    LocationItem ld1 = LocationItem{vp, "North of Earth", "qrc:/Resources/airplane1.jpg", "red"};

    mLocations.append(ld1);
}

int LocationModel::rowCount(const QModelIndex &parent) const
{
    return mLocations.size();
}

QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    const LocationItem ld = mLocations.at(index.row());

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

bool LocationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    LocationItem ld = mLocations.at(index.row());

    switch (role) {
    case NameRole:
        ld.viewpoint.name() = value.toString().toStdString();
        break;
//    case LonRole:
//        ld.viewpoint.focalPoint().get().x() = value.toDouble();
//        break;
//    case LatRole:
//        ld.viewpoint.focalPoint().get().y() = value.toDouble();
//        break;
//    case ZRole:
//        ld.viewpoint.focalPoint().get().z() = value.toDouble();
//        break;
//    case HeadingRole:
//        ld.viewpoint.setHeading(value.toDouble());
//        break;
//    case PitchRole:
//        ld.viewpoint.setPitch(value.toDouble());
//        break;
//    case RangeRole:
//        ld.viewpoint.setRange(value.toDouble());
//        break;
//    case DescriptionRole:
//        ld.description = value.toString();
//        break;
//    case ImageSourceRole:
//        ld.imageSource = value.toString();
//        break;
//    case ColorRole:
//        ld.color = value.toString();
//        break;
//    default:
//        break;
    }

    emit dataChanged(index, index, {role});
    return true;
}

void LocationModel::myRemoveRow(QModelIndex index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    mLocations.removeAt(index.row());
    endRemoveRows();
}

void LocationModel::goToLocation(QModelIndex index)
{
    mMapItem->getCameraController()->setViewpoint(mLocations.at(index.row()).viewpoint, 0);
}

void LocationModel::myAppendRow(const LocationItem &newLocationItem)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mLocations.append(newLocationItem);
    endInsertRows();
}

void LocationModel::myEditRow(QModelIndex index, const LocationItem &newLocationItem)
{
    beginResetModel();
    mLocations[index.row()] = newLocationItem;
    endResetModel();
}

QVector<LocationItem> LocationModel::locations() const
{
    return mLocations;
}

void LocationModel::setLocations(const QVector<LocationItem> &newLocations)
{
    mLocations = newLocations;
}

QHash<int, QByteArray> LocationModel::roleNames() const
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

    return locationFields;
}

MapItem *LocationModel::mapItem() const
{
    return mMapItem;
}

// ------------------------------------------------------- proxy model methods
LocationProxyModel *LocationProxyModel::mInstance = nullptr;

LocationProxyModel::LocationProxyModel()
{

}

LocationProxyModel *LocationProxyModel::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(mInstance == nullptr){ mInstance = new LocationProxyModel(); }
    return mInstance;
}

void LocationProxyModel::myRemoveRow(const QModelIndex &index)
{
    dynamic_cast<LocationModel*>(sourceModel())->myRemoveRow(mapToSource(index));
}

void LocationProxyModel::goToLocation(const QModelIndex &index)
{
    dynamic_cast<LocationModel*>(sourceModel())->goToLocation(mapToSource(index));
}

// for debug
void LocationProxyModel::printCurrentLocation()
{
    osgEarth::Viewpoint vp = dynamic_cast<LocationModel*>(sourceModel())->mapItem()->getCameraController()->getViewpoint();

    qDebug() << "vp.name(): " << QString::fromStdString(vp.name().get());
    qDebug() << "vp.focalPoint().value().x(): " << vp.focalPoint().value().x();
    qDebug() << "vp.focalPoint().value().y(): " << vp.focalPoint().value().y();
    qDebug() << "vp.focalPoint().value().z(): " << vp.focalPoint().value().z();
    qDebug() << "vp.heading(): " << vp.heading()->as(osgEarth::Units::DEGREES);
    qDebug() << "vp.pitch(): " << vp.pitch()->as(osgEarth::Units::DEGREES);
    qDebug() << "vp.range(): " << vp.range()->as(osgEarth::Units::METERS);
}

void LocationProxyModel::addNewLocation(QString newName, QString newDescription, QString newImageSource, QString newColor)
{
    osgEarth::Viewpoint vp = dynamic_cast<LocationModel*>(sourceModel())->mapItem()->getCameraController()->getViewpoint();
    vp.name() = newName.toStdString();

    dynamic_cast<LocationModel*>(sourceModel())->myAppendRow(LocationItem{vp, newDescription, newImageSource, newColor});
}

QVector3D LocationProxyModel::getCurrentXYZ()
{
    osgEarth::Viewpoint vp = dynamic_cast<LocationModel*>(sourceModel())->mapItem()->getCameraController()->getViewpoint();

    QVector3D qv3d;
    qv3d.setX(vp.focalPoint().value().x());
    qv3d.setY(vp.focalPoint().value().y());
    qv3d.setZ(vp.focalPoint().value().z());
    return qv3d;
}

void LocationProxyModel::editLocation(const QModelIndex &index, QString newName, QString newDescription, QString newImageSource, QString newColor)
{
    osgEarth::Viewpoint vp = dynamic_cast<LocationModel*>(sourceModel())->mapItem()->getCameraController()->getViewpoint();
    vp.name() = newName.toStdString();

    dynamic_cast<LocationModel*>(sourceModel())->myEditRow(mapToSource(index), LocationItem{vp, newDescription, newImageSource, newColor});
}

QString LocationProxyModel::searchedName() const
{
    return mSearchedWord;
}

void LocationProxyModel::setSearchedName(const QString &newSearchedName)
{
    if (mSearchedWord == newSearchedName)
        return;
    mSearchedWord = newSearchedName;
    emit searchedNameChanged();

    invalidateFilter();
}

bool LocationProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    const QString name = sourceModel()->data(index, NameRole).toString().toLower();

    return (name.contains(mSearchedWord.toLower()));
}

bool LocationProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant leftData = sourceModel()->data(source_left, NameRole);
    QVariant rightData = sourceModel()->data(source_right, NameRole);

    return leftData.toString() < rightData.toString();
}

// ----------------------------------------------------- model manager
LocationManager::LocationManager()
{
    mLocationProxyModel = LocationProxyModel::createSingletonInstance(nullptr, nullptr);
}

LocationManager *LocationManager::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if(mInstance == nullptr){ mInstance = new LocationManager(); }
    return mInstance;
}

void LocationManager::myRemoveRow(int index)
{
    mLocationProxyModel->myRemoveRow(mLocationProxyModel->index(index, 0));
}

void LocationManager::addNewLocation(QString newName, QString newDescription, QString newImageSource, QString newColor)
{
    mLocationProxyModel->addNewLocation(newName, newDescription, newImageSource, newColor);
}

void LocationManager::editLocation(int index, QString newName, QString newDescription, QString newImageSource, QString newColor)
{
    mLocationProxyModel->editLocation(mLocationProxyModel->index(index, 0), newName, newDescription, newImageSource, newColor);
}

LocationProxyModel *LocationManager::locationProxyModel()
{
    return mLocationProxyModel;
}

void LocationManager::initialize(MapItem *mapItem)
{
    LocationModel *myModel = new LocationModel(mapItem);
    mLocationProxyModel->setSourceModel(myModel);
}

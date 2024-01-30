#include <osgEarth/Viewpoint>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QJsonDocument>

#include "locationManager.h"

// ----------------------------------------------------- model manager
LocationManager::LocationManager()
{
    mLocationProxyModel = new LocationProxyModel;
}

LocationManager *LocationManager::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if(mInstance == nullptr){ mInstance = new LocationManager(); }
    return mInstance;
}

void LocationManager::initialize(MapItem *mapItem)
{
    LocationModel *myModel = new LocationModel(mapItem);

    if (myModel->readFromFile()) {
        qDebug() << "locations loaded from file";
    } else {
        qDebug() << "some error in reading location file";
    }

    mLocationProxyModel->setSourceModel(myModel);
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

void LocationManager::savedModelToFile()
{
    dynamic_cast<LocationModel*>(mLocationProxyModel->sourceModel())->writeToFile();
}

void LocationManager::loadModelFromFile()
{
    dynamic_cast<LocationModel*>(mLocationProxyModel->sourceModel())->readFromFile();
}

// ------------------------------------------------------- proxy model methods
LocationProxyModel::LocationProxyModel()
{

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

void LocationProxyModel::myRemoveRow(const QModelIndex &index)
{
    dynamic_cast<LocationModel*>(sourceModel())->myRemoveRow(mapToSource(index));
}

void LocationProxyModel::goToLocation(const QModelIndex &index)
{
    dynamic_cast<LocationModel*>(sourceModel())->goToLocation(mapToSource(index));
}

// DEBUG
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
// ENDDEBUG

void LocationProxyModel::addNewLocation(QString newName, QString newDescription, QString newImageSource, QString newColor)
{
    osgEarth::Viewpoint vp = dynamic_cast<LocationModel*>(sourceModel())->mapItem()->getCameraController()->getViewpoint();
    vp.name() = newName.toStdString();

    osgEarth::Viewpoint *vpPointer = new osgEarth::Viewpoint(vp);

    dynamic_cast<LocationModel*>(sourceModel())->myAppendRow(LocationItem{vpPointer, newDescription, newImageSource, newColor});
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

    osgEarth::Viewpoint *vpPointer = new osgEarth::Viewpoint(vp);

    dynamic_cast<LocationModel*>(sourceModel())->myEditRow(mapToSource(index), LocationItem{vpPointer, newDescription, newImageSource, newColor});
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

// ------------------------------------------------------------ model
LocationModel::LocationModel(MapItem *mapItem)
{
    mMapItem = mapItem;

    // test
    osgEarth::GeoPoint gp{mapItem->getMapSRS(), -165, 90, 0};
    osgEarth::Viewpoint *vp = new osgEarth::Viewpoint;
    vp->name() = "North";
    vp->setHeading(0);
    vp->setPitch(-20);
    vp->setRange(5000000);
    vp->focalPoint() = gp;

    mLocations.append(new LocationItem{vp, "North of Earth", "qrc:/Resources/airplane1.jpg", "red"});
}

int LocationModel::rowCount(const QModelIndex &parent) const
{
    return mLocations.size();
}

QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    const LocationItem *ld = mLocations.at(index.row());

    switch (role) {
    case NameRole:
        return QVariant(QString::fromStdString(ld->viewpoint->name().get()));
    case LonRole:
        return QVariant(ld->viewpoint->focalPoint().get().x());
    case LatRole:
        return QVariant(ld->viewpoint->focalPoint().get().y());
    case ZRole:
        return QVariant(ld->viewpoint->focalPoint().get().z());
    case HeadingRole:
        return QVariant(ld->viewpoint->heading()->as(osgEarth::Units::DEGREES));
    case PitchRole:
        return QVariant(ld->viewpoint->pitch()->as(osgEarth::Units::DEGREES));
    case RangeRole:
        return QVariant(ld->viewpoint->range()->as(osgEarth::Units::METERS));
    case DescriptionRole:
        return QVariant(ld->description);
    case ImageSourceRole:
        return QVariant(ld->imageSource);
    case ColorRole:
        return QVariant(ld->color);
    default:
        break;
    }

    return QVariant(false);
}

void LocationModel::myRemoveRow(QModelIndex index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    mLocations.removeAt(index.row());
    endRemoveRows();
}

void LocationModel::goToLocation(QModelIndex index)
{
    mMapItem->getCameraController()->setViewpoint(*(mLocations.at(index.row())->viewpoint), 0);
}

void LocationModel::myAppendRow(const LocationItem &newLocationItem)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    LocationItem *li = new LocationItem(newLocationItem);
    mLocations.append(li);
    endInsertRows();
}

void LocationModel::myAppendRow(QString name, double lon, double lat, double z, double heading, double pitch, double range, QString description, QString imageSource, QString color)
{
    osgEarth::GeoPoint gp{mMapItem->getMapSRS(), lon, lat, z};
    osgEarth::Viewpoint *vp = new osgEarth::Viewpoint;
    vp->name() = name.toStdString();
    vp->setHeading(heading);
    vp->setPitch(pitch);
    vp->setRange(range);
    vp->focalPoint() = gp;

    mLocations.append(new LocationItem{vp, description, imageSource, color});
}

void LocationModel::myEditRow(QModelIndex index, const LocationItem &newLocationItem)
{
    LocationItem *li = new LocationItem(newLocationItem);
    mLocations[index.row()] = li;
    emit dataChanged(this->index(index.row(), 0), this->index(index.row(), 0));
}

QVector<LocationItem *> LocationModel::locations() const
{
    return mLocations;
}

void LocationModel::setLocations(const QVector<LocationItem *> &newLocations)
{
    mLocations = newLocations;
}

MapItem *LocationModel::mapItem() const
{
    return mMapItem;
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

bool LocationModel::appendLocationsFromJson(const QJsonObject &json)
{
    if (const QJsonValue v = json["locations"]; v.isArray()) {
        QJsonArray locationsArray = v.toArray();
        for (const QJsonValue &location : locationsArray) {
            QJsonDocument doc(location.toObject());
            //            qDebug() << doc.toJson();
            mLocations.append(LocationItem::fromJson(location.toObject()));
        }
    }

    return true;
}

QJsonObject LocationModel::toJson()
{
    QJsonObject json;

    QJsonArray locationsArray;
    for (LocationItem *li : mLocations)
        locationsArray.append(li->toJson());
    json["locations"] = locationsArray;

    return json;
}

bool LocationModel::readFromFile()
{
    QDir dir;
    if (!dir.exists(appDir + "/" + savedDir)) {
        dir.mkpath(appDir + "/" + savedDir);
    }

    //    qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QFile locationsFile(appDir + "/" + savedDir + "/" + savedFileName);

    if (!locationsFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray savedData = locationsFile.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(savedData);

    appendLocationsFromJson(jsonDoc.object());

    return true;
}

bool LocationModel::writeToFile()
{
    QDir dir;
    if (!dir.exists(appDir + "/" + savedDir)) {
        dir.mkpath(savedDir);
    }

    QFile locationsFile(appDir + "/" + savedDir + "/" + savedFileName);

    if (!locationsFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonObject jsonObject = this->toJson();
    locationsFile.write(QJsonDocument(jsonObject).toJson());

    return true;
}

// ---------------------------------------------------------------------- structs
LocationItem *LocationItem::fromJson(const QJsonObject &json)
{
    LocationItem *result = new LocationItem;

    if (const QJsonValue v = json["viewpoint"]; v.isObject()) {

        QJsonObject viewpointJson = v.toObject();

        osgEarth::Viewpoint *viewpoint = new osgEarth::Viewpoint;

        if (const QJsonValue v = viewpointJson["name"]; v.isString()) {
            viewpoint->name() = v.toString().toStdString();
        }

        double lon = 0, lat = 0, z = 0;

        if (const QJsonValue v = viewpointJson["lon"]; v.isDouble()) {
            lon = v.toDouble();
        }

        if (const QJsonValue v = viewpointJson["lat"]; v.isDouble()) {
            lat = v.toDouble();
        }

        if (const QJsonValue v = viewpointJson["z"]; v.isDouble()) {
            z = v.toDouble();
        }

        osgEarth::GeoPoint gp{osgEarth::SpatialReference::get("wgs84"), lon, lat, z};
        viewpoint->focalPoint() = gp;

        if (const QJsonValue v = viewpointJson["heading"]; v.isDouble()) {
            viewpoint->setHeading(v.toDouble());
        }

        if (const QJsonValue v = viewpointJson["pitch"]; v.isDouble()) {
            viewpoint->setPitch(v.toDouble());
        }

        if (const QJsonValue v = viewpointJson["range"]; v.isDouble()) {
            viewpoint->setRange(v.toDouble());
        }

        result->viewpoint = viewpoint;
    }

    if (const QJsonValue v = json["description"]; v.isString()) {
        result->description = v.toString();
    }

    if (const QJsonValue v = json["imageSource"]; v.isString()) {
        result->imageSource = v.toString();
    }

    if (const QJsonValue v = json["color"]; v.isString()) {
        result->color = v.toString();
    }

    return result;
}

QJsonObject LocationItem::toJson()
{
    QJsonObject json;

    QJsonObject viewpointObject;

    viewpointObject["name"] = QString::fromStdString(this->viewpoint->name().get());

    viewpointObject["lon"] = this->viewpoint->focalPoint().get().x();
    viewpointObject["lat"] = this->viewpoint->focalPoint().get().y();
    viewpointObject["z"] = this->viewpoint->focalPoint().get().z();

    viewpointObject["heading"] = this->viewpoint->heading()->as(osgEarth::Units::DEGREES);
    viewpointObject["pitch"] = this->viewpoint->pitch()->as(osgEarth::Units::DEGREES);
    viewpointObject["range"] = this->viewpoint->range()->as(osgEarth::Units::METERS);

    json["viewpoint"] = viewpointObject;

    json["description"] = this->description;
    json["imageSource"] = this->imageSource;
    json["color"] = this->color;

    return json;
}

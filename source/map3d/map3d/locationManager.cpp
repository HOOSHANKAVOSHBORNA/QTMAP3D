#include <osgEarth/Viewpoint>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QJsonDocument>

#include "locationManager.h"

// ----------------------------------------------------- model manager
LocationManager::LocationManager(MapItem *mapItem, QObject *parent)
    : QObject(parent)
{
    mLocationProxyModel = new LocationProxyModel(mapItem, this);
    mLocationModel = new LocationModel(mapItem, mLocationProxyModel);

    // ------------------------------------------------- loading models from file
    if (mLocationModel->readFromFile()) {
        qInfo() << "info - success: "
                << "locations loaded from file";
    } else {
        qInfo() << "info - fail: "
                << "some error in reading location file";
    }

    mLocationProxyModel->setSourceModel(mLocationModel);
}

LocationManager::~LocationManager()
{
    qDebug()<<"~LocationManager";
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
LocationProxyModel::LocationProxyModel(MapItem *mapItem, QObject *parent)
    : QSortFilterProxyModel(parent)
{
    mMapItem = mapItem;
}

LocationProxyModel::~LocationProxyModel()
{
    qDebug()<<"~LocationProxyModel";
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

void LocationProxyModel::goToLocation(double lat, double lon)
{
    osgEarth::Viewpoint vp = dynamic_cast<LocationModel *>(sourceModel())
                                 ->mapItem()
                                 ->getCameraController()
                                 ->getViewpoint();
    dynamic_cast<LocationModel *>(sourceModel())
        ->mapItem()
        ->getCameraController()
        ->setViewpoint(osgEarth::Viewpoint("somewhere",
                                           lon,
                                           lat,
                                           vp.focalPoint().value().z(),
                                           vp.heading()->getValue(),
                                           vp.pitch()->getValue(),
                                           vp.getRange()),
                       1);
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
    dynamic_cast<LocationModel *>(sourceModel())
        ->myAppendRow(newName, newDescription, newImageSource, newColor);
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
    dynamic_cast<LocationModel *>(sourceModel())
        ->myEditRow(mapToSource(index), newName, newDescription, newImageSource, newColor);
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

QVector3D LocationProxyModel::viewPoint() const
{
    return mViewPoint;
}

void LocationProxyModel::setViewPoint(const QVector3D &newViewPoint)
{
    if (mViewPoint == newViewPoint)
        return;

    mViewPoint = newViewPoint;
    emit viewPointChanged();
}

void LocationProxyModel::updateCurrentViewPoint()
{
    osgEarth::Viewpoint vp = mMapItem->getCameraController()->getViewpoint();
    setViewPoint(
        {(float) vp.focalPoint()->x(), (float) vp.focalPoint()->y(), (float) vp.focalPoint()->z()});
}

void LocationProxyModel::addPlaceWindowOpened()
{
    qDebug() << "ay-debug ------ "
             << "opened";
    connect(mMapItem->getCameraController(),
            &CameraController::viewPointChanged,
            this,
            &LocationProxyModel::updateCurrentViewPoint);
}

void LocationProxyModel::addPlaceWindowClosed()
{
    qDebug() << "ay-debug ------ "
             << "closed";
    disconnect(mMapItem->getCameraController(), nullptr, this, nullptr);
}

// ------------------------------------------------------------ model
LocationModel::LocationModel(MapItem *mapItem, QObject *parent)
    : QAbstractListModel(parent)
{
    mMapItem = mapItem;

    // TEST
    //    osgEarth::GeoPoint gp{mapItem->getMapSRS(), -165, 90, 0};
    //    osgEarth::Viewpoint *vp = new osgEarth::Viewpoint;
    //    vp->name() = "North";
    //    vp->setHeading(0);
    //    vp->setPitch(-20);
    //    vp->setRange(5000000);
    //    vp->focalPoint() = gp;
    //    mLocations.append(new LocationItem{vp, "North of Earth", "qrc:/Resources/airplane1.jpg", "red"});
    // ENDTEST
}

LocationModel::~LocationModel()
{
    writeToFile();
    qDebug()<<"~LocationModel";
}

int LocationModel::rowCount(const QModelIndex &parent) const
{
    return mLocations.size();
}

QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    const LocationItem& ld = mLocations.at(index.row());

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

void LocationModel::myRemoveRow(QModelIndex index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    mLocations.removeAt(index.row());
    endRemoveRows();
}

void LocationModel::goToLocation(QModelIndex index)
{
    mMapItem->getCameraController()->setViewpoint((mLocations.at(index.row()).viewpoint), 1);
}

// DEBUG
void LocationModel::printViewpoint(osgEarth::Viewpoint *vp)
{
    qDebug() << "vp->name(): " << QString::fromStdString(vp->name().get());
    qDebug() << "vp->focalPoint().value().x(): " << vp->focalPoint().value().x();
    qDebug() << "vp->focalPoint().value().y(): " << vp->focalPoint().value().y();
    qDebug() << "vp->focalPoint().value().z(): " << vp->focalPoint().value().z();
    qDebug() << "vp->heading(): " << vp->heading()->as(osgEarth::Units::DEGREES);
    qDebug() << "vp->pitch(): " << vp->pitch()->as(osgEarth::Units::DEGREES);
    qDebug() << "vp->range(): " << vp->range()->as(osgEarth::Units::METERS);
}
// ENDDEBUG

void LocationModel::myAppendRow(QString newName,
                                QString newDescription,
                                QString newImageSource,
                                QString newColor)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    osgEarth::Viewpoint currentVp = mapItem()->getCameraController()->getViewpoint();
    osgEarth::GeoPoint gp{mMapItem->getMapSRS(),
                          currentVp.focalPoint()->x(),
                          currentVp.focalPoint()->y(),
                          currentVp.focalPoint()->z()};

    osgEarth::Viewpoint vp;
    vp.name() = newName.toStdString();
    vp.setHeading(currentVp.getHeading());
    vp.setPitch(currentVp.getPitch());
    vp.setRange(currentVp.getRange());
    vp.focalPoint() = gp;
    mLocations.append(LocationItem{vp, newDescription, newImageSource, newColor});

    endInsertRows();
}

void LocationModel::myEditRow(QModelIndex index,
                              QString newName,
                              QString newDescription,
                              QString newImageSource,
                              QString newColor)
{
    osgEarth::Viewpoint vp = mapItem()->getCameraController()->getViewpoint();
    LocationItem& li = mLocations[index.row()];
    li.viewpoint.name() = newName.toStdString();
    li.viewpoint.focalPoint()->x() = vp.focalPoint()->x();
    li.viewpoint.focalPoint()->y() = vp.focalPoint()->y();
    li.viewpoint.focalPoint()->z() = vp.focalPoint()->z();
    li.viewpoint.setHeading(vp.getHeading());
    li.viewpoint.setPitch(vp.getPitch());
    li.viewpoint.setRange(vp.getRange());
    li.description = newDescription;
    li.imageSource = newImageSource;
    li.color = newColor;

    emit dataChanged(this->index(index.row(), 0), this->index(index.row(), 0));
}

//QVector<LocationItem *> LocationModel::locations() const
//{
//    return mLocations;
//}

//void LocationModel::setLocations(const QVector<LocationItem *> &newLocations)
//{
//    mLocations = newLocations;
//}

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
//            QJsonDocument doc(location.toObject());
            LocationItem locationItem;
            locationItem.fromJson(location.toObject());
            mLocations.append(locationItem);
        }
    }

    return true;
}

QJsonObject LocationModel::toJson()
{
    QJsonObject json;

    QJsonArray locationsArray;
    for (LocationItem& li : mLocations)
        locationsArray.append(li.toJson());
    json["locations"] = locationsArray;

    return json;
}

bool LocationModel::readFromFile()
{
    QDir dir;
    if (!dir.exists(appDir + "/" + savedDir)) {
        dir.mkpath(appDir + "/" + savedDir);
    }

    savedFileName = UserManager::instance()->userName();
    if (UserManager::instance()->userName() == "") {
        savedFileName = "NoUser";
    }

    QFile locationsFile(appDir + "/" + savedDir + "/" + savedFileName + ".json");

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

    savedFileName = UserManager::instance()->userName();
    if (UserManager::instance()->userName() == "") {
        savedFileName = "NoUser";
    }

    QFile locationsFile(appDir + "/" + savedDir + "/" + savedFileName + ".json");

    if (!locationsFile.open(QIODevice::WriteOnly)) {
        qDebug() << "ay-debug ------ "
                 << "I am going to write to:" << locationsFile.fileName();
        return false;
    }

    QJsonObject jsonObject = this->toJson();
    locationsFile.write(QJsonDocument(jsonObject).toJson());

    return true;
}

// ---------------------------------------------------------------------- structs
LocationItem::~LocationItem()
{
    qDebug()<<"~LocationItem";
}

void LocationItem::fromJson(const QJsonObject &json)
{

    if (const QJsonValue v = json["viewpoint"]; v.isObject()) {

        QJsonObject viewpointJson = v.toObject();

        if (const QJsonValue v = viewpointJson["name"]; v.isString()) {
            viewpoint.name() = v.toString().toStdString();
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
        viewpoint.focalPoint() = gp;

        if (const QJsonValue v = viewpointJson["heading"]; v.isDouble()) {
            viewpoint.setHeading(v.toDouble());
        }

        if (const QJsonValue v = viewpointJson["pitch"]; v.isDouble()) {
            viewpoint.setPitch(v.toDouble());
        }

        if (const QJsonValue v = viewpointJson["range"]; v.isDouble()) {
            viewpoint.setRange(v.toDouble());
        }
    }

    if (const QJsonValue v = json["description"]; v.isString()) {
        description = v.toString();
    }

    if (const QJsonValue v = json["imageSource"]; v.isString()) {
        imageSource = v.toString();
    }

    if (const QJsonValue v = json["color"]; v.isString()) {
        color = v.toString();
    }
}

QJsonObject LocationItem::toJson()
{
    QJsonObject json;

    QJsonObject viewpointObject;

    viewpointObject["name"] = QString::fromStdString(this->viewpoint.name().get());

    viewpointObject["lon"] = this->viewpoint.focalPoint().get().x();
    viewpointObject["lat"] = this->viewpoint.focalPoint().get().y();
    viewpointObject["z"] = this->viewpoint.focalPoint().get().z();

    viewpointObject["heading"] = this->viewpoint.heading()->as(osgEarth::Units::DEGREES);
    viewpointObject["pitch"] = this->viewpoint.pitch()->as(osgEarth::Units::DEGREES);
    viewpointObject["range"] = this->viewpoint.range()->as(osgEarth::Units::METERS);

    json["viewpoint"] = viewpointObject;

    json["description"] = this->description;
    json["imageSource"] = this->imageSource;
    json["color"] = this->color;

    return json;
}

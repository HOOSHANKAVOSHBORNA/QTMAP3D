#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QStandardPaths>

#include "mapItem.h"
#include "osgEarth/Viewpoint"
#include "userManager.h"

class LocationManager;
class LocationProxyModel;
class LocationModel;
struct LocationItem;

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

// ------------------------------------------------------------ model manager
class LocationManager : public QObject
{
    Q_OBJECT

public:
    explicit LocationManager(MapItem *mapItem, QObject *parent = nullptr);
    ~LocationManager();
    void myRemoveRow(int index);
    void addNewLocation(QString newName, QString newDescription, QString newImageSource, QString newColor);
    void editLocation(int index, QString newName, QString newDescription, QString newImageSource, QString newColor);

    Q_INVOKABLE LocationProxyModel *locationProxyModel();

    Q_INVOKABLE void savedModelToFile();
    Q_INVOKABLE void loadModelFromFile();

private:
    LocationProxyModel* mLocationProxyModel;
    LocationModel *mLocationModel;
};

// ------------------------------------------------------------ proxy model
class LocationProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString searchedName READ searchedName WRITE setSearchedName NOTIFY searchedNameChanged FINAL)
    Q_PROPERTY(QVector3D viewPoint READ viewPoint WRITE setViewPoint NOTIFY viewPointChanged FINAL)
    Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged FINAL)

public:
    explicit LocationProxyModel(MapItem *mapItem, QObject *parent = nullptr);
    ~LocationProxyModel();

    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

    Q_INVOKABLE void myRemoveRow(const QModelIndex &index);
    Q_INVOKABLE void goToLocation(const QModelIndex &index);
    Q_INVOKABLE void goToLocation(double lat, double lang, double alt);
    Q_INVOKABLE void printCurrentLocation();
    Q_INVOKABLE void addNewLocation(QString newName, QString newDescription, QString newImageSource, QString newColor);
    Q_INVOKABLE QVector3D getCurrentXYZ();
    Q_INVOKABLE void editLocation(const QModelIndex &index, QString newName, QString newDescription, QString newImageSource, QString newColor);

    QString searchedName() const;
    void setSearchedName(const QString &newSearchedName);

    QVector3D viewPoint() const;
    void setViewPoint(const QVector3D &newViewPoint);
    void updateCurrentViewPoint();

    Q_INVOKABLE void addPlaceWindowClosed();
    Q_INVOKABLE void addPlaceWindowOpened();

    Q_INVOKABLE void capture();

    QString imagePath() const;
    void setImagePath(const QString &newImagePath);

signals:
    void searchedNameChanged();
    void viewPointChanged();

    void imagePathChanged();

private slots:
    void onImageProcessComplete();

private:
    MapItem *mMapItem;
    QString mSearchedWord;
    QVector3D mViewPoint;
    CaptureCallback *mCaptureCallback = nullptr;

    QString appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString savedDir = QString("locatoins");
    QString savedFileName = QString("NoUser");
    QString mImagePath;
};

// ------------------------------------------------------------ model
class LocationModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LocationModel(MapItem *mapItem, QObject *parent = nullptr);
    ~LocationModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void myRemoveRow(QModelIndex index);
    Q_INVOKABLE void goToLocation(QModelIndex index);
    Q_INVOKABLE void myAppendRow(QString newName,
                                 QString newDescription,
                                 QString newImageSource,
                                 QString newColor);
    Q_INVOKABLE void myEditRow(QModelIndex index,
                               QString newName,
                               QString newDescription,
                               QString newImageSource,
                               QString newColor);

    //    QVector<LocationItem *> locations() const;
    //    void setLocations(const QVector<LocationItem *> &newLocations);

    QString getFilePath();

    MapItem *mapItem() const;

    virtual QHash<int, QByteArray> roleNames() const override;

    bool appendLocationsFromJson(const QJsonObject &json);
    QJsonObject toJson();

    Q_INVOKABLE bool readFromFile();
    Q_INVOKABLE bool writeToFile();

    void printViewpoint(osgEarth::Viewpoint *vp);

public:
    QString appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString savedDir = QString("locatoins");
    QString savedFileName = QString("NoUser");

private:
    MapItem *mMapItem;
    QVector<LocationItem> mLocations;
};

// ------------------------------------------------------------ structs
struct LocationItem
{
public:
    ~LocationItem();
    osgEarth::Viewpoint viewpoint;
    QString description;
    QString imageSource;
    QString color;

    void fromJson(const QJsonObject &json);
    QJsonObject toJson();
};

#endif // LOCATIONMANAGER_H

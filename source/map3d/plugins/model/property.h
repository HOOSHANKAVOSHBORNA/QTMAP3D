#ifndef PROPERTY_H
#define PROPERTY_H

#include <QColor>
#include <QObject>
#include <QVector3D>
#include <osg/Referenced>

#include "simpleModelNode.h"

class PropertyItem;

// ---------------------------------------------------------------------- manager
class Property : public QObject
{
public:
    Property(osg::ref_ptr<SimpleModelNode> mCurrentModel, MapControllerItem *mapItem);
    void setPropertyItem(PropertyItem *newPropertyItem);

    void createQML();

    PropertyItem *propertyItem() const;

    QQuickItem *qmlItem() const;

    QVector3D getLocation() const;
    void setLocation(const QVector3D &newLocation);
    void setLocation(const osgEarth::GeoPoint &newLocation);

    QVector3D getMoveTo() const;
    void setMoveTo(const QVector3D &newMoveTo);
    void setMoveTo(const osgEarth::GeoPoint &newMoveTo);

    bool isMovable() const;
    void setIsMovable(bool newIsMovable);

    osg::ref_ptr<SimpleModelNode> currentModel() const;
    void setCurrentModel(const osg::ref_ptr<SimpleModelNode> &newCurrentModel);

private:
    MapControllerItem *mMapItem;
    PropertyItem *mPropertyItem;
    QQuickItem *mQmlItem;
};

// ---------------------------------------------------------------------- interface for qml
class PropertyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY propertyChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY propertyChanged FINAL)
    Q_PROPERTY(QVector3D location READ getLocation WRITE setLocation NOTIFY propertyChanged FINAL)
    Q_PROPERTY(bool isMovable READ isMovable WRITE setIsMovable NOTIFY propertyChanged FINAL)
    Q_PROPERTY(QVector3D moveTo READ getMoveTo WRITE setMoveTo NOTIFY propertyChanged FINAL)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY propertyChanged FINAL)

public:
    PropertyItem(osg::ref_ptr<SimpleModelNode> mCurrentModel, MapControllerItem *mapItem);

    QString name();
    void setName(const QString &newName);

    QColor color() const;
    void setColor(const QColor &newColor);

    QVector3D getLocation() const;
    void setLocation(const QVector3D &newLocation);

    QVector3D getMoveTo() const;
    void setMoveTo(const QVector3D &newmoveTo);

    double speed() const;
    void setSpeed(double newSpeed);

    osg::ref_ptr<SimpleModelNode> currentModel() const;
    void setCurrentModel(const osg::ref_ptr<SimpleModelNode> &newCurrentModel);

    void nodeToProperty(const osgEarth::GeoPoint &geoPos, QString state);

    bool isMovable() const;
    void setIsMovable(bool newIsMovable);

signals:
    void propertyChanged();

private:
    MapControllerItem *mMapItem{nullptr};
    osg::ref_ptr<SimpleModelNode> mCurrentModel{nullptr};
    QString mName{"defaultName"};
    bool mIsMovable;
    QColor mColor;
    QVector3D mLocation;
    QVector3D mMoveTo;
    double mSpeed;
};

#endif // PROPERTY_H

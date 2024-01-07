#ifndef PROPERTY_H
#define PROPERTY_H

#include <QColor>
#include <QObject>
#include <QVector3D>
#include <osg/Referenced>

#include "mapControllerItem.h"
#include "simpleModelNode.h"

// class prototypes
class Property;
class PropertyItem;

// ---------------------------------------------------------------------- manager
class Property : public QObject
{
public:
    Property(MapControllerItem *mapItem);

    osg::ref_ptr<SimpleModelNode> modelNode() const;
    void setModelNode(const osg::ref_ptr<SimpleModelNode> &newModelNode);

    void setName(const std::string &name);
    void setPosition(const osgEarth::GeoPoint &positon);
    void moveTo(const osgEarth::GeoPoint &positon);
    void flyTo(const osgEarth::GeoPoint &positon);

    QQuickItem *qmlItem() const;

private:
    void createQML();
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
    Q_PROPERTY(bool isFlyable READ isFlyable WRITE setIsFlyable NOTIFY propertyChanged FINAL)
    Q_PROPERTY(QVector3D moveTo READ getMoveTo WRITE setMoveTo NOTIFY propertyChanged FINAL)
    Q_PROPERTY(QVector3D flyTo READ getFlyTo WRITE setFlyTo NOTIFY propertyChanged FINAL)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY propertyChanged FINAL)

public:
    PropertyItem(MapControllerItem *mapItem);

    osg::ref_ptr<SimpleModelNode> modelNode() const;
    void setModelNode(const osg::ref_ptr<SimpleModelNode> &newModelNode);

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

    QVector3D getFlyTo() const;
    void setFlyTo(const QVector3D &newFlyTo);

    bool isMovable() const;
    void setIsMovable(bool newIsMovable);

    bool isFlyable() const;
    void setIsFlyable(bool newIsFlyable);

signals:
    void propertyChanged();

private:
    MapControllerItem *mMapItem{nullptr};
    osg::ref_ptr<SimpleModelNode> mModelNode{nullptr};
    QString mName{"defaultName"};
    QColor mColor{"white"};
    QVector3D mLocation;
    QVector3D mMoveTo;
    double mSpeed{10};
    QVector3D mFlyTo;
    bool mIsMovable;
    bool mIsFlyable;
};

#endif // PROPERTY_H

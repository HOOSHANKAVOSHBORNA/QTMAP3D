#ifndef PROPERTY_H
#define PROPERTY_H

#include <QColor>
#include <QObject>
#include <QVector3D>
#include <osg/Referenced>

#include "mapControllerItem.h"

// class prototypes
class Property;
class PropertyItem;

// ---------------------------------------------------------------------- manager
class Property : public QObject
{
    Q_OBJECT
public:
    explicit Property(QQmlEngine *engine, MapControllerItem *mapItem, QObject *parent = nullptr);

    NodeData nodeData() const;
    void setNodeData(const NodeData &nodeData);

    QQuickItem *qmlItem() const;

    bool hasModel() const;
    void sethasModel(bool newHasModel);

signals:
    void nodeDataChanged(const NodeData &nodeData);

private:
    void createQML();

private:
    QQmlEngine *mQmlEngine = nullptr;
    MapControllerItem *mMapItem = nullptr;
    PropertyItem *mPropertyItem = nullptr;
    QQuickItem *mQmlItem = nullptr;
    bool mHasModel{false};
};

// ---------------------------------------------------------------------- interface for qml
class PropertyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY propertyChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY propertyChanged FINAL)
    Q_PROPERTY(QVector3D location READ getLocation WRITE setLocation NOTIFY propertyChanged FINAL)
    Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY propertyChanged FINAL)

public:
    PropertyItem(MapControllerItem *mapItem, QObject *parent = nullptr);

    NodeData nodeData() const;
    void setNodeData(const NodeData &nodeData);

    QString name();
    void setName(const QString &newName);

    QColor color() const;
    void setColor(const QColor &newColor);

    QVector3D getLocation() const;
    void setLocation(const QVector3D &newLocation);

    double speed() const;
    void setSpeed(double newSpeed);

signals:
    void propertyChanged();
    void nodeDataChanged(const NodeData &nodeData);

private:
    MapControllerItem *mMapItem{nullptr};
    NodeData mNodeData;
    QString mName{"defaultName"};
    QColor mColor{QColorConstants::Red};
    QVector3D mLocation;
    double mSpeed{10};
    bool mIsNodeDataSet{false};
};

#endif // PROPERTY_H

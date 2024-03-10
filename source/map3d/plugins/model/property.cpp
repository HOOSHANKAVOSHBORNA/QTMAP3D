#include <QQmlContext>

#include "property.h"

// ---------------------------------------------------------------------- manager
Property::Property(QQmlEngine *engine, MapControllerItem *mapItem, QObject *parent)
    :QObject(parent)
{
    mQmlEngine = engine;
    mMapItem = mapItem;
    mPropertyItem = new PropertyItem(mapItem, this);
    connect(mPropertyItem, &PropertyItem::nodeDataChanged, this,[this](const NodeData &nodeData){
        if(mHasModel){
            emit nodeDataChanged(nodeData);
        }
    });
//    qmlEngine(mMapItem)->rootContext()->setContextProperty("modelPropertyInterface", mPropertyItem);
    createQML();
}

NodeData Property::nodeData() const
{
    return mPropertyItem->nodeData();
}

void Property::setNodeData(const NodeData &nodeData)
{
    mPropertyItem->setNodeData(nodeData);
}

void Property::createQML()
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    connect(comp, &QQmlComponent::statusChanged, this, [=] {
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }

        mQmlItem = qobject_cast<QQuickItem *>(comp->create());
        mQmlItem->setProperty("cppInterface", QVariant::fromValue(mPropertyItem));
    });

    comp->loadUrl(QUrl("qrc:/PropertyItem.qml"));
}

bool Property::hasModel() const
{
    return mHasModel;
}

void Property::sethasModel(bool newHasModel)
{
    mHasModel = newHasModel;
}

QQuickItem *Property::qmlItem() const
{
    return mQmlItem;
}

// ---------------------------------------------------------------------- interface for qml
PropertyItem::PropertyItem(MapControllerItem *mapItem, QObject *parent)
    :QObject(parent),
    mMapItem(mapItem)
{
    connect(this, &PropertyItem::propertyChanged, this, [this](){
        if(!mIsNodeDataSet)
            emit nodeDataChanged(mNodeData);
    });

    mNodeData.name = mName;
    mNodeData.color = mColor.name(QColor::HexArgb);
    mNodeData.longitude = mLocation.x();
    mNodeData.latitude = mLocation.y();
    mNodeData.altitude = mLocation.z();
    mNodeData.speed = mSpeed;
}

NodeData PropertyItem::nodeData() const
{
    return mNodeData;
}

void PropertyItem::setNodeData(const NodeData &nodeData)
{
    mIsNodeDataSet = true;
    mNodeData = nodeData;
    mNodeData.color = mColor.name(QColor::HexArgb);
    mNodeData.speed = mSpeed;
    setName(mNodeData.name);
    setLocation(QVector3D(mNodeData.longitude, mNodeData.latitude, mNodeData.altitude));
    mIsNodeDataSet = false;
    emit propertyChanged();
}

QString PropertyItem::name()
{
    return mName;
}

void PropertyItem::setName(const QString &newName)
{
    if (mName == newName)
        return;

    mName = newName;
    mNodeData.name = mName;
    emit propertyChanged();
}

QColor PropertyItem::color() const
{
    return mColor;
}

void PropertyItem::setColor(const QColor &newColor)
{
    if (mColor == newColor)
        return;
    mColor = newColor;
    mNodeData.color = newColor.name(QColor::HexArgb);
    emit propertyChanged();
}

QVector3D PropertyItem::getLocation() const
{
    return mLocation;
}

void PropertyItem::setLocation(const QVector3D &newLocation)
{
    if (qFuzzyCompare(mLocation.x(), newLocation.x())
        && qFuzzyCompare(mLocation.y(), newLocation.y())
        && qFuzzyCompare(mLocation.z(), newLocation.z()))
        return;

    mLocation = newLocation;
    mNodeData.longitude = mLocation.x();
    mNodeData.latitude = mLocation.y();
    mNodeData.altitude = mLocation.z();
    emit propertyChanged();
}

double PropertyItem::speed() const
{
    return mSpeed;
}

void PropertyItem::setSpeed(double newSpeed)
{
    if (qFuzzyCompare(mSpeed, newSpeed))
        return;

    mSpeed = newSpeed;
    mNodeData.speed = mSpeed;
    emit propertyChanged();
}

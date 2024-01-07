#include <QQmlContext>

#include "flyableModelNode.h"
#include "moveableModelNode.h"
#include "property.h"
#include "utility.h"

// ---------------------------------------------------------------------- manager
Property::Property(MapControllerItem *mapItem)
{
    mMapItem = mapItem;
    mPropertyItem = new PropertyItem(mapItem);
//    qmlEngine(mMapItem)->rootContext()->setContextProperty("modelPropertyInterface", mPropertyItem);
    createQML();
}

osg::ref_ptr<SimpleModelNode> Property::modelNode() const
{
    return mPropertyItem->modelNode();
}

void Property::setModelNode(const osg::ref_ptr<SimpleModelNode> &newModelNode)
{
    mPropertyItem->setModelNode(newModelNode);
    if(newModelNode){
        setName(newModelNode->getName());
        setPosition(newModelNode->getPosition());
//        moveTo(newModelNode->getPosition());
//        flyTo(newModelNode->getPosition());
    }
}

void Property::setName(const std::string &name)
{
    mPropertyItem->setName(QString::fromStdString(name));
}

void Property::setPosition(const osgEarth::GeoPoint &positon)
{
    mPropertyItem->setLocation(Utility::osgEarthGeoPointToQvector3D(positon));
}

void Property::moveTo(const osgEarth::GeoPoint &positon)
{
    mPropertyItem->setMoveTo(Utility::osgEarthGeoPointToQvector3D(positon));
}

void Property::flyTo(const osgEarth::GeoPoint &positon)
{
    mPropertyItem->setFlyTo(Utility::osgEarthGeoPointToQvector3D(positon));
}

void Property::createQML()
{
    QQmlComponent *comp = new QQmlComponent(qmlEngine(mMapItem));
    connect(comp, &QQmlComponent::statusChanged, [&] {
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }

        mQmlItem = qobject_cast<QQuickItem *>(comp->create());
        mQmlItem->setProperty("cppInterface", QVariant::fromValue(mPropertyItem));
    });

    comp->loadUrl(QUrl("qrc:/PropertyItem.qml"));
}

QQuickItem *Property::qmlItem() const
{
    return mQmlItem;
}

// ---------------------------------------------------------------------- interface for qml
PropertyItem::PropertyItem(MapControllerItem *mapItem)
    :mMapItem(mapItem)
{
}

osg::ref_ptr<SimpleModelNode> PropertyItem::modelNode() const
{
    return mModelNode;
}

void PropertyItem::setModelNode(const osg::ref_ptr<SimpleModelNode> &newModelNode)
{
    mModelNode = newModelNode;
    setIsMovable(false);
    setIsFlyable(false);

    if(mModelNode){
        if(mModelNode->asMoveableModelNode()){
            setIsMovable(true);
            mModelNode->asMoveableModelNode()->setSpeed(mSpeed);
            mModelNode->nodeData()->speed = mSpeed;
        }
        if(mModelNode->asFlyableModelNode())
            setIsFlyable(true);

        mModelNode->setColor(Utility::qColor2osgEarthColor(mColor));
    }
}

QString PropertyItem::name()
{
    return mName;
}

void PropertyItem::setName(const QString &newName)
{
    if (mName == newName)
        return;

    if (mModelNode){
        mModelNode->setName(mName.toStdString());
        mModelNode->nodeData()->name = mName.toStdString();
    }

    mName = newName;
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

    if (mModelNode){
        mModelNode->nodeData()->color = newColor.name(QColor::HexArgb).toStdString();
        mModelNode->setColor(Utility::qColor2osgEarthColor(newColor));
    }

    mColor = newColor;
    emit propertyChanged();
}

QVector3D PropertyItem::getLocation() const
{
    return mLocation;
}

void PropertyItem::setLocation(const QVector3D &newLocation)
{
    if (mLocation == newLocation)
        return;

    if (mModelNode){
        mModelNode->nodeData()->latitude = mLocation.x();
        mModelNode->nodeData()->longitude = mLocation.y();
        mModelNode->nodeData()->altitude = mLocation.z();
        mModelNode->setPosition(Utility::qVector3DToosgEarthGeoPoint(mLocation, mMapItem->getMapSRS()));
    }

    mLocation = newLocation;
    emit propertyChanged();
}

QVector3D PropertyItem::getMoveTo() const
{
    return mMoveTo;
}

void PropertyItem::setMoveTo(const QVector3D &newMoveTo)
{
    if (mMoveTo == newMoveTo)
        return;

    if(mModelNode && mModelNode->asMoveableModelNode()){
        mModelNode->asMoveableModelNode()->moveTo(Utility::qVector3DToosgEarthGeoPoint(newMoveTo, mMapItem->getMapSRS()), mSpeed);
    }
    mMoveTo = newMoveTo;
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

    if (mModelNode && mModelNode->asMoveableModelNode()){
        mModelNode->asMoveableModelNode()->setSpeed(mSpeed);
    }

    mSpeed = newSpeed;
    emit propertyChanged();
}

QVector3D PropertyItem::getFlyTo() const
{
    return mFlyTo;
}

void PropertyItem::setFlyTo(const QVector3D &newFlyTo)
{
    if (mFlyTo == newFlyTo)
        return;
    if(mModelNode && mModelNode->asFlyableModelNode()){
        mModelNode->asFlyableModelNode()
            ->flyTo(Utility::qVector3DToosgEarthGeoPoint(newFlyTo, mMapItem->getMapSRS()), mSpeed);
    }
    mFlyTo = newFlyTo;

    emit propertyChanged();
}

bool PropertyItem::isMovable() const
{
    return mIsMovable;
}

void PropertyItem::setIsMovable(bool newIsMovable)
{
    if (mIsMovable == newIsMovable)
        return;
    mIsMovable = newIsMovable;
    emit propertyChanged();
}

bool PropertyItem::isFlyable() const
{
    return mIsFlyable;
}

void PropertyItem::setIsFlyable(bool newIsFlyable)
{
    if (mIsFlyable == newIsFlyable)
        return;
    mIsFlyable = newIsFlyable;
    emit propertyChanged();
}

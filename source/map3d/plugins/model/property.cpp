#include "property.h"
#include "flyableModelNode.h"
#include "moveableModelNode.h"
#include "utility.h"
#include <QQmlContext>

// ---------------------------------------------------------------------- manager
Property::Property(osg::ref_ptr<SimpleModelNode> mCurrentModel, MapControllerItem *mapItem)
{
    mMapItem = mapItem;
    mPropertyItem = new PropertyItem(mCurrentModel, mapItem);
    qmlEngine(mMapItem)->rootContext()->setContextProperty("modelPropertyInterface", mPropertyItem);
    createQML();
}

void Property::setPropertyItem(PropertyItem *newPropertyItem)
{
    mPropertyItem = newPropertyItem;
}

void Property::createQML()
{
    QQmlComponent *comp = new QQmlComponent(qmlEngine(mMapItem));
    connect(comp, &QQmlComponent::statusChanged, [&] {
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }

        mQmlItem = qobject_cast<QQuickItem *>(comp->create());
    });

    comp->loadUrl(QUrl("qrc:/PropertyItem.qml"));
}

PropertyItem *Property::propertyItem() const
{
    return mPropertyItem;
}

QQuickItem *Property::qmlItem() const
{
    return mQmlItem;
}

QVector3D Property::getLocation() const
{
    return mPropertyItem->getLocation();
}

void Property::setLocation(const QVector3D &newLocation)
{
    mPropertyItem->setLocation(newLocation);
}

void Property::setLocation(const osgEarth::GeoPoint &newLocation)
{
    mPropertyItem->setLocation(Utility::osgEarthGeoPointToQvector3D(newLocation));
}

QVector3D Property::getMoveTo() const
{
    return mPropertyItem->getMoveTo();
}

void Property::setMoveTo(const QVector3D &newmoveTo)
{
    return mPropertyItem->setMoveTo(newmoveTo);
}

void Property::setMoveTo(const osgEarth::GeoPoint &newMoveTo)
{
    mPropertyItem->setMoveTo(Utility::osgEarthGeoPointToQvector3D(newMoveTo));
}

QVector3D Property::getFlyTo() const
{
    return mPropertyItem->getFlyTo();
}

void Property::setFlyTo(const QVector3D &newFlyTo)
{
    mPropertyItem->setFlyTo(newFlyTo);
}

void Property::setFlyTo(const osgEarth::GeoPoint &newFlyTo)
{
    mPropertyItem->setFlyTo(Utility::osgEarthGeoPointToQvector3D(newFlyTo));
}

bool Property::isMovable() const
{
    return mPropertyItem->isMovable();
}

void Property::setIsMovable(bool newIsMovable)
{
    mPropertyItem->setIsMovable(newIsMovable);
}

bool Property::isFlyable() const
{
    return mPropertyItem->isFlyable();
}

void Property::setIsFlyable(bool newIsFlyable)
{
    mPropertyItem->setIsFlyable(newIsFlyable);
}

osg::ref_ptr<SimpleModelNode> Property::currentModel() const
{
    return mPropertyItem->currentModel();
}

void Property::setCurrentModel(const osg::ref_ptr<SimpleModelNode> &newCurrentModel)
{
    mPropertyItem->setCurrentModel(newCurrentModel);
}

// ---------------------------------------------------------------------- interface for qml
PropertyItem::PropertyItem(osg::ref_ptr<SimpleModelNode> newNode, MapControllerItem *mapItem)
{
    mMapItem = mapItem;
    mCurrentModel = newNode;
}

QString PropertyItem::name()
{
    return mName;
}

void PropertyItem::setName(const QString &newName)
{
    if (mName == newName)
        return;

    if (!mCurrentModel)
        return;

    mName = newName;
    mCurrentModel->nodeData()->name = mName.toStdString();
    emit propertyChanged();
}

bool PropertyItem::isMovable() const
{
    if (!mCurrentModel)
        return true;

    return mCurrentModel->asMoveableModelNode();
}

QColor PropertyItem::color() const
{
    if (!mCurrentModel)
        return QColor("#FFFFFF");

    return mColor;
}

void PropertyItem::setColor(const QColor &newColor)
{
    if (mColor == newColor)
        return;

    if (!mCurrentModel)
        return;

    mColor = newColor;
    mCurrentModel->nodeData()->color = newColor.name(QColor::HexArgb).toStdString();
    mCurrentModel->setColor(
        osgEarth::Color(newColor.redF(), newColor.greenF(), newColor.blueF(), newColor.alphaF()));
    emit propertyChanged();
}

QVector3D PropertyItem::getLocation() const
{
    if (!mCurrentModel)
        return QVector3D{0, 0, 0};

    return mLocation;
}

void PropertyItem::setLocation(const QVector3D &newLocation)
{
    if (mLocation == newLocation)
        return;

    if (!mCurrentModel)
        return;

    mLocation = newLocation;
    mCurrentModel->setPosition(
        Utility::qVector3DToosgEarthGeoPoint(mLocation, mMapItem->getMapSRS()));
    emit propertyChanged();
}

QVector3D PropertyItem::getMoveTo() const
{
    if (!mCurrentModel)
        return QVector3D{0, 0, 0};

    return mMoveTo;
}

void PropertyItem::setMoveTo(const QVector3D &newMoveTo)
{
    if (mMoveTo == newMoveTo)
        return;

    mMoveTo = newMoveTo;
//    mCurrentModel->asMoveableModelNode()
//        ->moveTo(Utility::qVector3DToosgEarthGeoPoint(newMoveTo, mMapItem->getMapSRS()), mSpeed);
    emit propertyChanged();
}

double PropertyItem::speed() const
{
    if (!mCurrentModel)
        return 0;

    return mSpeed;
}

void PropertyItem::setSpeed(double newSpeed)
{
    if (qFuzzyCompare(mSpeed, newSpeed))
        return;

    if (!mCurrentModel)
        return;

    mSpeed = newSpeed;
    if (mCurrentModel->asMoveableModelNode()) {
        mCurrentModel->asMoveableModelNode()->setSpeed(mSpeed);
    }

    emit propertyChanged();
}

osg::ref_ptr<SimpleModelNode> PropertyItem::currentModel() const
{
    return mCurrentModel;
}

void PropertyItem::setCurrentModel(const osg::ref_ptr<SimpleModelNode> &newCurrentModel)
{
    mCurrentModel = newCurrentModel;
    if (mCurrentModel->asMoveableModelNode()) {
        mIsMovable = true;
        this->setSpeed(mCurrentModel->asMoveableModelNode()->speed());

        if (mCurrentModel->asFlyableModelNode()) {
            mIsFlyable = true;
        } else {
            mIsFlyable = false;
        }
    } else {
        mIsMovable = false;
    }

    this->setLocation(QVector3D{(float) mCurrentModel->nodeData()->latitude,
                                (float) mCurrentModel->nodeData()->longitude,
                                (float) mCurrentModel->nodeData()->altitude});
    this->setName(QString::fromStdString(mCurrentModel->nodeData()->name));
    this->setColor(QColor(QString::fromStdString(mCurrentModel->nodeData()->color)));

    emit propertyChanged();
}

void PropertyItem::setIsMovable(bool newIsMovable)
{
    if (mIsMovable == newIsMovable)
        return;
    mIsMovable = newIsMovable;
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
    mFlyTo = newFlyTo;
//    mCurrentModel->asFlyableModelNode()
//        ->flyTo(Utility::qVector3DToosgEarthGeoPoint(newFlyTo, mMapItem->getMapSRS()), mSpeed);
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

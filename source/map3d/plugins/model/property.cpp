#include "property.h"
#include "flyableModelNode.h"
#include "moveableModelNode.h"
#include "utility.h"

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

bool Property::isMovable() const
{
    return mPropertyItem->isMovable();
}

void Property::setIsMovable(bool newIsMovable)
{
    mPropertyItem->setIsMovable(newIsMovable);
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
    emit propretyChanged();
}

bool PropertyItem::isMovable() const
{
    if (!mCurrentModel)
        return true;

    return mCurrentModel->asMoveableModelNode() || mCurrentModel->asFlyableModelNode();
}

QColor PropertyItem::color() const
{
    if (!mCurrentModel)
        return "#FFFFFF";

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
    emit propretyChanged();
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
    emit propretyChanged();
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
    emit propretyChanged();
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
    if (mCurrentModel->asFlyableModelNode()) {
        mCurrentModel->asFlyableModelNode()->setSpeed(mSpeed);
    } else {
        mCurrentModel->asMoveableModelNode()->setSpeed(mSpeed);
    }

    emit propretyChanged();
}

osg::ref_ptr<SimpleModelNode> PropertyItem::currentModel() const
{
    return mCurrentModel;
}

void PropertyItem::setCurrentModel(const osg::ref_ptr<SimpleModelNode> &newCurrentModel)
{
    mCurrentModel = newCurrentModel;
    if (mCurrentModel->asMoveableModelNode() || mCurrentModel->asFlyableModelNode()) {
        mIsMovable = true;
    } else {
        mIsMovable = false;
    }
}

void PropertyItem::setIsMovable(bool newIsMovable)
{
    if (mIsMovable == newIsMovable)
        return;
    mIsMovable = newIsMovable;
    emit propretyChanged();
}

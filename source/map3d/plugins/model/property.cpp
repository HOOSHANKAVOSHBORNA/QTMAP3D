#include "property.h"
#include "flyableModelNode.h"
#include "moveableModelNode.h"
#include "utility.h"

// ---------------------------------------------------------------------- manager
Property::Property(osg::ref_ptr<SimpleModelNode> mCurrentModel, MapControllerItem *mapItem)
{
    mMapItem = mapItem;
    mPropertyItem = new PropertyItem(mCurrentModel, mapItem);
    createQML();
}

void Property::setPropertyItem(PropertyItem *newPropertyItem)
{
    mPropertyItem = newPropertyItem;
}

void Property::createQML()
{
    QQmlComponent *comp = new QQmlComponent(qmlEngine(mMapItem));
    connect(comp, &QQmlComponent::statusChanged, [comp, this]() {
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

PropertyItem *Property::setLocation(const QVector3D &newLocation)
{
    return mPropertyItem->setLocation(newLocation);
}

QVector3D Property::getMoveTo() const
{
    return mPropertyItem->getMoveTo();
}

PropertyItem *Property::setMoveTo(const QVector3D &newmoveTo)
{
    return mPropertyItem->setMoveTo(newmoveTo);
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
    return mPropertyItem->setCurrentModel(newCurrentModel);
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

PropertyItem *PropertyItem::setName(const QString &newName)
{
    if (mName == newName)
        return this;
    mName = newName;
    mCurrentModel->nodeData()->name = mName.toStdString();
    emit propretyChanged();

    return this;
}

bool PropertyItem::isMovable() const
{
    if (!mCurrentModel)
        return true;
    return mCurrentModel->asMoveableModelNode() || mCurrentModel->asFlyableModelNode();
}

QColor PropertyItem::color() const
{
    return mColor;
}

PropertyItem *PropertyItem::setColor(const QColor &newColor)
{
    if (mColor == newColor)
        return this;
    mColor = newColor;
    mCurrentModel->nodeData()->color = newColor.name(QColor::HexArgb).toStdString();
    mCurrentModel->setColor(
        osgEarth::Color(newColor.redF(), newColor.greenF(), newColor.blueF(), newColor.alphaF()));
    emit propretyChanged();

    return this;
}

QVector3D PropertyItem::getLocation() const
{
    return mLocation;
}

PropertyItem *PropertyItem::setLocation(const QVector3D &newLocation)
{
    if (mLocation == newLocation)
        return this;
    mLocation = newLocation;
    mCurrentModel->setPosition(
        Utility::qVector3DToosgEarthGeoPoint(mLocation, mMapItem->getMapSRS()));
    emit propretyChanged();

    return this;
}

QVector3D PropertyItem::getMoveTo() const
{
    return mMoveTo;
}

PropertyItem *PropertyItem::setMoveTo(const QVector3D &newMoveTo)
{
    if (mMoveTo == newMoveTo)
        return this;
    mMoveTo = newMoveTo;
    emit propretyChanged();

    return this;
}

double PropertyItem::speed() const
{
    return mSpeed;
}

PropertyItem *PropertyItem::setSpeed(double newSpeed)
{
    if (qFuzzyCompare(mSpeed, newSpeed))
        return this;
    mSpeed = newSpeed;
    if (mCurrentModel->asFlyableModelNode()) {
        mCurrentModel->asFlyableModelNode()->setSpeed(mSpeed);
    } else {
        mCurrentModel->asMoveableModelNode()->setSpeed(mSpeed);
    }

    emit propretyChanged();

    return this;
}

osg::ref_ptr<SimpleModelNode> PropertyItem::currentModel() const
{
    return mCurrentModel;
}

void PropertyItem::setCurrentModel(const osg::ref_ptr<SimpleModelNode> &newCurrentModel)
{
    mCurrentModel = newCurrentModel;
}

void PropertyItem::nodeToProperty(const osgEarth::GeoPoint &geoPos, QString nodeState)
{
    //    qDebug() << "I am in update Property";
    if (mCurrentModel->asFlyableModelNode()) {
        this->setSpeed(mCurrentModel->asFlyableModelNode()->speed());
        if (nodeState == "ready") {
            this->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
        } else {
            this->setMoveTo(Utility::osgEarthGeoPointToQvector3D(geoPos));
        }
    } else if (mCurrentModel->asMoveableModelNode()) {
        qDebug() << "in movable";
        this->setSpeed(mCurrentModel->asMoveableModelNode()->speed());
        if (nodeState == "ready") {
            this->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
            qDebug() << "in movable ready";
        } else {
            this->setMoveTo(Utility::osgEarthGeoPointToQvector3D(geoPos));
            qDebug() << "in movable moving";
        }
    } else {
        this->setLocation(Utility::osgEarthGeoPointToQvector3D(geoPos));
    }

    this->setName(QString::fromStdString(mCurrentModel->nodeData()->name));
    this->setColor(QColor(QString::fromStdString(mCurrentModel->nodeData()->color)));
}

void PropertyItem::setIsMovable(bool newIsMovable)
{
    if (mIsMovable == newIsMovable)
        return;
    mIsMovable = newIsMovable;
    emit propretyChanged();
}

#include "property.h"
#include "flyableModelNode.h"
#include "moveableModelNode.h"
#include "utility.h"

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

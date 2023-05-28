#include "coneProperties.h"

#include <QQmlComponent>
#include <QQuickItem>

ConePropertiesModel::ConePropertiesModel(Cone *cone, MapItem *mapItem, QObject *parent):
    QObject(parent),
    mCone(cone),
    mMapItem(mapItem)
{
}

QString ConePropertiesModel::getFillcolor() const
{
    return mFillcolor;
}

void ConePropertiesModel::setFillColor(const QString &fillcolor)
{
    if (mFillcolor == fillcolor)
        return;
    mFillcolor = fillcolor;
    if (mCone) {
        osgEarth::Color tmpColor = mCone->getColor();
        float A = tmpColor.a();
        tmpColor = mFillcolor.toStdString();
        tmpColor.a() = A;
        mCone->setColor(tmpColor);
    }
}

QVector3D ConePropertiesModel::getLocation() const
{
    return mLocation;
}

void ConePropertiesModel::setLocation(const QVector3D &location)
{
    if (mLocation == location)
        return;
    mLocation = location;
    if (mCone){
        osgEarth::GeoPoint tmpLocation = mCone->getPosition();
        tmpLocation.x() = static_cast<double>(location.x());
        tmpLocation.y() = static_cast<double>(location.y());
        tmpLocation.z() = static_cast<double>(location.z()+mCone->getHeight().getValue()/2);
        mCone->setPosition(tmpLocation);
        emit positionToQmlChanged();
    }
}

double ConePropertiesModel::getRadius() const
{
    return mRadius;
}

void ConePropertiesModel::setRadius(const double &radius)
{
    if (std::abs(mRadius - radius) < 0.001)
        return;
    mRadius = radius;
    if (mCone)
        mCone->setRadius(mRadius);
}

double ConePropertiesModel::getHeight() const
{
    return mHeight;
}

void ConePropertiesModel::setHeight(const double &coneHeight)
{
    if (std::abs(mHeight - coneHeight) < 0.001)
        return;
    mHeight = coneHeight;
    if (mCone)
        mCone->setHeight(mHeight);
}

int ConePropertiesModel::getTransparency() const
{
    return mTransparency;
}

void ConePropertiesModel::setTransparency(const int &transparency)
{
    if (mTransparency == transparency)
        return;
    mTransparency = transparency;
    if (mCone) {
        osgEarth::Color tmpColor = mCone->getColor();
        tmpColor.a() = static_cast<float>(transparency) / 100;
        mCone->setColor(tmpColor);
    }
}

void ConePropertiesModel::setCone(Cone *cone)
{
    mCone = cone;
    if (!mCone)
        return;
    osgEarth::Color tmpColor = mCone->getColor();
    float opacity = mTransparency;
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = opacity / 100;
    mCone->setColor(tmpColor);
    mCone->setHeight(mHeight);
    mCone->setRadius(mRadius);
}

void ConePropertiesModel::setRelative(const bool &value){
        if(value == mRelative)
            return;
    mRelative = value;
    if(mCone){
        osgEarth::GeoPoint tempLocation =  mCone->getPosition();
        if(value == true )
        {
            tempLocation.makeRelative(mMapItem->getMapNode()->getTerrain());
            mCone->setPosition(tempLocation);
        }
        else if(value == false )
        {
            tempLocation.makeAbsolute(mMapItem->getMapNode()->getTerrain());
            mCone->setPosition(tempLocation);
        }
    }
}


ConeProperties::ConeProperties(Cone* cone, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapItem *mapItem, QObject *parent) :
    QObject(parent),
    mQmlEngine(qmlEngine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapItem, cone](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mConeProperties = new ConePropertiesModel(cone, mapItem);
            mItem->setProperty("coneProperties", QVariant::fromValue<ConePropertiesModel*>(mConeProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/ConeProperty.qml"));
}

void ConeProperties::show()
{
    mUiHandle->propertiesShow(mItem);
}

void ConeProperties::hide()
{
    mUiHandle->propertiesHide(mItem);
}

void ConeProperties::setCone(Cone *cone)
{
    mConeProperties->setCone(cone);
}

void ConeProperties::setLocation(osgEarth::GeoPoint location)
{
    QVector3D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    tmp.setZ(static_cast<float>(location.z()));

    mConeProperties->setLocation(tmp);
}


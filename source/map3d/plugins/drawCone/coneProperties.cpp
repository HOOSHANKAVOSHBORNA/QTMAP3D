#include "coneProperties.h"

#include <QQmlComponent>
#include <QQuickItem>

ConePropertiesModel::ConePropertiesModel(Cone *cone, MapController *mapcontroller, QObject *parent):
    QObject(parent),
    mCone(cone),
    mMapController(mapcontroller)
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
        tmpLocation.z() = static_cast<double>(location.z());
        mCone->setPosition(tmpLocation);
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

int ConePropertiesModel::getClamp() const
{
    return mClamp;
}

void ConePropertiesModel::setClamp(int clamp)
{

}

void ConePropertiesModel::setCone(Cone *cone)
{
    mCone = cone;
    if (!mCone)
        return;
    osgEarth::Color tmpColor = mCone->getColor();
    float opacity = tmpColor.a();
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = opacity;
    mCone->setColor(tmpColor);
    setTransparency(mTransparency);
    mCone->setHeight(mHeight);
//    mCone->setClamp(mClamp);
    mCone->setRadius(mRadius);
}


ConeProperties::ConeProperties(Cone* cone, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapController *mapcontroller, QObject *parent) :
    QObject(parent),
    mQmlEngine(qmlEngine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapcontroller, cone](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mConeProperties = new ConePropertiesModel(cone, mapcontroller);
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

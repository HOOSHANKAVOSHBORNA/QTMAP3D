#include "cylinderProperties.h"

#include <QQmlComponent>
#include <QQuickItem>

CylinderPropertiesModel::CylinderPropertiesModel(Cylinder *Cylinder, MapItem *mapItem, QObject *parent):
    QObject(parent),
    mCylinder(Cylinder),
    mMapItem(mapItem)
{
}

QString CylinderPropertiesModel::getFillcolor() const
{
    return mFillcolor;
}

void CylinderPropertiesModel::setFillColor(const QString &fillcolor)
{
    if (mFillcolor == fillcolor)
        return;
    mFillcolor = fillcolor;
    if (mCylinder) {
        osgEarth::Color tmpColor = mCylinder->getColor();
        float A = tmpColor.a();
        tmpColor = mFillcolor.toStdString();
        tmpColor.a() = A;
        mCylinder->setColor(tmpColor);
    }
}

QVector3D CylinderPropertiesModel::getLocation() const
{
    return mLocation;
}

void CylinderPropertiesModel::setLocation(const QVector3D &location)
{
    if (mLocation == location)
        return;
    mLocation = location;
    if (mCylinder){
        osgEarth::GeoPoint tmpLocation = mCylinder->getPosition();
        tmpLocation.x() = static_cast<double>(location.x());
        tmpLocation.y() = static_cast<double>(location.y());
        tmpLocation.z() = static_cast<double>(location.z());
        mCylinder->setPosition(tmpLocation);
        emit positionToQmlChanged();
    }
}

double CylinderPropertiesModel::getRadius() const
{
    return mRadius;
}

void CylinderPropertiesModel::setRadius(const double &radius)
{
    if (std::abs(mRadius - radius) < 0.001)
        return;
    mRadius = radius;
    if (mCylinder)
        mCylinder->setRadius(mRadius);
}

double CylinderPropertiesModel::getHeight() const
{
    return mHeight;
}

void CylinderPropertiesModel::setHeight(const double &CylinderHeight)
{
    if (std::abs(mHeight - CylinderHeight) < 0.001)
        return;
    mHeight = CylinderHeight;
    if (mCylinder)
        mCylinder->setHeight(mHeight);
}

int CylinderPropertiesModel::getTransparency() const
{
    return mTransparency;
}

void CylinderPropertiesModel::setTransparency(const int &transparency)
{
    if (mTransparency == transparency)
        return;
    mTransparency = transparency;
    if (mCylinder) {
        osgEarth::Color tmpColor = mCylinder->getColor();
        tmpColor.a() = static_cast<float>(transparency) / 100;
        mCylinder->setColor(tmpColor);
    }
}

void CylinderPropertiesModel::setCylinder(Cylinder *Cylinder)
{
    mCylinder = Cylinder;
    if (!mCylinder)
        return;
    osgEarth::Color tmpColor = mCylinder->getColor();
    float opacity = mTransparency;
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = opacity / 100;
    mCylinder->setColor(tmpColor);
    mCylinder->setHeight(mHeight);
    mCylinder->setRadius(mRadius);
}

void CylinderPropertiesModel::setRelative(const bool &value){
        if(value == mRelative)
            return;
    mRelative = value;
    if(mCylinder){
        osgEarth::GeoPoint tempLocation =  mCylinder->getPosition();
        if(value == true )
        {
            tempLocation.makeRelative(mMapItem->getMapNode()->getTerrain());
            mCylinder->setPosition(tempLocation);
        }
        else if(value == false )
        {
            tempLocation.makeAbsolute(mMapItem->getMapNode()->getTerrain());
            mCylinder->setPosition(tempLocation);
        }
    }
}


//CylinderProperties::CylinderProperties(Cylinder* Cylinder, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapItem *mapItem, QObject *parent) :
//    QObject(parent),
//    mQmlEngine(qmlEngine),
//    mUiHandle(uiHandle)
//{
//    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
//    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapItem, Cylinder](){
//        if (comp->status() == QQmlComponent::Ready) {
//            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
//            mCylinderProperties = new CylinderPropertiesModel(Cylinder, mapItem);
//            mItem->setProperty("cylinderProperties", QVariant::fromValue<CylinderPropertiesModel*>(mCylinderProperties));
//        }
//    });
//    comp->loadUrl(QUrl("qrc:/CylinderProperty.qml"));
//}

CylinderProperties::CylinderProperties(QQuickItem *parent):
    Property(parent)
{

}

void CylinderProperties::setFillColor(const QColor &color)
{
    setFillColorStatus(true);
    Property::setFillColor(color);
//    mCylinder->setColor(color);
}

void CylinderProperties::setRadius(const double &radius)
{
    setRadiusStatus(true);
    Property::setRadius(radius);
}
void CylinderProperties::setLocationRelative(const bool &relative)
{
    setLocationRelative(true);
    Property::setLocationRelative(relative);
}



//void CylinderProperties::setStrokeWidth(const double &opacity)
//{
//    setStrokeStatus(true);
//    Property::setStrokeWidth(opacity);
//}


void CylinderProperties::setLocation(const QVector3D &status)
{
    setLocationStatus(true);
    Property::setLocation(status);


}

void CylinderProperties::setHeight(const double &height)
{
    setHeightStatus(true);
    Property::setHeight(height);
}


Cylinder *CylinderProperties::getCylinder() const
{
    return mCylinder;
}

//void CylinderProperties::setCylinder(Cylinder *newCylinder)
//{
//    mCylinder = newCylinder;
//}



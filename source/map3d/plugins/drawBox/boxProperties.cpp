#include "boxProperties.h"

#include <QQmlComponent>
#include <QQuickItem>

BoxPropertiesModel::BoxPropertiesModel(Box *box, MapController *mapcontroller, QObject *parent):
    QObject(parent),
    mBox(box),
    mMapController(mapcontroller)
{
}


QString BoxPropertiesModel::getColor() const
{
    return mColor;
}

void BoxPropertiesModel::setColor(const QString &fillcolor)
{
    if (mColor == fillcolor)
        return;
    mColor = fillcolor;
    if (mBox) {
        osgEarth::Color tmpColor = mBox->getColor();
        float A = tmpColor.a();
        tmpColor = mColor.toStdString();
        tmpColor.a() = A;
        mBox->setColor(tmpColor);
    }
}

QVector3D BoxPropertiesModel::getLocation() const
{
    return mLocation;
}

void BoxPropertiesModel::setLocation(const QVector3D &location)
{
    if (mLocation == location)
        return;
    mLocation = location;
    if (mBox){
        osgEarth::GeoPoint tmpLocation = mBox->getPosition();
        tmpLocation.x() = static_cast<double>(location.x());
        tmpLocation.y() = static_cast<double>(location.y());
        tmpLocation.z() = static_cast<double>(location.z());
        mBox->setPosition(tmpLocation);
        emit positionToQmlChanged();
    }
}



double BoxPropertiesModel::getHeight() const
{
    return mHeight;
}

void BoxPropertiesModel::setHeight(const double &boxHeight)
{
    if (std::abs(mHeight - boxHeight) < 0.001)
        return;
    mHeight = boxHeight;
    if (mBox)
        mBox->setHeight(mHeight);
}

double BoxPropertiesModel::getWidth() const
{
    return mWidth;
}

void BoxPropertiesModel::setWidth(const double &boxWidth)
{
    if (std::abs(mWidth - boxWidth) < 0.001)
        return;
    mWidth = boxWidth;
    if (mBox)
        mBox->setWidth(mWidth);
}

double BoxPropertiesModel::getLength() const
{
    return mLength;
}

void BoxPropertiesModel::setLength(const double &boxLength)
{
    if (std::abs(mLength - boxLength) < 0.001)
        return;
    mLength = boxLength;
    if (mBox)
        mBox->setLength(mLength);
}

int BoxPropertiesModel::getOpacity() const
{
    return mOpacity;
}

void BoxPropertiesModel::setOpacity(const int &transparency)
{
    if (mOpacity == transparency)
        return;
    mOpacity = transparency;
    if (mBox) {
        osgEarth::Color tmpColor = mBox->getColor();
        tmpColor.a() = static_cast<float>(transparency) / 100;
        mBox->setColor(tmpColor);
    }
}



void BoxPropertiesModel::setRelative(const bool &value){
        if(value == mRelative)
            return;
    mRelative = value;
    if(mBox){
        osgEarth::GeoPoint tempLocation =  mBox->getPosition();
        if(value == true )
        {
            tempLocation.makeRelative(mMapController->getMapNode()->getTerrain());
            mBox->setPosition(tempLocation);
        }
        else if(value == false )
        {
            tempLocation.makeAbsolute(mMapController->getMapNode()->getTerrain());
            mBox->setPosition(tempLocation);
        }
    }
}


void BoxPropertiesModel::setBox(Box *box)
{
    mBox = box;
    if (!mBox)
        return;
    osgEarth::Color tmpColor = mBox->getColor();
    float opacity = mOpacity;
    tmpColor  = mColor.toStdString();
    tmpColor.a() = opacity / 100;
    mBox->setColor(tmpColor);
    mBox->setHeight(mHeight);
    mBox->setWidth(mWidth);
    mBox->setLength(mLength);
}




BoxProperties::BoxProperties(Box* box, QQmlEngine *qmlEngine, UIHandle *uiHandle, MapController *mapcontroller, QObject *parent) :
    QObject(parent),
    mQmlEngine(qmlEngine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapcontroller, box](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mBoxProperties = new BoxPropertiesModel(box, mapcontroller);
            mItem->setProperty("boxProperties", QVariant::fromValue<BoxPropertiesModel*>(mBoxProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/BoxProperty.qml"));
}

void BoxProperties::show()
{
    mUiHandle->propertiesShow(mItem);
}

void BoxProperties::hide()
{
    mUiHandle->propertiesHide(mItem);
}

void BoxProperties::setBox(Box *box)
{
    mBoxProperties->setBox(box);
}

void BoxProperties::setLocation(osgEarth::GeoPoint location)
{
    QVector3D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    tmp.setZ(static_cast<float>(location.z()+mBoxProperties->getHeight()/2));

    mBoxProperties->setLocation(tmp);
}


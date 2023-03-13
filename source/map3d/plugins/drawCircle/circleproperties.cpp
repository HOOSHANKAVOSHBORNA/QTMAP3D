#include "circleproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>



CirclePropertiesModel::CirclePropertiesModel(MapController *mapController, QObject *parent) :
    QObject(parent),
    mMapController(mapController)
{
    //    QObject::connect(this,&CirclePropertiesModel::circlePropertiesChanged,this,&CirclePropertiesModel::circlePropertiesChangedToQML);

}

QString CirclePropertiesModel::getFillcolor() const
{
    return mFillcolor;
}
void CirclePropertiesModel:: setFillColor(const QString &value){
    if(value == mFillcolor)
        return;
    mFillcolor = value;
    if(mCircle){
        osgEarth::Color tmpColor = mCircle->getColor();
        float A = tmpColor.a();
        tmpColor  = value.toStdString();
        tmpColor.a() = A;
        mCircle->setColor(tmpColor);
    }
}


QString CirclePropertiesModel::getLinecolor() const
{
    return mLinecolor;
}
void CirclePropertiesModel:: setLineColor(const QString &value){
    if(value == mLinecolor)
        return;
    mLinecolor = value;
    if(mCircle){
        mCircle->setLineColor(mLinecolor.toStdString());
    }
}


QVector3D CirclePropertiesModel::getLocation() const
{
    return mLocation;
}
void CirclePropertiesModel:: setLocation(const QVector3D &value){
    if(value == mLocation)
        return;
    mLocation = value;
    if(mCircle){
        osgEarth::GeoPoint tempLocation =  mCircle->getPosition();
        tempLocation.x() = static_cast<double>(value.x());
        tempLocation.y() = static_cast<double>(value.y());
        tempLocation.z() =static_cast<double>( value.z());
        mCircle->setPosition(tempLocation);
    }
}


double CirclePropertiesModel::getRadius() const
{
    return mRadius;
}
void CirclePropertiesModel::setRadius(const double &value){
    if(std::abs(value - mRadius) < 1)
        return;
    mRadius = value;

    if(mCircle){
        mCircle->setRadius(osgEarth::Distance(value));
    }
}


double CirclePropertiesModel::getCircleheight() const
{
    return mCircleHeight;
}
void CirclePropertiesModel::setCircleHeight(const double &value){
    if(std::abs(value - mCircleHeight) < 1)
        return;
    mCircleHeight = value;
    if(mCircle){
        mCircle->setCircleHeight(static_cast<float>(value));
    }
}


int CirclePropertiesModel::getTransparency() const
{
    return mTransparency;
}
void CirclePropertiesModel::setTransparency(const int &value){
    if(value == mTransparency)
        return;
    mTransparency = value;
    if(mCircle){
        float tempValue = value;
        osg::Vec4f tempColor = mCircle->getColor();
        tempColor.a() = tempValue /100;
        mCircle->setColor(osg::Vec4f(tempColor));
    }
}


double CirclePropertiesModel::getArcstart() const
{
    return mArcstart;
}
void CirclePropertiesModel::setArcstart(const double &value){
    if(std::abs(value - mArcstart) < 0.5)
        return;
    mArcstart = value;
    if(mCircle){
        mCircle->setArcStart(value);
    }
}

double CirclePropertiesModel::getArcend() const
{
    return mArcend;
}
void CirclePropertiesModel::setArcend(const double &value){
    if(std::abs(value - mArcend) < 0.5)
        return;
    mArcend = value;
    if(mCircle){
        mCircle->setArcEnd(value);
    }
}


int CirclePropertiesModel::getClamp() const
{
    return mClamp;
}
void CirclePropertiesModel::setClamp(int value){
    if(value == mClamp)
        return;
    mClamp = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(value);
    if(mCircle){
        mCircle->setClamp(mClamp);
    }
}


bool CirclePropertiesModel::getRelative() const
{
    return mRelative;
}
void CirclePropertiesModel::setRelative(const bool &value){
    if(value == mRelative)
        return;
    mRelative = value;
    if(mCircle){
        osgEarth::GeoPoint tempLocation =  mCircle->getPosition();

        if(value == true)
        {
            tempLocation.makeRelative(mMapController->getMapNode()->getTerrain());
            mCircle->setPosition(tempLocation);
        }
        else if(value == false)
        {
            tempLocation.makeAbsolute(mMapController->getMapNode()->getTerrain());
            mCircle->setPosition(tempLocation);
        }
    }
}

void CirclePropertiesModel::setCircle(Circle *circle)
{
    mCircle = circle;
    if(!circle){
        return;
    }
    osgEarth::Color tmpColor = mCircle->getColor();
    float opacity = tmpColor.a();
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = opacity;
    mCircle->setColor(tmpColor);
    mCircle->setCircleHeight(static_cast<float>(mCircleHeight));
    mCircle->setClamp(mClamp);
    mCircle->setArcStart(mArcstart);
    mCircle->setArcEnd(mArcend);
    mCircle->setRadius(mRadius);
}


CircleProperties::CircleProperties(QQmlEngine *engine, UIHandle *uiHandle, MapController *mapController, QObject *parent) :
    QObject(parent),
    mQmlEngine(engine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapController](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mCircleProperties = new CirclePropertiesModel(mapController);
            mItem->setProperty("circleProperties", QVariant::fromValue<CirclePropertiesModel*>(mCircleProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/CircleProperty.qml"));

}

void CircleProperties::show()
{
    mUiHandle->propertiesShow(mItem);
}

void CircleProperties::hide()
{
    mUiHandle->propertiesHide(mItem);
}

void CircleProperties::setCircle(Circle *circle)
{
    mCircleProperties->setCircle(circle);
}

#include "circleproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>



CirclePropertiesModel::CirclePropertiesModel(QObject *parent) :
    QObject(parent)
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
    //    if(mCircle){
    //        mCircle->set
    //    }
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
        tempLocation.x() = value.x();
        tempLocation.y() = value.y();
        tempLocation.z() = value.z();
        mCircle->setPosition(tempLocation);
    }
}


double CirclePropertiesModel::getRadius() const
{
    return mRadius;
}
void CirclePropertiesModel::setRadius(const double &value){
    if(value == mRadius)
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
    if(value == mCircleHeight)
        return;
    mCircleHeight = value;
    if(mCircle){
        mCircle->setCircleHeight(value);
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
    if(value == mArcstart)
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
    if(value == mArcend)
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
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = mTransparency/100;
    mCircle->setColor(tmpColor);
    mCircle->setCircleHeight(mCircleHeight);
    mCircle->setClamp(mClamp);
    mCircle->setArcStart(mArcstart);
    mCircle->setArcEnd(mArcend);
    mCircle->setRadius(mRadius);
}


CircleProperties::CircleProperties(QQmlEngine *engine, UIHandle *uiHandle, QObject *parent) :
    QObject(parent),
    mQmlEngine(engine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mCircleProperties = new CirclePropertiesModel();
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

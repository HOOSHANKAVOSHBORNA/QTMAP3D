#include "circleproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>



CirclePropertiesModel::CirclePropertiesModel(Circle *circle, MapController *mapController, QObject *parent) :
    QObject(parent),
    mCircle(circle),
    mMapController(mapController)
{
//       QObject::connect(this,&CirclePropertiesModel::circlePropertiesChanged,this,&CirclePropertiesModel::circlePropertiesChangedToQML);
    if (mCircle) {
        mLocation.setX(static_cast<float>(mCircle->getPosition().x()));
        mLocation.setY(static_cast<float>(mCircle->getPosition().y()));
        mLocation.setZ(static_cast<float>(mCircle->getPosition().z()));
    }
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

void CirclePropertiesModel:: setLocation(const QVector3D  &value){
    if(value == mLocation)
        return;
    mLocation = value;

    if(mCircle){
        osgEarth::GeoPoint tempLocation =  mCircle->getPosition();
        tempLocation.x() = static_cast<double>(value.x());
        tempLocation.y() = static_cast<double>(value.y());
        tempLocation.z() = static_cast<double>(value.z());
        mCircle->setPosition(tempLocation);
        emit positionToQmlChanged();
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
//    if(value == mTransparency)
//        return;
    mTransparency = value;
    if(mCircle){
        float tempValue = value;
        osg::Vec4f tempColor = mCircle->getColor();
        tempColor.a() = tempValue /100;
        mCircle->setColor(osg::Vec4f(tempColor));
    }
}


int CirclePropertiesModel::getLineOpacity() const
{
    return mLineOpacity;
}
void CirclePropertiesModel::setLineOpacity(const int &value){
    if(value == mLineOpacity)
        return;
    mLineOpacity = value;
    if(mCircle){
        float tempValue = value;
        osg::Vec4f tempColor = mCircle->getLineColor();
        tempColor.a() = tempValue /100;
        mCircle->setLineColor(osg::Vec4f(tempColor));
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

double CirclePropertiesModel::getLineWidth() const
{
    return mLineWidth;
}

void CirclePropertiesModel::setLineWidth(double line)
{
    mLineWidth = line;
    if (mCircle)
        mCircle->setLineWidth(static_cast<float>(mLineWidth));
}

void CirclePropertiesModel::setCircle(Circle *circle)
{
    mCircle = circle;
    if(!circle){
        return;
    }
    osgEarth::Color tmpColor = mCircle->getColor();
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = mTransparency;
    mCircle->setColor(tmpColor);
    osgEarth::Color mpColor = mCircle->getLineColor();
    mpColor  = mLinecolor.toStdString();
    mpColor.a() = mLineOpacity;
    mCircle->setLineColor(tmpColor);


    mCircle->setLineWidth(static_cast<float>(mLineWidth));
    setTransparency(mTransparency);
    mCircle->setCircleHeight(static_cast<float>(mCircleHeight));
    mCircle->setClamp(mClamp);
    mCircle->setArcStart(mArcstart);
    mCircle->setArcEnd(mArcend);
    mCircle->setRadius(mRadius);
}

CircleProperties::CircleProperties(Circle* circle, QQmlEngine *engine, UIHandle *uiHandle, MapController *mapController, QObject *parent) :
    QObject(parent),
    mQmlEngine(engine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, mapController, circle](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mCircleProperties = new CirclePropertiesModel(circle, mapController);
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

void CircleProperties::setLocation(osgEarth::GeoPoint location)
{
    QVector3D tmp;
    tmp.setX(static_cast<float>(location.x()));
    tmp.setY(static_cast<float>(location.y()));
    tmp.setZ(static_cast<float>(location.z()));

    mCircleProperties->setLocation(tmp);
}



#include "circleproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>



CirclePropertiesModel::CirclePropertiesModel(QObject *parent) :
    QObject(parent)
{
    QObject::connect(this,&CirclePropertiesModel::circlePropertiesChanged,this,&CirclePropertiesModel::circlePropertiesChangedToQML);

}

QString CirclePropertiesModel::fillcolor() const
{
    return mFillcolor;
}
void CirclePropertiesModel:: setFillColor(const QString &value){
    if(value == mFillcolor)
        return;
    mFillcolor = value;
    emit circlePropertiesChanged(FillColor , value);

    osgEarth::Color tmpColor = mCircle->getColor();
    float A = tmpColor.a();
    tmpColor  = value.toStdString();
    tmpColor.a() = A;
    mCircle->setColor(tmpColor);



}

QString CirclePropertiesModel::linecolor() const
{
    return mLinecolor;
}
void CirclePropertiesModel:: setLineColor(const QString &value){
    if(value == mLinecolor)
        return;
    mLinecolor = value;
    emit circlePropertiesChanged(LineColor , value);
}


QVector3D CirclePropertiesModel::location() const
{
    return mLocation;
}
void CirclePropertiesModel:: setLocation(const QVector3D &value){
    if(value == mLocation)
        return;
    mLocation = value;
    emit circlePropertiesChanged(Location , value);

    osgEarth::GeoPoint tempLocation =  mCircle->getPosition();
    tempLocation.x() = value.x();
    tempLocation.y() = value.y();
    tempLocation.z() = value.z();
    mCircle->setPosition(tempLocation);
}


double CirclePropertiesModel::radius() const
{
    return mRadius;
}
void CirclePropertiesModel::setRadius(const double &value){
    if(value == mRadius)
        return;
    mRadius = value;
    emit circlePropertiesChanged(Radius, value);

    mCircle->setRadius(osgEarth::Distance(value));
}


double CirclePropertiesModel::circleheight() const
{
    return mCircleHeight;
}
void CirclePropertiesModel::setCircleHeight(const double &value){
    if(value == mCircleHeight)
        return;
    mCircleHeight = value;
    emit circlePropertiesChanged(CircleHeight, value);

    mCircle->setCircleHeight(value);
}


int CirclePropertiesModel::transparency() const
{
    return mTransparency;
}
void CirclePropertiesModel::setTransparency(const int &value){
    if(value == mTransparency)
        return;
    mTransparency = value;
    emit circlePropertiesChanged(Transparency, value);
    float tempValue = value;
    osg::Vec4f tempColor = mCircle->getColor();
    tempColor.a() = tempValue /100;
    mCircle->setColor(osg::Vec4f(tempColor));

}


double CirclePropertiesModel::arcstart() const
{
    return mArcstart;
}
void CirclePropertiesModel::setArcstart(const double &value){
    if(value == mArcstart)
        return;
    mArcstart = value;
    emit circlePropertiesChanged(ArcStart, value);

    mCircle->setArcStart(value);
}

double CirclePropertiesModel::arcend() const
{
    return mArcend;
}
void CirclePropertiesModel::setArcend(const double &value){
    if(value == mArcend)
        return;
    mArcend = value;
    emit circlePropertiesChanged(ArcEnd, value);

    mCircle->setArcEnd(value);
}


osgEarth::Symbology::AltitudeSymbol::Clamping CirclePropertiesModel::clamp() const
{
    return mClamp;
}
void CirclePropertiesModel::setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping &value){
    if(value == mClamp)
        return;
    mClamp = value;
    emit circlePropertiesChanged(Clamp, value);

    mCircle->setClamp(value);
}


bool CirclePropertiesModel::relative() const
{
    return mRelative;
}
void CirclePropertiesModel::setRelative(const bool &value){
    if(value == mRelative)
        return;
    mRelative = value;
    emit circlePropertiesChanged(Relative, value);
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

void CirclePropertiesModel::setCircle(Circle *circle)
{
    mCircle = circle;
    mFillcolor = QString::fromStdString(mCircle->getColor().toHTML());
    mFillcolor.remove(7,2);
//    mCircleHeight = circle->getCircleHeight();
    mLocation.setX(mCircle->getPosition().x());
    mLocation.setY(mCircle->getPosition().y());
    mLocation.setZ(mCircle->getPosition().z());
    mRelative = mCircle->getPosition().isRelative();
    mRadius = mCircle->getRadius().as(osgEarth::Units::METERS);
    mArcstart = mCircle->getArcStart().getValue();
    mArcend = mCircle->getArcEnd().getValue();
//    mClamp = circle->getClamp();
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

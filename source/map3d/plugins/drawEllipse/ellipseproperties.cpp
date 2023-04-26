#include "ellipseproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>



EllipsePropertiesModel::EllipsePropertiesModel(Ellipse *ellipse, MapController *mapController, QObject *parent) :
    QObject(parent),
    mEllipse(ellipse),
    mMapController(mapController)
{

}

QString EllipsePropertiesModel::getFillcolor() const
{
    return mFillcolor;
}

void EllipsePropertiesModel:: setFillColor(const QString &value){
    if(value == mFillcolor)
        return;
    mFillcolor = value;
    if(mEllipse){
        osgEarth::Color tmpColor = mEllipse->getColor();
        float A = tmpColor.a();
        tmpColor  = value.toStdString();
        tmpColor.a() = A;
        mEllipse->setColor(tmpColor);
    }
}


QString EllipsePropertiesModel::getLinecolor() const
{
    return mLinecolor;
}

void EllipsePropertiesModel:: setLineColor(const QString &value){
    if(value == mLinecolor)
        return;
    mLinecolor = value;
    if(mEllipse){
        mEllipse->setStrokeColor(mLinecolor.toStdString());
    }
}



double EllipsePropertiesModel::getEllipseheight() const
{
    return mEllipseHeight;
}

void EllipsePropertiesModel::setEllipseHeight(const double &value){
    if(std::abs(value - mEllipseHeight) < 1)
        return;
    mEllipseHeight = value;
    if(mEllipse){
        mEllipse->setHeight(static_cast<float>(value));
    }
}

int EllipsePropertiesModel::getTransparency() const
{
    return mTransparency;
}

void EllipsePropertiesModel::setTransparency(const int &value){
//    if(value == mTransparency)
//        return;
    mTransparency = value;
    if(mEllipse){
        float tempValue = value;
        osg::Vec4f tempColor = mEllipse->getColor();
        tempColor.a() = tempValue /100;
        mEllipse->setColor(osg::Vec4f(tempColor));
    }
}


int EllipsePropertiesModel::getLineOpacity() const
{
    return mLineOpacity;
}
void EllipsePropertiesModel::setLineOpacity(const int &value){
    if(value == mLineOpacity)
        return;
    mLineOpacity = value;
    if(mEllipse){
        float tempValue = value;
        osg::Vec4f tempColor = mEllipse->getStrokeColor();
        tempColor.a() = tempValue /100;
        mEllipse->setStrokeColor(osg::Vec4f(tempColor));
    }
}




int EllipsePropertiesModel::getClamp() const
{
    return mClamp;
}

void EllipsePropertiesModel::setClamp(int value){
    if(value == mClamp)
        return;
    mClamp = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(value);
    if(mEllipse){
        mEllipse->setClamp(mClamp);
    }
}



double EllipsePropertiesModel::getLineWidth() const
{
    return mLineWidth;
}

void EllipsePropertiesModel::setLineWidth(double line)
{
    mLineWidth = line;
    if (mEllipse)
        mEllipse->setStrokeWidth(static_cast<float>(mLineWidth));
}

void EllipsePropertiesModel::setEllipse(Ellipse *ellipse)
{
    mEllipse = ellipse;
    if(!ellipse){
        return;
    }
    osgEarth::Color tmpColor = mEllipse->getColor();
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = mTransparency;
    mEllipse->setColor(tmpColor);
    osgEarth::Color mpColor = mEllipse->getStrokeColor();
    mpColor  = mLinecolor.toStdString();
    mpColor.a() = mLineOpacity;
    mEllipse->setStrokeColor(mpColor);


    mEllipse->setStrokeWidth(static_cast<float>(mLineWidth));
    setTransparency(mTransparency);
    mEllipse->setHeight(static_cast<float>(mEllipseHeight));
    mEllipse->setClamp(mClamp);

}

EllipseProperties::EllipseProperties(Ellipse* ellipse, QQmlEngine *engine, UIHandle *uiHandle, QObject *parent) :
    QObject(parent),
    mQmlEngine(engine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, ellipse](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mEllipseProperties = new EllipsePropertiesModel(ellipse);
            mItem->setProperty("ellipseProperties", QVariant::fromValue<EllipsePropertiesModel*>(mEllipseProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/EllipseProperty.qml"));

}

void EllipseProperties::show()
{
    mUiHandle->propertiesShow(mItem);
}

void EllipseProperties::hide()
{
    mUiHandle->propertiesHide(mItem);
}

void EllipseProperties::setEllipse(Ellipse *ellipse)
{
    mEllipseProperties->setEllipse(ellipse);
}



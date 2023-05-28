#include "rectproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>



RectPropertiesModel::RectPropertiesModel(Rect *rect, MapItem *mapItem, QObject *parent) :
    QObject(parent),
    mRect(rect),
    mMapItem(mapItem)
{

}

QString RectPropertiesModel::getFillcolor() const
{
    return mFillcolor;
}

void RectPropertiesModel:: setFillColor(const QString &value){
    if(value == mFillcolor)
        return;
    mFillcolor = value;
    if(mRect){
        osgEarth::Color tmpColor = mRect->getColor();
        float A = tmpColor.a();
        tmpColor  = value.toStdString();
        tmpColor.a() = A;
        mRect->setColor(tmpColor);
    }
}


QString RectPropertiesModel::getLinecolor() const
{
    return mLinecolor;
}

void RectPropertiesModel:: setLineColor(const QString &value){
    if(value == mLinecolor)
        return;
    mLinecolor = value;
    if(mRect){
        mRect->setStrokeColor(mLinecolor.toStdString());
    }
}



int RectPropertiesModel::getTransparency() const
{
    return mTransparency;
}

void RectPropertiesModel::setTransparency(const int &value){
//    if(value == mTransparency)
//        return;
    mTransparency = value;
    if(mRect){
        float tempValue = value;
        osg::Vec4f tempColor = mRect->getColor();
        tempColor.a() = tempValue /100;
        mRect->setColor(osg::Vec4f(tempColor));
    }
}


int RectPropertiesModel::getLineOpacity() const
{
    return mLineOpacity;
}
void RectPropertiesModel::setLineOpacity(const int &value){
    if(value == mLineOpacity)
        return;
    mLineOpacity = value;
    if(mRect){
        float tempValue = value;
        osg::Vec4f tempColor = mRect->getStrokeColor();
        tempColor.a() = tempValue /100;
        mRect->setStrokeColor(osg::Vec4f(tempColor));
    }
}




int RectPropertiesModel::getClamp() const
{
    return mClamp;
}

void RectPropertiesModel::setClamp(int value){
    if(value == mClamp)
        return;
    mClamp = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(value);
    if(mRect){
        mRect->setClamp(mClamp);
    }
}

double RectPropertiesModel::getLineWidth() const
{
    return mLineWidth;
}

void RectPropertiesModel::setLineWidth(double line)
{
    mLineWidth = line;
    if (mRect)
        mRect->setStrokeWidth(static_cast<float>(mLineWidth));
}



double RectPropertiesModel::getWidth() const
{
    return mWidth;
}

void RectPropertiesModel::setWidth(double width)
{
    mWidth = width;
    if (mRect)
        mRect->setWidth(static_cast<double>(mWidth));
}


double RectPropertiesModel::getHeight() const
{
    return mHeight;
}

void RectPropertiesModel::setHeight(double height)
{
    mHeight = height;
    if (mRect)
        mRect->setHeight(static_cast<double>(mHeight));
}



void RectPropertiesModel::setRect(Rect *rect)
{
    mRect = rect;
    if(!rect){
        return;
    }
    osgEarth::Color tmpColor = mRect->getColor();
    tmpColor  = mFillcolor.toStdString();
    tmpColor.a() = mTransparency;
    mRect->setColor(tmpColor);
    osgEarth::Color mpColor = mRect->getStrokeColor();
    mpColor  = mLinecolor.toStdString();
    mpColor.a() = mLineOpacity;
    mRect->setStrokeColor(mpColor);


    mRect->setStrokeWidth(static_cast<float>(mLineWidth));
    mRect->setWidth(static_cast<double>(mWidth));
    mRect->setHeight(static_cast<double>(mHeight));
    setTransparency(mTransparency);
    mRect->setClamp(mClamp);

}

RectProperties::RectProperties(Rect* rect, QQmlEngine *engine, UIHandle *uiHandle, QObject *parent) :
    QObject(parent),
    mQmlEngine(engine),
    mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, rect](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mRectProperties = new RectPropertiesModel(rect);
            mItem->setProperty("rectProperties", QVariant::fromValue<RectPropertiesModel*>(mRectProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/RectProperty.qml"));

}

void RectProperties::show()
{
    mUiHandle->propertiesShow(mItem);
}

void RectProperties::hide()
{
    mUiHandle->propertiesHide(mItem);
}

void RectProperties::setRect(Rect *rect)
{
    mRectProperties->setRect(rect);
}



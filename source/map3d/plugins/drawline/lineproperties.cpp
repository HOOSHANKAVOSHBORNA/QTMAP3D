#include "lineproperties.h"
#include <QtDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include "plugininterface.h"



LinePropertiesModel::LinePropertiesModel(QObject *parent) :

    QObject(parent)
{
    QObject::connect(this,&LinePropertiesModel::linePropertiesChanged,this,&LinePropertiesModel::linePropertiesChangedToQML);
}

QString LinePropertiesModel::color() const
{
    return mColor;
}
void LinePropertiesModel:: setColor(const QString &value){
    if(value == mColor)
        return;
    mColor = value;
    if(mLineNode){
        mLineNode->setColor(value.toStdString());
    }

}

QString LinePropertiesModel::pointColor() const
{
    return mPointColor;
}
void LinePropertiesModel:: setPointColor(const QString &value){
    if(value == mPointColor)
        return;
    mPointColor = value;
    if(mLineNode){
        mLineNode->setPointColor(value.toStdString());
    }

}

float LinePropertiesModel::width() const
{
    return mWidth;
}
void LinePropertiesModel:: setWidth(const float &value){
    if(value == mWidth)
        return;
    mWidth = value;
    if(mLineNode){
        mLineNode->setWidth(value);
    }
}

float LinePropertiesModel::height() const
{
    return mHeight;
}
void LinePropertiesModel:: setHeight(const float &value){
    if(value == mHeight)
        return;
    mHeight = value;
    if(mLineNode){
        mLineNode->setHeight(value);
    }
}

float LinePropertiesModel::pointwidth() const
{
    return mPointwidth;
}
void LinePropertiesModel::setPointwidth(const float &value){
    if(value == mPointwidth)
        return;
    mPointwidth = value;
    if(mLineNode){
        mLineNode->setPointWidth(value);
    }
}

unsigned LinePropertiesModel::tesselation() const
{
    return mTesselation;
}
void LinePropertiesModel::setTesselation(const unsigned &value){
    if(value == mTesselation)
        return;
    mTesselation = value;
    if(mLineNode){
        mLineNode->setTessellation(value);
    }
}

osgEarth::Symbology::AltitudeSymbol::Clamping  LinePropertiesModel::clamp() const
{
    return mClamp;
}
void LinePropertiesModel::setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping  &value){
    if(value == mClamp)
        return;
    mClamp = value;
    if(mLineNode){
        mLineNode->setClamp(value);
    }
}

bool LinePropertiesModel::visible() const
{
    return mVisible;
}
void LinePropertiesModel::setVisible(const bool &value){
    if(value == mVisible)
        return;
    mVisible = value;
    if(mLineNode){
        mLineNode->setPointVisible(value);
    }
}

bool LinePropertiesModel::smooth() const
{
    return mSmooth;
}
void LinePropertiesModel::setSmooth(const bool &value){
    if(value == mSmooth)
        return;
    mSmooth = value;
    if(mLineNode){
        mLineNode->setSmooth(value);
    }
}

void LinePropertiesModel::setLine(LineNode* linNode)
{
    mLineNode = linNode;
    mLineNode->setColor(mColor.toStdString());
    mLineNode->setPointColor(mPointColor.toStdString());
    mLineNode->setWidth(mWidth);
    mLineNode->setTessellation(mTesselation);
    mLineNode->setPointWidth(mPointwidth);
    mLineNode->setSmooth(mSmooth);
    mLineNode->setPointVisible(mVisible);
}

LineProperties::LineProperties(QQmlEngine *engine,UIHandle *muiHandle, QObject *parent ):
    QObject(parent),
    mQmlEngine(engine),
    mUiHandle(muiHandle)
{
    //--show property window---------------------------------------------------------------------------------
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mLineProperties = new LinePropertiesModel();
            mItem->setProperty("lineProperties", QVariant::fromValue<LinePropertiesModel*>(mLineProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/resources/LineProperty.qml"));
    //--------------------------------------------------------------------------------------------------

}

Q_INVOKABLE void LineProperties::show()
{
    mUiHandle->propertiesShow(mItem);
}

void LineProperties::hide()
{

    mUiHandle->propertiesHide(mItem);
}

void LineProperties::setLine(LineNode *line)
{
    mLine = line;
    mLineProperties->setLine(line);
}

#include "lineproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>



LinePropertiesModel::LinePropertiesModel(LineNode* lineNode, QObject *parent) :

    QObject(parent),
    mLineNode(lineNode)
{
       QObject::connect(this,&LinePropertiesModel::linePropertiesChanged,this,&LinePropertiesModel::linePropertiesChangedToQML);


//       mRadius = lineNode->getRadius().as(osgEarth::Units::METERS);
         mColor = QString::fromStdString(lineNode->getColor().toHTML());
         mColor.remove(7,2);
         mPointColor = QString::fromStdString(lineNode->getPointColor().toHTML());
         mPointColor.remove(7,2);


}

QString LinePropertiesModel::color() const
{
    return mColor;
}
void LinePropertiesModel:: setColor(const QString &value){
    if(value == mColor)
        return;
    mColor = value;
    emit linePropertiesChanged(Color , value);
    mLineNode->setColor(value.toStdString());

}

QString LinePropertiesModel::pointColor() const
{
    return mPointColor;
}
void LinePropertiesModel:: setPointColor(const QString &value){
    if(value == mPointColor)
        return;
    mPointColor = value;
    emit linePropertiesChanged(PointColor , value);
    mLineNode->setPointColor(value.toStdString());

}

float LinePropertiesModel::width() const
{
    return mWidth;
}
void LinePropertiesModel:: setWidth(const float &value){
    if(value == mWidth)
        return;
    mWidth = value;
    emit linePropertiesChanged(Width , value);
    mLineNode->setWidth(value);
}

float LinePropertiesModel::height() const
{
    return mHeight;
}
void LinePropertiesModel:: setHeight(const float &value){
    if(value == mHeight)
        return;
    mHeight = value;
    emit linePropertiesChanged(Height , value);
    mLineNode->setHeight(value);
}

float LinePropertiesModel::pointwidth() const
{
    return mPointwidth;
}
void LinePropertiesModel::setPointwidth(const float &value){
    if(value == mPointwidth)
        return;
    mPointwidth = value;
    emit linePropertiesChanged(Pointwidth, value);

    mLineNode->setPointWidth(value);
}

unsigned LinePropertiesModel::tesselation() const
{
    return mTesselation;
}
void LinePropertiesModel::setTesselation(const unsigned &value){
    if(value == mTesselation)
        return;
    mTesselation = value;
    emit linePropertiesChanged(Tesselation, value);


    mLineNode->setTessellation(value);

}

osgEarth::Symbology::AltitudeSymbol::Clamping  LinePropertiesModel::clamp() const
{
    return mClamp;
}
void LinePropertiesModel::setClamp(const osgEarth::Symbology::AltitudeSymbol::Clamping  &value){
    if(value == mClamp)
        return;
    mClamp = value;
    emit linePropertiesChanged(Clamp, value);

    mLineNode->setClamp(value);
}

bool LinePropertiesModel::visible() const
{
    return mVisible;
}
void LinePropertiesModel::setVisible(const bool &value){
    if(value == mVisible)
        return;
    mVisible = value;
    emit linePropertiesChanged(Visible, value);

    mLineNode->setPointVisible(value);
}

bool LinePropertiesModel::smooth() const
{
    return mSmooth;
}
void LinePropertiesModel::setSmooth(const bool &value){
    if(value == mSmooth)
        return;
    mSmooth = value;
    emit linePropertiesChanged(Smooth, value);

    mLineNode->setSmooth(value);
}

LineProperties::LineProperties(QQmlEngine *engine, LineNode *line, QObject *parent):
    QObject(parent),
    mQmlEngine(engine),
    mLine(line)
{
    //--show property window---------------------------------------------------------------------------------
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            LinePropertiesModel *lineProperties = new LinePropertiesModel(mLine);
            mItem->setProperty("lineProperties", QVariant::fromValue<LinePropertiesModel*>(lineProperties));
        }
    });
    comp->loadUrl(QUrl("qrc:/resources/LineProperty.qml"));
    //--------------------------------------------------------------------------------------------------

}

void LineProperties::show()
{
    QMetaObject::invokeMethod(mItem, "show");
}

void LineProperties::hide()
{
    QMetaObject::invokeMethod(mItem, "hide");
}

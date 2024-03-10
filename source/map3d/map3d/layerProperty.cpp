#include <osg/Material>
#include <osgEarth/Color>
#include <osgEarth/VisibleLayer>

#include "layerProperty.h"
#include "utility.h"

LayerPropertyItem::LayerPropertyItem(QObject *parent) : QObject(parent){}

QString LayerPropertyItem::name() const
{
    return mName;
}

void LayerPropertyItem::setName(const QString &newName)
{
    mName = newName;

    if (!mModelNodeLayer)
        return;

    mModelNodeLayer->setName(mName.toStdString());
    emit nameChanged();
}

QColor LayerPropertyItem::color() const
{
    return mColor;
}

void LayerPropertyItem::setColor(const QColor &newColor)
{
    mColor = newColor;

    if (!mModelNodeLayer)
        return;

    if(newColor.alphaF() != 0.0){
    osgEarth::Color color = Utility::qColor2osgEarthColor(mColor);
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse(osg::Material::FRONT_AND_BACK, color);
    mModelNodeLayer->getOrCreateStateSet()
        ->setAttributeAndModes(mat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

    if(mLayerSetting){
        mLayerSetting->beginGroup("layer");
        mSettingList.replace(0,mColor.name());
        mLayerSetting->setValue(QString::number(mModelNodeLayer->getUID()),mSettingList);
        mLayerSetting->endGroup();
    }
    }else{
        mModelNodeLayer->getOrCreateStateSet()->removeAttribute(osg::StateAttribute::MATERIAL);
        if(mLayerSetting){
            mLayerSetting->beginGroup("layer");
            mSettingList.replace(0,NULL);
            mLayerSetting->setValue(QString::number(mModelNodeLayer->getUID()),mSettingList);
            mLayerSetting->endGroup();
        }
    }

    emit colorChanged();


}

bool LayerPropertyItem::isVisible() const
{
    return mIsVisible;
}

void LayerPropertyItem::setIsVisible(bool newIsVisible)
{
    mIsVisible = newIsVisible;

    if (!mModelNodeLayer)
        return;


    auto layerVisible = dynamic_cast<osgEarth::VisibleLayer *>(mModelNodeLayer.get());
    layerVisible->setVisible(newIsVisible);

    emit isVisibleChanged();

    if(mLayerSetting){
        mLayerSetting->beginGroup("layer");
        mSettingList.replace(1,QVariant(newIsVisible).toString());
        mLayerSetting->setValue(QString::number(mModelNodeLayer->getUID()),mSettingList);
        mLayerSetting->endGroup();
    }
}

osg::ref_ptr<osgEarth::Layer> LayerPropertyItem::modelNodeLayer() const
{
    return mModelNodeLayer;
}

void LayerPropertyItem::setModelNodeLayer(const osg::ref_ptr<osgEarth::Layer> &newModelNodeLayer)
{
    mModelNodeLayer = newModelNodeLayer;
    if (!mModelNodeLayer)
        return;
    setName(QString::fromStdString(mModelNodeLayer->getName()));
}

double LayerPropertyItem::opacity() const
{
    return mOpacity;
}

void LayerPropertyItem::setOpacity(double newOpacity)
{
    mOpacity = newOpacity;

    if (!mModelNodeLayer)
        return;

    mColor.setAlphaF(newOpacity+0.001);
    setColor(mColor);


    emit opacityChanged();

}

QSettings *LayerPropertyItem::getLayerSettings()
{
    return mLayerSetting;
}

void LayerPropertyItem::setLayerSettings(QSettings *setting)
{

    mLayerSetting = setting;
    mLayerSetting->beginGroup("layer");
    if(mModelNodeLayer){
        mLayerSetting->setValue(QString::number(mModelNodeLayer->getUID()),mSettingList);
    }
    mSettingList.resize(3);
    mLayerSetting->endGroup();
}

QList<QString> LayerPropertyItem::getSettingList()
{
    return mSettingList;
}


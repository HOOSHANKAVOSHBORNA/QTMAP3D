#include <osg/Material>
#include <osgEarth/Color>

#include "layerProperty.h"
#include "utility.h"

QString LayerPropertyItem::name() const
{
    return mName;
}

void LayerPropertyItem::setName(const QString &newName)
{
    mName = newName;
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

    osgEarth::Color color = Utility::qColor2osgEarthColor(mColor);

    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse(osg::Material::FRONT_AND_BACK, color);
    mModelNodeLayer->getOrCreateStateSet()
        ->setAttributeAndModes(mat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

    emit colorChanged();
}

bool LayerPropertyItem::isVisible() const
{
    return mIsVisible;
}

void LayerPropertyItem::setIsVisible(bool newIsVisible)
{
    mIsVisible = newIsVisible;
    // TODO
    //    mModelNodeLayer->setVisible(mIsVisible);
    emit isVisibleChanged();
}

osg::ref_ptr<osgEarth::Layer> LayerPropertyItem::modelNodeLayer() const
{
    return mModelNodeLayer;
}

void LayerPropertyItem::setModelNodeLayer(const osg::ref_ptr<osgEarth::Layer> &newModelNodeLayer)
{
    mModelNodeLayer = newModelNodeLayer;

    setName(QString::fromStdString(mModelNodeLayer->getName()));
    // TODO
}

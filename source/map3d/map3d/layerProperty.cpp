#include "layerProperty.h"

QString LayerPropertyItem::name() const
{
    return mName;
}

void LayerPropertyItem::setName(const QString &newName)
{
    qDebug() << "here";
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
    // TODO
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
    qDebug() << mModelNodeLayer->getName();
    setName(QString::fromStdString(mModelNodeLayer->getName()));
    // TODO
}

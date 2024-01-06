#ifndef LAYERPROPERTY_H
#define LAYERPROPERTY_H

#include <QColor>
#include <QObject>
#include <QQuickItem>
#include <osg/Referenced>
#include <osgEarth/Layer>

// ----------------------------------------------------------------------------- class prototypes
class LayerPropertyItem;

// ----------------------------------------------------------------------------- manager of this is in layerManger files

// ----------------------------------------------------------------------------- interface for qml
class LayerPropertyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setIsVisible NOTIFY isVisibleChanged FINAL)

public:
    osg::ref_ptr<osgEarth::Layer> modelNodeLayer() const;
    void setModelNodeLayer(const osg::ref_ptr<osgEarth::Layer> &newModelNodeLayer);

    QString name() const;
    void setName(const QString &newName);

    QColor color() const;
    void setColor(const QColor &newColor);

    bool isVisible() const;
    void setIsVisible(bool newIsVisible);

signals:
    void nameChanged();
    void colorChanged();
    void isVisibleChanged();

private:
    osg::ref_ptr<osgEarth::Layer> mModelNodeLayer;
    QString mName;
    QColor mColor;
    bool mIsVisible;
};

#endif // LAYERPROPERTY_H

#ifndef LAYERPROPERTY_H
#define LAYERPROPERTY_H

#include <QColor>
#include <QObject>
#include <QQuickItem>
#include <osg/Referenced>
#include <osgEarth/Layer>
#include <QSettings>

// ----------------------------------------------------------------------------- class prototypes
class LayerPropertyItem;

// ----------------------------------------------------------------------------- manager of this is in layerManger files

// ----------------------------------------------------------------------------- interface for qml
class LayerPropertyItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setIsVisible NOTIFY isVisibleChanged FINAL)

public:
    LayerPropertyItem(QObject* parent = nullptr);
    osg::ref_ptr<osgEarth::Layer> modelNodeLayer() const;
    void setModelNodeLayer(const osg::ref_ptr<osgEarth::Layer> &newModelNodeLayer);

    QString name() const;
    void setName(const QString &newName);

    QColor color() const;
    void setColor(const QColor &newColor);

    bool isVisible() const;
    void setIsVisible(bool newIsVisible);

    double opacity() const;
    void setOpacity(double newOpacity);

    QSettings* getLayerSettings();
    void setLayerSettings(QSettings *setting);
    QList<QString> getSettingList();


signals:
    void nameChanged();
    void colorChanged();
    void isVisibleChanged();

    void opacityChanged();

private:
    osg::ref_ptr<osgEarth::Layer> mModelNodeLayer;
    QString mName;
    QColor mColor;
    bool mIsVisible;
    double mOpacity;
    QSettings *mLayerSetting{nullptr};
    QList<QString> mSettingList;
};

#endif // LAYERPROPERTY_H

#ifndef QMLNODE_H
#define QMLNODE_H

#include <QQuickItem>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgGA/GUIEventHandler>

class QmlNode;

class QmlNodeEventHandler: public osgGA::GUIEventHandler
{

public:
    QmlNodeEventHandler(QmlNode *qmlNode);
    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;

private:
    QmlNode *mQmlNode;
};

class QmlNode: public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    QmlNode(QQmlEngine *qmlEngine = nullptr, QQuickItem *parent = nullptr);
    ~QmlNode();
    osgEarth::Annotation::GeoPositionNode *osgNode() const { return mOsgNode; }
    void setEngine(QQmlEngine *engine);

    void setOsgNode(osgEarth::Annotation::GeoPositionNode *newOsgNode);

private:
    QQmlEngine *mEngine;
    osgEarth::Annotation::GeoPositionNode *mOsgNode;
    QmlNodeEventHandler *mQmlNodeEventHandler{nullptr};
};

#endif // QMLNODE_H

#ifndef QMLNODE_H
#define QMLNODE_H

#include <QQuickItem>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgGA/GUIEventHandler>

class QmlNode;

class QmlNodeCallback : public osg::NodeCallback
{
public:
    QmlNodeCallback(QmlNode *qmlNode);
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;

private:
    QmlNode *mQmlNode;
};

class QmlNode: public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(double nodeRadius READ nodeRadius WRITE setNodeRadius NOTIFY nodeRadiusChanged FINAL)

public:
    QmlNode(QQmlEngine *qmlEngine = nullptr, QQuickItem *parent = nullptr);
    ~QmlNode();
    osgEarth::Annotation::GeoPositionNode *osgNode() const { return mOsgNode; }
    void setEngine(QQmlEngine *engine);

    void setOsgNode(osgEarth::Annotation::GeoPositionNode *newOsgNode);

    double nodeRadius() const;
    void setNodeRadius(double newNodeRadius);

    bool isShow() const;
    void show(bool show);

signals:
    void nodeRadiusChanged();

private:
    bool mIsShow{false};
    QQmlEngine *mEngine;
    osgEarth::Annotation::GeoPositionNode *mOsgNode;
    osg::ref_ptr<osg::NodeCallback> mNodeCallback{nullptr};
    double mNodeRadius;
};

#endif // QMLNODE_H

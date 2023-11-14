#include "qmlNode.h"
#include <QQmlEngine>
#include <QSGGeometryNode>
#include <QSGTexture>
#include <osgDB/ReadFile>
#include <osgViewer/View>

QmlNode::QmlNode(QQmlEngine *qmlEngine, QQuickItem *parent):
    mEngine(qmlEngine), QQuickItem(parent)
{

}

QmlNode::~QmlNode()
{
    mOsgNode->removeEventCallback(mQmlNodeEventHandler);
    delete mQmlNodeEventHandler;
}

void QmlNode::setEngine(QQmlEngine *engine)
{
    mEngine = engine;
}

void QmlNode::setOsgNode(osgEarth::Annotation::GeoPositionNode *newOsgNode)
{
    mOsgNode = newOsgNode;
    mOsgNode->removeEventCallback(mQmlNodeEventHandler);
    mQmlNodeEventHandler = new QmlNodeEventHandler(this);
    mOsgNode->addEventCallback(mQmlNodeEventHandler);
}

QmlNodeEventHandler::QmlNodeEventHandler(QmlNode *qmlNode)
{
    mQmlNode = qmlNode;
}

bool QmlNodeEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);

    if (!view)
    {
        return false;
    }
    if (ea.getEventType() == osgGA::GUIEventAdapter::FRAME){
        osg::Vec3d worldPoint;
        mQmlNode->osgNode()->getPosition().toWorld(worldPoint);
        float height = static_cast<float>(view->getCamera()->getViewport()->height());
        float width = static_cast<float>(view->getCamera()->getViewport()->width());

        const osg::Matrixd pMatrix = view->getCamera()->getProjectionMatrix();
        const osg::Matrixd vMatrix = view->getCamera()->getViewMatrix();
        osg::Vec3f result =   (worldPoint * vMatrix) * pMatrix;
        float outX = result.x() * (width/2.0f) + width/2.0f;
        float outY = result.y() * (height/2.0f) + height/2.0f;
        outY = height - outY;

//        mQmlNode->setX(outX);
//        mQmlNode->setY(outY);
//        mQmlNode->setZ(100);
        mQmlNode->setPosition(QPointF(outX, outY));

//        emit mQmlNode->xChanged();
//        emit mQmlNode->yChanged();
//        emit mQmlNode->zChanged();
        qDebug()<<outX;
        qDebug()<<"mask: "<<mQmlNode->osgNode()->getNodeMask();
    }

    return false;
}

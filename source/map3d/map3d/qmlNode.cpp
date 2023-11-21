#include "qmlNode.h"
#include <QQmlEngine>
#include <QSGGeometryNode>
#include <QSGTexture>
#include <osg/ComputeBoundsVisitor>
#include <osgDB/ReadFile>
#include <osgViewer/View>
#include <osgViewer/Viewer>

QmlNode::QmlNode(QQmlEngine *qmlEngine, QQuickItem *parent):
    mEngine(qmlEngine), QQuickItem(parent)
{

}

QmlNode::~QmlNode()
{
    mOsgNode->removeCullCallback(mNodeCallback);
    //    delete mNodeCallback;
}

void QmlNode::setEngine(QQmlEngine *engine)
{
    mEngine = engine;
}

void QmlNode::setOsgNode(osgEarth::Annotation::GeoPositionNode *newOsgNode)
{
    mOsgNode = newOsgNode;
    mOsgNode->removeCullCallback(mNodeCallback);
    mNodeCallback = new QmlNodeCallback(this);
    mOsgNode->addCullCallback(mNodeCallback);
}

double QmlNode::nodeRadius() const
{
    return mNodeRadius;
}

void QmlNode::setNodeRadius(double newNodeRadius)
{
    if (qFuzzyCompare(mNodeRadius, newNodeRadius))
        return;
//    qDebug() << "node radius: " << newNodeRadius;
    mNodeRadius = newNodeRadius;
    emit nodeRadiusChanged();
}

bool QmlNode::isShow() const
{
    return mIsShow;
}

void QmlNode::show(bool show)
{
    mIsShow = show;
    setVisible(show);
}

QmlNodeCallback::QmlNodeCallback(QmlNode *qmlNode)
{
    mQmlNode = qmlNode;
}

void QmlNodeCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    if(mQmlNode->isShow()){
        bool visible = false;

        osgUtil::CullVisitor* cullVisitor = nv->asCullVisitor();
        osg::Camera* cam = cullVisitor->getCurrentCamera();
        // If this is an RTT camera, we need to use it's "parent"
        // to calculate the proper scale factor.
        if (cam->isRenderToTextureCamera() &&
            cam->getView() &&
            cam->getView()->getCamera() &&
            cam->getView()->getCamera() != cam)
        {
            cam = cam->getView()->getCamera();
        }

        if (cam->getViewport())
        {
            osg::ComputeBoundsVisitor cbv;
            node->accept(cbv);
            auto center = cbv.getBoundingBox().center();
            //--screen point-------------------------------------------------------------
            const osg::Matrixd transmat
                = cam->getViewMatrix()
                  * cam->getProjectionMatrix()
                /** cam->getViewport()->computeWindowMatrix()*/;
            float height = static_cast<float>(cam->getViewport()->height());
            float width = static_cast<float>(cam->getViewport()->width());
            auto toScreen = [&](osg::Vec3 v) -> QPointF {
                osg::Vec3 vec(v.x(), v.y(), v.z()/*, 1.0*/);
                vec = vec * transmat;
                //            vec = vec / vec.w();
                vec.x() = vec.x() * (width/2.0f) + width/2.0f;
                vec.y() = vec.y() * (height/2.0f) + height/2.0f;
                return QPointF(static_cast<int>(vec.x()), static_cast<int>(height - vec.y()));
            };

            //--set position------------------------------------------------------------
            auto screenCenter = toScreen(center);
            mQmlNode->setPosition(screenCenter - QPointF(mQmlNode->width()/2, mQmlNode->height()/2));

            //--screen radius-----------------------------------------------------------
            std::vector<int> xs; xs.reserve(8);
            std::vector<int> ys; ys.reserve(8);
            for (int i=0; i < 8; i++) {
                QPointF p = toScreen(cbv.getBoundingBox().corner(i));
                xs.push_back(p.x());
                ys.push_back(p.y());
            };
            int xmin = *std::min_element(xs.begin(), xs.end());
            int xmax = *std::max_element(xs.begin(), xs.end());
            int ymin = *std::min_element(ys.begin(), ys.end());
            int ymax = *std::max_element(ys.begin(), ys.end());
            osg::Vec2 bottomLeft(xmin, ymin);
            osg::Vec2 upperRight(xmax, ymax);
            int screenRadius  = (upperRight - bottomLeft).length()/2.0;
            mQmlNode->setNodeRadius(screenRadius/2);

            //--distance to camera--------------------------------------------
            //        float camDistance = cullVisitor->getDistanceToEyePoint(center,true);
            //--check node is behind of earth--------------------------------
//            osgUtil::LineSegmentIntersector::Intersections intersections;
//            auto viewer = dynamic_cast<osgViewer::Viewer*>(cam->getView());
//            if (viewer && viewer->computeIntersections(screenCenter.x(), height - screenCenter.y(), intersections))
//            {
//                const osg::NodePath& nodePath = intersections.begin()->nodePath;
//                for(osg::NodePath::const_iterator nitr=nodePath.begin();
//                     nitr!=nodePath.end();
//                     ++nitr)
//                {
//                    auto hitNode = dynamic_cast<osg::Node*>(*nitr);
//                    if (hitNode == node)
//                    {
//                        visible = true;
//                    }
//                }
//            }
//            mQmlNode->setVisible(visible);
        }
    }
    traverse(node, nv);
}

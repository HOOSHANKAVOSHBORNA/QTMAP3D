#include "autoScaler.h"
#include <osg/Switch>
#include <osgEarthAnnotation/PlaceNode>
#include <QDebug>

void AutoScaler::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    if (_enabled) {
        if(_scaled){
//            osg::PositionAttitudeTransform* pATransform;
            osgEarth::Annotation::GeoPositionNode* pATransform = static_cast<osgEarth::Annotation::GeoPositionNode*>(node);
//            osgEarth::Annotation::GeoPositionNode* geo = dynamic_cast<osgEarth::Annotation::GeoPositionNode*>(node);
//            if(geo)
//                pATransform = geo->getPositionAttitudeTransform();
//            else
//                pATransform = dynamic_cast<osg::PositionAttitudeTransform*>(node);

//            pATransform->setScale(_baseScale);
            osgUtil::CullVisitor* cs = static_cast<osgUtil::CullVisitor*>(nv);

            osg::Camera* cam = cs->getCurrentCamera();

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
                // Reset the scale so we get a proper bound
                pATransform->setScale(_baseScale);
                const osg::BoundingSphere& bs = node->getBound();

                // transform centroid to VIEW space:
                osg::Vec3d centerView = bs.center() * cam->getViewMatrix();

                // Set X coordinate to the radius so we can use the resulting CLIP
                // distance to calculate meters per pixel:
                centerView.x() = bs.radius();

                // transform the CLIP space:
                osg::Vec3d centerClip = centerView * cam->getProjectionMatrix();

                // caluclate meters per pixel:
                double mpp = (centerClip.x()*1.7) * cam->getViewport()->width();

                // and the resulting scale we need to auto-scale.
                double scale = bs.radius() / mpp;

                scale *= _defaultScale;

                if (scale < _minScale)
                    scale = _minScale;
                else if (scale>(_maxScale*_defaultScale))
                    scale = (_maxScale*_defaultScale);

                pATransform->setScale(
                    osg::componentMultiply(_baseScale, osg::Vec3d(scale, scale, scale)));
//                qDebug()<<"scale: "<<scale;
            }

            if (node->getCullingActive() == false)
            {
                node->setCullingActive(true);
            }
        }
        traverse(node, nv);
    }
}

void AutoScaler::setDefaultScale(double defaultScale)
{
    _defaultScale = defaultScale;
}

void AutoScaler::setMinScale(double minScale)
{
    _minScale = minScale;
}

void AutoScaler::setMaxScale(double maxScale)
{
    _maxScale = maxScale;
}

void AutoScaler::setEnabled(bool enabled)
{
    _enabled = enabled;
}

void AutoScaler::setScaled(bool newScaled)
{
    _scaled = newScaled;
}

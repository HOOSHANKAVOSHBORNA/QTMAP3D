#include "defenseModelNodeAutoScaler.h"


void DefenseModelNodeAutoScaler::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    if (_enabled) {
        osgEarth::Annotation::GeoPositionNode* geo = static_cast<osgEarth::Annotation::GeoPositionNode*>(node);
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
            geo->getPositionAttitudeTransform()->setScale(_baseScale);
            const osg::BoundingSphere& bs = node->getBound();

            // transform centroid to VIEW space:
            osg::Vec3d centerView = bs.center() * cam->getViewMatrix();

            // Set X coordinate to the radius so we can use the resulting CLIP
            // distance to calculate meters per pixel:
            centerView.x() = bs.radius();

            // transform the CLIP space:
            osg::Vec3d centerClip = centerView * cam->getProjectionMatrix();

            // caluclate meters per pixel:
            double mpp = (centerClip.x()*0.5) * cam->getViewport()->width();

            // and the resulting scale we need to auto-scale.
            double scale = bs.radius() / mpp;

            scale *= _defaultScale;

            if (scale < _minScale)
                scale = _minScale;
            else if (scale>(_maxScale*_defaultScale))
                scale = (_maxScale*_defaultScale);

            geo->getPositionAttitudeTransform()->setScale(
                        osg::componentMultiply(_baseScale, osg::Vec3d(scale, scale, scale)));
        }

        if (node->getCullingActive() == false)
        {
            node->setCullingActive(true);
        }

        traverse(node, nv);
    }
}

void DefenseModelNodeAutoScaler::setDefaultScale(double defaultScale)
{
    _defaultScale = defaultScale;
}

void DefenseModelNodeAutoScaler::setMinScale(double minScale)
{
    _minScale = minScale;
}

void DefenseModelNodeAutoScaler::setMaxScale(double maxScale)
{
    _maxScale = maxScale;
}

void DefenseModelNodeAutoScaler::setEnabled(bool enabled)
{
    _enabled = enabled;
}


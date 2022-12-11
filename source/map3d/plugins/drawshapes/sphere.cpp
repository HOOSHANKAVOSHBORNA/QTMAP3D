#include "sphere.h"
#include <osgEarth/GLUtils>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osgEarthAnnotation/ModelNode>
#include "mapcontroller.h"
#include <osg/ClipNode>
#include <osg/ClipPlane>

Sphere::Sphere(MapController *mapController, float radius, bool clamp)
{
   mMapController = mapController;
   osg::Vec3 eye(0.f,0.f,0.f);

   pSphereShape = new osg::Sphere(eye, radius);
   pShapeDrawable = new osg::ShapeDrawable(pSphereShape.get());
   pShapeDrawable->setColor(osg::Vec4(0.0, 0.5f, 1.0, 0.3f));

   if (clamp == true){
       style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
   }
   else{
       style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;

   }
   style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
   style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(pShapeDrawable);

   model = new osgEarth::Annotation::ModelNode(mMapController->getMapNode(), style);

   osg::ClipPlane* clipplane = new osg::ClipPlane();
   clipplane->setClipPlane(0, 1, 0, 1);

   this->addClipPlane(clipplane);
   this->addChild(model);
}


void Sphere::setProp(osg::Vec4 color, osg::Vec3 center, float radius)
{
    osg::ref_ptr<osg::Sphere> pSphereShapeN = new osg::Sphere(center, radius);
    pShapeDrawable->setShape(pSphereShapeN);
    pShapeDrawable->setColor(color);

}

void Sphere::setColor(osg::Vec4 color)
{
    pShapeDrawable->setColor(color);
}

void Sphere::setCenter(osg::Vec3 center)
{
    osg::ref_ptr<osg::Sphere> pSphereShapeN = new osg::Sphere(center, pSphereShape->getRadius());
    pShapeDrawable->setShape(pSphereShapeN);
}

void Sphere::setRadius(float radius)
{
    osg::ref_ptr<osg::Sphere> pSphereShapeN = new osg::Sphere(pSphereShape->getCenter(), radius);
    pShapeDrawable->setShape(pSphereShapeN);

}

void Sphere::setClamp(bool clamp)
{
    if (clamp == true){
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    }
    model->setStyle(style);
}





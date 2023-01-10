#include "sphere.h"
#include <osgEarth/GLUtils>
#include "mapcontroller.h"


Sphere::Sphere(MapController *mapController, float radius, bool clamp)
{
   mMapController = mapController;
   osg::Vec3 eye(0.f,0.f,0.f);
   this->setRadius(radius);
   this->setCenter(eye);
   //pSphereShape = new osg::Sphere(eye,radius);
   pShapeDrawable = new osg::ShapeDrawable(this);
   pShapeDrawable->setColor(osg::Vec4(0.0, 0.5f, 1.0, 0.3f));

   if (clamp == true){
       style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
   }
   else{
       style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;

   }
   style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
   style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(pShapeDrawable);


   model = new osgEarth::Annotation::ModelNode(mMapController->getMapNode(), style);
   model->addChild(this->asNode());

//   osg::ClipPlane* clipplane = new osg::ClipPlane();
//   clipplane->setClipPlane(0, 1, 0, 1);

//   this->addClipPlane(clipplane);
//   this->addChild(model);

}



void Sphere::setColor(osg::Vec4 color)
{
    pShapeDrawable->setColor(color);
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





#include "cone.h"
#include <osgEarth/GLUtils>
#include "mapcontroller.h"
Cone::Cone(MapController *mapController, float radius, float height, bool clamp)
{
    mMapController = mapController;
    osg::Vec3 eye(0.f,0.f,0.f);

    pConeShape = new osg::Cone(eye, radius, height);
    pShapeDrawable = new osg::ShapeDrawable(pConeShape.get());
    pShapeDrawable->setColor(osg::Vec4(0.6f, 0.0f, 1.0f, 0.3f));

    if (clamp == true){
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    }
    else{
        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_NONE;

    }
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(pShapeDrawable);


    model = new osgEarth::Annotation::ModelNode(mMapController->getMapNode(), style);

    osg::ClipPlane* clipplane = new osg::ClipPlane();
    clipplane->setClipPlane(0, 1, 0, 1);

    this->addClipPlane(clipplane);
    this->addChild(model);
}

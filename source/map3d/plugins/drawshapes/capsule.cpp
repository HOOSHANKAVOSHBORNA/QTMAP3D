#include "capsule.h"
#include <osgEarth/GLUtils>
#include "cylinder.h"
#include "mapcontroller.h"

Capsule::Capsule(MapController *mapController, float radius, float height, bool clamp)
{
    mMapController = mapController;
    osg::Vec3 eye(0.f,0.f,radius + height);

    pCapsuleShape = new osg::Capsule(eye, radius, height);
    osg::TessellationHints* tes = new  osg::TessellationHints() ;
    tes->setCreateBottom(false);

    auto pCylinderShape = new osg::Cylinder(eye, radius, 0);
    auto com = new osg::CompositeShape();
    com->addChild(pCapsuleShape);
    com->addChild(pCylinderShape);

    pShapeDrawable = new osg::ShapeDrawable(com, tes);
    pShapeDrawable->setColor(osg::Vec4(0.6f, 0.2f, 1.0f, 0.3f));

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

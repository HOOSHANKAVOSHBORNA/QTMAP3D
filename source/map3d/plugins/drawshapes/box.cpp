#include "box.h"
#include <osgEarth/GLUtils>
#include "mapcontroller.h"

Box::Box(MapController *mapController, float length, float width, float height, bool clamp)
{
    mMapController = mapController;
    osg::Vec3 eye(0.f,0.f,0.f);

    pBoxShape = new osg::Box(eye, length, width, height);
    pShapeDrawable = new osg::ShapeDrawable(pBoxShape.get());
    pShapeDrawable->setColor(osg::Vec4(1.0f, 0.2f, 1.0f, 0.3f));

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

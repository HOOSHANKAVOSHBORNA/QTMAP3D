
#include "truckl.h"
#include "mapcontroller.h"

TruckL::TruckL(MapController *mapController) :
    osgEarth::Annotation::ModelNode(mapController->getMapNode(), osgEarth::Symbology::Style())
{

}

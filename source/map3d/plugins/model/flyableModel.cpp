#include "flyableModel.h"

flyableModel::flyableModel(MapItem *mapControler, const std::string &modelUrl, QObject *parent):
    moveableModel(mapControler, modelUrl, parent)
{

}

void flyableModel::flyTo(osgEarth::GeoPoint destinationPoint, double velocity)
{

}

double flyableModel::speed() const
{

}

void flyableModel::setSpeed(double newSpeed)
{

}

#include "flyableModel.h"

flyableModel::flyableModel(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent):
    MoveableModel(mapControler, modelUrl, iconUrl, parent)
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

simpleModelNode *flyableModel::getNewModel()
{
    return new flyableModel(mapItem(), modelUrl(), iconUrl());
}

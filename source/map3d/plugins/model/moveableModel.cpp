#include "moveableModel.h"
#include "mapItem.h"
#include <QObject>

moveableModel::moveableModel(MapItem *mapControler, const std::string &modelUrl, QObject *parent):
    simpleModelNode(mapControler, modelUrl, parent)

{

}

void moveableModel::moveTo(osgEarth::GeoPoint destinationPoint, double mSpeed)
{

}

simpleModelNode *moveableModel::getNewModel()
{
    return new moveableModel(mMapItem, mModelUrl);
}

double moveableModel::speed() const
{
    return mSpeed;
}

void moveableModel::setSpeed(double newSpeed)
{
    mSpeed = newSpeed;
}

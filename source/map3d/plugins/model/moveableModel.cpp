#include "moveableModel.h"
#include "mapItem.h"
#include <QObject>

moveableModel::moveableModel(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent):
    simpleModelNode(mapControler, modelUrl, iconUrl, parent)

{

}

void moveableModel::moveTo(osgEarth::GeoPoint destinationPoint, double mSpeed)
{

}

simpleModelNode *moveableModel::getNewModel()
{
    return new moveableModel(mMapItem, mModelUrl, mIconUrl);
}

double moveableModel::speed() const
{
    return mSpeed;
}

void moveableModel::setSpeed(double newSpeed)
{
    mSpeed = newSpeed;
}

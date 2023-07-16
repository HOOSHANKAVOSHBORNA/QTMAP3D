#ifndef FLYABLEMODEL_H
#define FLYABLEMODEL_H
#include "moveableModel.h"

class flyableModel: public moveableModel
{
public:
    flyableModel(MapItem *mapControler, const std::string &modelUrl, QObject *parent);
    void flyTo(osgEarth::GeoPoint destinationPoint, double velocity);
    double speed() const override;
    void setSpeed(double newSpeed) override;

private:
};

#endif // FLYABLEMODEL_H

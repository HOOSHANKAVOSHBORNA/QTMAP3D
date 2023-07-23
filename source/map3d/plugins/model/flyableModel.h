#ifndef FLYABLEMODEL_H
#define FLYABLEMODEL_H
#include "moveableModel.h"

class flyableModel: public moveableModel
{
public:
    flyableModel(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent = nullptr);
    void flyTo(osgEarth::GeoPoint destinationPoint, double velocity);
    double speed() const override;
    void setSpeed(double newSpeed) override;
    virtual simpleModelNode *getNewModel() override;
private:
protected:

};

#endif // FLYABLEMODEL_H

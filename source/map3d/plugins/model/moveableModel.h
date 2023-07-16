#ifndef MOVEABLEMODEL_H
#define MOVEABLEMODEL_H
#include "simpleModelNode.h"

class moveableModel : public simpleModelNode
{
public:
    explicit moveableModel(MapItem* mapControler, const std::string& modelUrl, QObject *parent = nullptr);
    void moveTo(osgEarth::GeoPoint destinationPoint, double mSpeed);
    virtual simpleModelNode* getNewModel() override;
    virtual double speed() const;
    virtual void setSpeed(double newSpeed);

private:
    double mSpeed;
};

#endif // MOVEABLEMODEL_H

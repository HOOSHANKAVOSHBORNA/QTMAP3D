#ifndef PARTICLETEST_H
#define PARTICLETEST_H

#include "serviceManager.h"
#include "qobject.h"


class ParticleTest : public QObject
{
   Q_OBJECT
public:
   ParticleTest(ServiceManager *serviceManager);

private:
   void createExplosion(ExplosionData data);

private:
   ServiceManager *mServiceManager;
   QVector<ExplosionData> mParticleDataList;
   int  mCount{0};
};

#endif // PARTICLETEST_H

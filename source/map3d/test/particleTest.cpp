#include "particleTest.h"
#include <qtimer.h>
#include <qrandom.h>

ParticleTest::ParticleTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    //--create and update aircraft info------------------------
    QTimer *timerUpdateAircraft = new QTimer();
    QObject::connect(timerUpdateAircraft, &QTimer::timeout, [this](){
        ExplosionData eData;
        double longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
        double latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
        double duration = (QRandomGenerator::global()->generate() % (20));
        double scale = 3 + (QRandomGenerator::global()->generate() % (7));
        eData.id = 400 + mCount;
        eData.name = &"Explosion" [ eData.id];
        eData.layerId = 402;
        eData.latitude  = latitude;
        eData.longitude = longitude;
        eData.altitude = 0;
        eData.duration = duration;
        eData.scale = scale;
        eData.command = Command::Add;
        createExplosion(eData);
        mCount++;
    });
    timerUpdateAircraft->start(5000);
}

void ParticleTest::createExplosion(ExplosionData data)
{
    mParticleDataList.append(data);
    mServiceManager->sendExplosion(data);
}

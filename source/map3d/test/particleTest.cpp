//#include "particleTest.h"
//#include "qjsondocument.h"
//#include "qtimer.h"
//#include <qrandom.h>

//ParticleTest::ParticleTest(NetworkManager *networkManager):
//    mNetworkManager(networkManager)
//{
//    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]{
//        //--create and update aircraft info------------------------
//        QTimer *timerUpdateAircraft = new QTimer();
//        QObject::connect(timerUpdateAircraft, &QTimer::timeout, [this](){
//            ExplosionData eData;
//            double longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
//            double latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
//            double duration = (QRandomGenerator::global()->generate() % (20));
//            double scale = 3 + (QRandomGenerator::global()->generate() % (7));
//            eData.latitude  = latitude;
//            eData.longitude = longitude;
//            eData.duration = duration;
//            eData.scale = scale;
//            createExplosion(eData);
//        });
//        timerUpdateAircraft->start(5000);
//    });
//}

//void ParticleTest::createExplosion(ExplosionData data)
//{
//    mParticleDataList.append(data);
//    QJsonObject obj;
//    obj.insert("Type","Particle");
//    obj.insert("COMMAND","ADD");
//    obj.insert("latitude",data.latitude);
//    obj.insert("longitude",data.longitude);
//    obj.insert("duration",data.duration);
//    obj.insert("scale",data.scale);
//    QJsonDocument doc;
//    doc.setObject(obj);
//    mNetworkManager->sendData(doc.toJson(QJsonDocument::Indented));
//}

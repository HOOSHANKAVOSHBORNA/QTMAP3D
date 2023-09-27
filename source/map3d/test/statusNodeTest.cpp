#include "statusNodeTest.h"
#include <QJsonDocument>
#include <QTimer>

StatusNodeTest::StatusNodeTest(NetworkManager *networkManager):
    mNetworkManager(networkManager)
{
    QObject::connect(mNetworkManager, &NetworkManager::flyableQueueDeclared, [this]{
        QTimer *timerUpdateAircraft = new QTimer();
        QObject::connect(timerUpdateAircraft, &QTimer::timeout, [this](){
                for(auto& jsonDocument: mStatusDataList)
                    mNetworkManager->sendStatusData(jsonDocument.toJson(QJsonDocument::Compact));
        });
        timerUpdateAircraft->start(1000);
    });
}

void StatusNodeTest::addOrUpdateStatusInfo(int id, QJsonDocument doc)
{
    mStatusDataList[id] = doc;
}

void StatusNodeTest::deleteStatusInfo(int id)
{
    mStatusDataList.remove(id);
}

#ifndef MODELNODETEST_H
#define MODELNODETEST_H

#include "networkManager.h"
#include <QJsonDocument>

struct FlayableData
{
    QJsonDocument flyableDoc;
    QJsonDocument statusDoc;
    QJsonDocument lineDoc;
};
class FlyableNodeTest
{
public:
    FlyableNodeTest(NetworkManager *networkManager);

private:
    void createFlyableInfo();
    void updateFlyableInfo();

private:
    NetworkManager *mNetworkManager;
    bool mFlyableQueueDeclared{false};

    QVector<FlayableData> mFlyableDataList;
    int mMaxFlyableNumber{5};
};

#endif // MODELNODETEST_H

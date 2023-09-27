#ifndef MODELNODETEST_H
#define MODELNODETEST_H

#include "networkManager.h"
#include "statusNodeTest.h"

#include <QJsonDocument>

class ModelNodeTest
{
public:
    ModelNodeTest(NetworkManager *networkManager, StatusNodeTest *statusNodeTest);

private:
    void createFlyableInfo();
    void updateFlyableInfo();

private:
    NetworkManager *mNetworkManager;
    StatusNodeTest *mStatusNodeTest;
    bool mFlyableQueueDeclared{false};

    QVector<QJsonDocument> mFlyableDataList;
    int mMaxFlyableNumber{5};
};

#endif // MODELNODETEST_H

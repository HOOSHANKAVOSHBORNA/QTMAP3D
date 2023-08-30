#ifndef MODELNODETEST_H
#define MODELNODETEST_H

#include "networkManager.h"

#include <QJsonDocument>

class ModelNodeTest
{
public:
    ModelNodeTest(NetworkManager *networkManager);

private:
    void createFlyableInfo();
    void updateFlyableInfo();

private:
    NetworkManager *mNetworkManager;
    bool mFlyableQueueDeclared{false};

    QVector<QJsonDocument> mFlyableDataList;
    int mMaxFlyableNumber{10};
};

#endif // MODELNODETEST_H

#ifndef MOVABLENODETEST_H
#define MOVABLENODETEST_H

#include "networkManager.h"
#include <QJsonDocument>

struct MovableData
{
    QJsonDocument movableDoc;
    QJsonDocument statusDoc;
    QJsonDocument lineDoc;
};

class MovableNodeTest
{
public:
    MovableNodeTest(NetworkManager *networkManager);

private:
    void createMovableInfo();
    void updateMovableInfo();

private:
    NetworkManager *mNetworkManager;
    QVector<MovableData> mMovableDataList;
    int mMaxMovableNumber{5};
    bool mMovableQueueDeclared{false};
};

#endif // MOVABLENODETEST_H

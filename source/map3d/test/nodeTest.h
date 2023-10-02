#ifndef NODETEST_H
#define NODETEST_H

#include "networkManager.h"
#include <QJsonDocument>

struct NodeData
{
    QJsonDocument nodeDoc;
    QJsonDocument statusDoc;
    QJsonDocument circleDoc;
};

class NodeTest
{
public:
    NodeTest(NetworkManager *networkManager);

private:
    void createInfo();
    void updateInfo();

private:
    NetworkManager *mNetworkManager;
    QVector<NodeData> mNodeDataList;
    int mMaxNumber{5};
};

#endif // NODETEST_H

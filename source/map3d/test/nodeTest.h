#ifndef NODETEST_H
#define NODETEST_H

#include "serviceManager.h"
#include <QJsonDocument>

struct NodeInfo
{
    NodeData nodeData;
    StatusNodeData statusNodeData;
    CircleData circleData;
    PolygonData polygonData;
};

class NodeTest
{
public:
    NodeTest(ServiceManager *serviceManager);

private:
    void createInfo();
    void updateInfo();

private:
    ServiceManager *mServiceManager;
    QVector<NodeInfo> mNodeInfoList;
    int mMaxNumber{20};
    int mCount{0};
};

#endif // NODETEST_H

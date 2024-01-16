#ifndef NODETEST_H
#define NODETEST_H

#include "serviceManager.h"
#include <QJsonDocument>

struct NodeInfo
{
    NodeData nodeData{0};
    StatusNodeData statusNodeData{0};
    CircleData circleData{0};
    PolygonData polygonData{0};
    PolyLineData polyLineData{0};
};

class NodeTest
{
public:
    NodeTest(ServiceManager *serviceManager);

private:
    void createInfo();
    void updateInfo();
    void removeInfo();

private:
    ServiceManager *mServiceManager;
    QVector<NodeInfo> mNodeInfoList;
    int mMaxNumber{30};
    int mDeleteNode{0};
    int mCount{0};
    int mRemoveRatio{5};
};

#endif // NODETEST_H

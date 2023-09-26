#ifndef STATUSNODETEST_H
#define STATUSNODETEST_H

#include "networkManager.h"

class StatusNodeTest
{
public:
    StatusNodeTest(NetworkManager *networkManager);

public:
    void addOrUpdateStatusInfo(int id, QJsonDocument doc);
    void deleteStatusInfo(int id);

private:
    NetworkManager *mNetworkManager;

    QMap<int, QJsonDocument> mStatusDataList;
};

#endif // STATUSNODETEST_H

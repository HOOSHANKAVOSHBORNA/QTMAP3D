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
struct Url
{
    QString Url2d;
    QString Url3d;
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

    QVector<FlayableData> mFlyableDataList;
    int mMaxFlyableNumber{10};

    QVector<Url> mUrlList;
};

#endif // MODELNODETEST_H

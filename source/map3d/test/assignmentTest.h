#ifndef ASSIGNMENTTEST_H
#define ASSIGNMENTTEST_H

#include "networkManager.h"
#include <QJsonDocument>

struct AssignmentData
{
    QJsonDocument movableDoc;
    QJsonDocument statusDoc;
    QJsonDocument lineDoc;
};


class AssignmentTest
{
public:
    AssignmentTest(NetworkManager *networkManager);

private:
    void createAssignment();
    void updateAssignment();

private:
    NetworkManager *mNetworkManager;
    QVector<AssignmentData> mAssignmentDataList;
    int  mMaxAssignNumber{5};
    bool mAssignmentQueueDeclared{false};
    int  mCount{0};
};

#endif // ASSIGNMENTTEST_H

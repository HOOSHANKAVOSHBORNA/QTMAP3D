#ifndef ASSIGNMENTTEST_H
#define ASSIGNMENTTEST_H

#include "networkManager.h"
#include <QJsonDocument>
#include <QJsonObject>

#define stringify( name ) #name


struct AssignmentData
{
    int attackerID;
    int targetID;
    std::string state;
};


class AssignmentTest : public QObject
{
    Q_OBJECT
public:
    AssignmentTest(NetworkManager *networkManager);

private:
    bool randomBool();
    void createAssignment(AssignmentData data);
    void updateAssignment(AssignmentData data);
    void removeAssignment(AssignmentData data);

public slots:
    void dataReceived(QJsonObject obj);

private:
    NetworkManager *mNetworkManager;
    QVector<AssignmentData> mAssignmentDataList;
    bool mAssignmentQueueDeclared{false};
    int  mCount{0};
};

#endif // ASSIGNMENTTEST_H
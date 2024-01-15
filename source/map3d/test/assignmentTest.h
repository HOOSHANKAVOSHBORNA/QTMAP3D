#ifndef ASSIGNMENTTEST_H
#define ASSIGNMENTTEST_H

#include "serviceManager.h"
#include <QJsonDocument>
#include <QJsonObject>


class AssignmentTest : public QObject
{
   Q_OBJECT
public:
   AssignmentTest(ServiceManager *serviceManager);

private:
   void createAssignment(AssignmentData data);
   void updateAssignment(AssignmentData data);
   void removeAssignment(AssignmentData data);

public slots:
   void dataReceived(AssignmentData assignmentData);

private:
   ServiceManager *mServiceManager;
   QVector<AssignmentData> mAssignmentDataList;
   QVector<NodeData> mBulletDataList;
   QVector<QString> mStates{"Assigned", "Search", "Lock", "Fire", "Succeed", "Failed"};
   int mCount{1000};
};

#endif // ASSIGNMENTTEST_H

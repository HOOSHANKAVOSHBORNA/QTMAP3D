#include "assignmentTest.h"

#include <QTimer>
#include <QRandomGenerator>
#include <QJsonObject>

AssignmentTest::AssignmentTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    QObject::connect(mServiceManager,&ServiceManager::assignmentDataReceived, this, &AssignmentTest::dataReceived);
    //--create and update assignment info------------------------
    QTimer *timerUpdateAssign = new QTimer();
    QObject::connect(timerUpdateAssign, &QTimer::timeout, [this](){
        for(auto& AssignmentData: mAssignmentDataList){
            AssignmentData.state = QString::fromStdString(mStates.at(QRandomGenerator::global()->generate() % (6)));
            updateAssignment(AssignmentData);
        }
    });
    timerUpdateAssign->start(5000);

}
void AssignmentTest::createAssignment(AssignmentData data)
{
    if(QRandomGenerator::global()->generate() % (2) -1){
        mAssignmentDataList.append(data);
        data.command = "ADDORUPDATE";
        data.layerId = 401;
        mServiceManager->sendAssignment(data);
    }
}

void AssignmentTest::updateAssignment(AssignmentData data)
{
    data.command = "ADDORUPDATE";
    mServiceManager->sendAssignment(data);
}

void AssignmentTest::removeAssignment(AssignmentData data)
{
    if(QRandomGenerator::global()->generate() % (2) -1){
        // mAssignmentDataList.removeOne(data);
        data.command = "REMOVE";
        mServiceManager->sendAssignment(data);
    }
}

void AssignmentTest::dataReceived(AssignmentData data)
{
    if(data.command == "ASSIGNREQUEST"){
        createAssignment(data);
    }
    if(data.command == "CANCELREQUEST"){
        removeAssignment(data);
    }
}

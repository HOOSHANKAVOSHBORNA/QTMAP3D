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
            if(QRandomGenerator::global()->generate() % (2) -1){
                // if(mStates.indexOf(AssignmentData.state) >= 5){mAssignmentDataList.removeOne(AssignmentData);}
                // AssignmentData.state = QString::fromStdString(mStates.at(mStates.indexOf(AssignmentData.state) + 1));
                AssignmentData.state = QString::fromStdString(mStates.at(QRandomGenerator::global()->generate() % 6));
                updateAssignment(AssignmentData);
            }
        }
    });
    timerUpdateAssign->start(4000);

}
void AssignmentTest::createAssignment(AssignmentData data)
{
    if(QRandomGenerator::global()->generate() % (2) -1){
        mAssignmentDataList.append(data);
        data.command = Command::Add;
        data.layerId = 401;
        data.state = "Assigned";
        mServiceManager->sendAssignment(data);
    }
}

void AssignmentTest::updateAssignment(AssignmentData data)
{
    data.command = Command::Update;
    mServiceManager->sendAssignment(data);
}

void AssignmentTest::removeAssignment(AssignmentData data)
{
    if(QRandomGenerator::global()->generate() % (2) -1){
        // mAssignmentDataList.removeOne(data);
        data.command = Command::Remove;
        mServiceManager->sendAssignment(data);
    }
}

void AssignmentTest::dataReceived(AssignmentData assignmentData)
{
    if(assignmentData.command == Command::Add){
        createAssignment(assignmentData);
    }
    if(assignmentData.command == Command::Remove){
        removeAssignment(assignmentData);
    }
}

#include "assignmentTest.h"

#include <QColor>
#include <QTimer>
#include <QRandomGenerator>
#include <QJsonObject>
#include <QJsonArray>

AssignmentTest::AssignmentTest(NetworkManager *networkManager):
    mNetworkManager(networkManager)
{
    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]{
        mAssignmentQueueDeclared = true;

        //--create and update assignment info------------------------
        QTimer *timerUpdateAssign = new QTimer();
        QObject::connect(timerUpdateAssign, &QTimer::timeout, [this](){
            createAssignment();
            updateAssignment();
            if(mAssignmentQueueDeclared)
                for(auto& AssignmentData: mAssignmentDataList){
                    mNetworkManager->sendData(AssignmentData.movableDoc.toJson(QJsonDocument::Compact));
                    mNetworkManager->sendData(AssignmentData.statusDoc.toJson(QJsonDocument::Compact));
                    mNetworkManager->sendData(AssignmentData.lineDoc.toJson(QJsonDocument::Compact));
                }
        });
        timerUpdateAssign->start(1000);
    });
}
void AssignmentTest::createAssignment()
{

}

void AssignmentTest::updateAssignment()
{

}

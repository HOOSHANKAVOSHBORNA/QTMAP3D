#include "assignmentTest.h"

#include <QColor>
#include <QTimer>
#include <QRandomGenerator>
#include <QJsonObject>
#include <QJsonArray>

AssignmentTest::AssignmentTest(NetworkManager *networkManager):
    mNetworkManager(networkManager)
{
    QObject::connect(mNetworkManager,&NetworkManager::assignDataReceived, this, &AssignmentTest::dataReceived);
    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]{
        mAssignmentQueueDeclared = true;

        //--create and update assignment info------------------------
        QTimer *timerUpdateAssign = new QTimer();
        QObject::connect(timerUpdateAssign, &QTimer::timeout, [this](){

            if(mAssignmentQueueDeclared)
                for(auto& AssignmentData: mAssignmentDataList){
                    // mNetworkManager->sendData(AssignmentData.movableDoc.toJson(QJsonDocument::Compact));

                }
        });
        timerUpdateAssign->start(1000);
    });
}
void AssignmentTest::createAssignment(AssignmentData data)
{
    if(randomBool()){
        mAssignmentDataList.append(data);
        QJsonObject obj;
        obj.insert("Type","Assign");
        obj.insert("COMMAND","ADDORUPDATE");
        obj.insert("attackerID",QString::fromStdString(data.attackerID));
        obj.insert("targetID",QString::fromStdString(data.targetID));
        obj.insert("state","ASSIGNED");
        QJsonDocument doc;
        doc.setObject(obj);
        mNetworkManager->sendData(doc.toJson(QJsonDocument::Indented));
    }
}

void AssignmentTest::updateAssignment(AssignmentData data)
{

}

bool AssignmentTest::randomBool()
{
    static auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
    return gen();
}


void AssignmentTest::removeAssignment(AssignmentData data)
{
    if(randomBool()){
        // mAssignmentDataList.removeOne(data);
        QJsonObject obj;
        obj.insert("Type","Assign");
        obj.insert("COMMAND","REMOVE");
        obj.insert("attackerID",QString::fromStdString(data.attackerID));
        obj.insert("targetID",QString::fromStdString(data.targetID));
        QJsonDocument doc;
        doc.setObject(obj);
        mNetworkManager->sendData(doc.toJson(QJsonDocument::Indented));
    }
}

void AssignmentTest::dataReceived(QJsonObject obj)
{
    AssignmentData data;
    data.attackerID = obj.value("attackerID").toString().toStdString();
    data.targetID = obj.value("targetID").toString().toStdString();
    if(obj.value("COMMAND").toString() == "ASSIGNREQUEST"){
        createAssignment(data);
    }
    if(obj.value("COMMAND").toString() == "CANCELREQUEST"){
        removeAssignment(data);
    }
}

#include "assignmentTest.h"
#include "nodeTest.h"

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
        for(auto& bulletData: mBulletDataList){
            bulletData.longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
            bulletData.latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
            bulletData.altitude = 2000 + (QRandomGenerator::global()->generate() % (9000 - 2000));
            bulletData.speed = 100000;
            mServiceManager->sendNode(bulletData);
        }
        for(auto& AssignmentData: mAssignmentDataList){
            if(QRandomGenerator::global()->generate() % (2) -1){
                if(mStates.indexOf(AssignmentData.state) > 3){
                    if(QRandomGenerator::global()->generate() % (2) -1){
                        AssignmentData.state = mStates[4];
                        updateAssignment(AssignmentData);
                        //---bullet-----------------------------------------------
                        double longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
                        double latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
                        double altitude = 2000 + (QRandomGenerator::global()->generate() % (9000 - 2000));
                        NodeData nodData;
                        nodData.id = AssignmentData.attackerId + mCount;
                        nodData.name = "bullet" + QString::number(nodData.id);
                        nodData.type = NodeType::Flyable;
                        nodData.category = "Bullet";
                        nodData.command = Command::Add;
                        nodData.iconInfoUrl =  "../data/models/missile/missile.png";
                        nodData.imgInfoUrl = "qrc:/Resources/airplane1.jpg";
                        nodData.url2D = "../data/models/missile/missile.png";
                        nodData.url3D = "../data/models/missile/missile.osgb";
                        nodData.color = "red";
                        nodData.isAttacker = false;
                        nodData.latitude = latitude;
                        nodData.longitude = longitude;
                        nodData.altitude = altitude;
                        nodData.layersId.push_back(401);
                        mServiceManager->sendNode(nodData);
                        mBulletDataList.append(nodData);
                        mAssignmentDataList.removeOne(AssignmentData);
                        mCount++;
                        //--end bullet ------------------------------------
                    }else{
                        AssignmentData.state = mStates[5];
                        updateAssignment(AssignmentData);
                        mAssignmentDataList.removeOne(AssignmentData);
                    }
                }else{
                    AssignmentData.state = mStates.at(mStates.indexOf(AssignmentData.state) + 1);
                    updateAssignment(AssignmentData);
                }
            }
        }
    });
    timerUpdateAssign->start(4000);

}
void AssignmentTest::createAssignment(AssignmentData data)
{
    if(QRandomGenerator::global()->generate() % (100) < 70){
        mAssignmentDataList.append(data);
        data.command = Command::Add;
        data.layerId = 401;
        data.state = "Assigned";
        mServiceManager->sendAssignment(data);
    }else{
        data.command = Command::Remove;
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
        mAssignmentDataList.removeOne(data);
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

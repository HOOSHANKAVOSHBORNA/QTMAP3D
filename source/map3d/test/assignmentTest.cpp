//#include "assignmentTest.h"

//#include <QColor>
//#include <QTimer>
//#include <QRandomGenerator>
//#include <QJsonObject>
//#include <QJsonArray>

//AssignmentTest::AssignmentTest(NetworkManager *networkManager):
//    mNetworkManager(networkManager)
//{
//    QObject::connect(mNetworkManager,&NetworkManager::assignDataReceived, this, &AssignmentTest::dataReceived);
//    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]{

//        //--create and update assignment info------------------------
//        QTimer *timerUpdateAssign = new QTimer();
//        QObject::connect(timerUpdateAssign, &QTimer::timeout, [this](){

//            for(auto& AssignmentData: mAssignmentDataList){
//                if(randomBool()){
//                    if(randomBool()){
//                        AssignmentData.state = "SEARCH";
//                    }else{
//                        AssignmentData.state = "FIRE";
//                    }
//                }else{
//                    if(randomBool()){
//                        AssignmentData.state = "LOCK";
//                    }else{
//                        AssignmentData.state = "ASSIGNED";
//                    }
//                }
//                QJsonObject obj;
//                obj.insert("Type","Assign");
//                obj.insert("COMMAND","ADDORUPDATE");
//                obj.insert("attackerID",AssignmentData.attackerID);
//                obj.insert("targetID",AssignmentData.targetID);
//                obj.insert("state", AssignmentData.state.data());
//                QJsonDocument doc;
//                doc.setObject(obj);
//                mNetworkManager->sendData(doc.toJson(QJsonDocument::Indented));
//            }
//        });
//        timerUpdateAssign->start(1000);
//    });
//}
//void AssignmentTest::createAssignment(QJsonDocument data)
//{
//    if(randomBool()){
//        mAssignmentDataList.append(data);
//        QJsonObject obj;
//        obj.insert("Type","Assign");
//        obj.insert("COMMAND","ADDORUPDATE");
//        obj.insert("attackerID",data.attackerID);
//        obj.insert("targetID",data.targetID);
//        obj.insert("state","ASSIGNED");
//        QJsonDocument doc;
//        doc.setObject(obj);
//        mNetworkManager->sendData(doc.toJson(QJsonDocument::Indented));
//    }
//}

//void AssignmentTest::updateAssignment(QJsonDocument data)
//{

//}

//void AssignmentTest::removeAssignment(QJsonDocument data)
//{
//    if(randomBool()){
//        // mAssignmentDataList.removeOne(data);
//        QJsonObject obj;
//        obj.insert("Type","Assign");
//        obj.insert("COMMAND","REMOVE");
//        obj.insert("attackerID",data.attackerID);
//        obj.insert("targetID",data.targetID);
//        QJsonDocument doc;
//        doc.setObject(obj);
//        mNetworkManager->sendData(doc.toJson(QJsonDocument::Indented));
//    }
//}

//void AssignmentTest::dataReceived(QJsonObject obj)
//{
//    AssignmentData data;
//    data.attackerID = obj.value("attackerID").toInt();
//    data.targetID = obj.value("targetID").toInt();
//    if(obj.value("COMMAND").toString() == "ASSIGNREQUEST"){
//        createAssignment(data);
//    }
//    if(obj.value("COMMAND").toString() == "CANCELREQUEST"){
//        removeAssignment(data);
//    }
//}

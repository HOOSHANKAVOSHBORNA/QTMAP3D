#include <QCoreApplication>
#include "assignmentTest.h"
#include "layerTest.h"
#include "networkManager.h"
#include "flyableNodeTest.h"
#include "movableNodeTest.h"
#include "nodeTest.h"
#include "serviceManager.h"
#include "particleTest.h"
#include "userTest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NetworkManager networkManager;
    networkManager.start();
    ServiceManager serviceManager(&networkManager);
    //    QObject::connect(&networkManager, &NetworkManager::flyableQueueDeclared,[&networkManager]{
    //        networkManager.sendFlyableData("data");
    //    });
    UserTest *userTest = new UserTest(&serviceManager);
    LayerTest *layerTest;
    NodeTest *nodeTest;
    AssignmentTest *assignmentTest;
    ParticleTest *particleTest;

    QObject::connect(userTest, &UserTest::login,[&]{
        qDebug()<<"login";
//        layerTest = new LayerTest(&serviceManager);
//        nodeTest = new NodeTest(&serviceManager);
//        assignmentTest = new AssignmentTest(&serviceManager);
//        particleTest = new ParticleTest(&serviceManager);
    });
    QObject::connect(userTest, &UserTest::logout,[&]{
        qDebug()<<"logout";
//        delete layerTest;
//        delete nodeTest;
//        delete assignmentTest;
//        delete particleTest;
    });


    return a.exec();
}

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
    UserTest userTest(&serviceManager);
//    LayerTest LayerTest(&serviceManager);

//    NodeTest nodeTest(&serviceManager);
//    AssignmentTest assignmentTest(&serviceManager);
//    ParticleTest particleTest(&serviceManager);

    return a.exec();
}

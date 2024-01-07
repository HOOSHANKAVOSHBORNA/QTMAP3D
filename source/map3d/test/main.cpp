#include <QCoreApplication>
#include "assignmentTest.h"
#include "layerTest.h"
#include "networkManager.h"
#include "flyableNodeTest.h"
#include "movableNodeTest.h"
#include "nodeTest.h"
#include "serviceManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NetworkManager networkManager;
    networkManager.start();
    ServiceManager serviceManager(&networkManager);
//    QObject::connect(&networkManager, &NetworkManager::flyableQueueDeclared,[&networkManager]{
//        networkManager.sendFlyableData("data");
//    });
    LayerTest LayerTest(&networkManager);
    FlyableNodeTest flyableNodeTest(&networkManager);
    MovableNodeTest movableNodeTest(&networkManager);
    NodeTest nodeTest(&networkManager);
    AssignmentTest assignmentTest(&networkManager);

    return a.exec();
}

#include <QCoreApplication>
#include "layerTest.h"
#include "networkManager.h"
#include "flyableNodeTest.h"
#include "nodeTest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NetworkManager networkManager;
    networkManager.start();
//    QObject::connect(&networkManager, &NetworkManager::flyableQueueDeclared,[&networkManager]{
//        networkManager.sendFlyableData("data");
//    });
    LayerTest LayerTest(&networkManager);
    FlyableNodeTest flyableNodeTest(&networkManager);
    NodeTest nodeTest(&networkManager);

    return a.exec();
}

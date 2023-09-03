#include <QCoreApplication>
#include "layerTest.h"
#include "networkManager.h"
#include "modelNodeTest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NetworkManager networkManager;
//    QObject::connect(&networkManager, &NetworkManager::flyableQueueDeclared,[&networkManager]{
//        networkManager.sendFlyableData("data");
//    });
    LayerTest LayerTest(&networkManager);
//    ModelNodeTest modelNodeTest(&networkManager);

    networkManager.start();

    return a.exec();
}

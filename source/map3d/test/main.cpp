#include <QCoreApplication>
#include "layerTest.h"
#include "networkManager.h"
#include "modelNodeTest.h"
#include "statusNodeTest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NetworkManager networkManager;
    networkManager.start();
//    QObject::connect(&networkManager, &NetworkManager::flyableQueueDeclared,[&networkManager]{
//        networkManager.sendFlyableData("data");
//    });
    LayerTest LayerTest(&networkManager);
    StatusNodeTest statusNodeTest(&networkManager);
    ModelNodeTest modelNodeTest(&networkManager, &statusNodeTest);



    return a.exec();
}

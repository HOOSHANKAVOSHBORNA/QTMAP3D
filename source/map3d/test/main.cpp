#include <QCoreApplication>
#include "networkManager.h"
#include "modelNodeTest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NetworkManager networkManager;
//    QObject::connect(&networkManager, &NetworkManager::flyableQueueDeclared,[&networkManager]{
//        networkManager.sendFlyableData("data");
//    });
    ModelNodeTest modelNodeTest(&networkManager);

    networkManager.start();

    return a.exec();
}

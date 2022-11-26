#include "server.h"
#include"createdata.h"

#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server(1234);

//    Q_UNUSED(server)
    //run demo ------------------------------------------------
    CreateData createData;
    QTimer *timer = new QTimer();
    //createData.createTargetinfo();
    QObject::connect(timer, &QTimer::timeout, [&](){
        createData.createTargetinfo();
        createData.updateTargetinfo();
        for(auto targetInfo:createData.targetList)
            server.sendMessageToAll(targetInfo);
    });
    timer->start(10000);
    //---------------------------------------------------------


    return a.exec();
}

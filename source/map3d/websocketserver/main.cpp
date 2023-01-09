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

    createData.createStationInfo();
    createData.createSystemInfo();
    //----------------------------------------------------------
    QObject::connect(timer, &QTimer::timeout, [&](){
        createData.createAircraftInfo();
        createData.updateAircraftInfo();
        for(auto targetInfo:createData.aircraftList)
            server.sendMessageToAll(targetInfo);
        //---------------------------------------------
        for(auto station:createData.stationList)
            server.sendMessageToAll(station);
        //---------------------------------------------
        for(auto system:createData.systemList)
            server.sendMessageToAll(system);
    });
    timer->start(10000);
    //---------------------------------------------------------


    return a.exec();
}

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
    createData.createAircraftInfo();
    //----------------------------------------------------------
    QObject::connect(timer, &QTimer::timeout, [&](){
        //---------------------------------------------
        for(auto station:createData.stationList)
            server.sendMessageToAll(station);
        //---------------------------------------------
        for(auto system:createData.systemList)
            server.sendMessageToAll(system);
        //---------------------------------------------
        //createData.createAircraftInfo();
        createData.updateAircraftInfo();
        for(auto targetInfo:createData.aircraftList)
            server.sendMessageToAll(targetInfo);
    });
    timer->start(10000);
    //---------------------------------------------------------


    return a.exec();
}

#include "server.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server(1234);

    Q_UNUSED(server)

    return a.exec();
}

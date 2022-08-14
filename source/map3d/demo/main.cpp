
#include <QApplication>
#include "map3dlib.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Q_INIT_RESOURCE(map3dlib);
    Map3dlib w;
    w.show();
    return a.exec();
}

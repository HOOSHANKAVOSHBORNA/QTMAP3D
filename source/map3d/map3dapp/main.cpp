
#include <QApplication>
#include "map3dlib.h"

#include <QSurfaceFormat>

int main(int argc, char *argv[])
{

    QSurfaceFormat  fmt;
    fmt.setVersion(2,0);
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    fmt.setSamples(1);
    QSurfaceFormat::setDefaultFormat(fmt);

    QApplication a(argc, argv);
    //Q_INIT_RESOURCE(map3dlib);
    Map3dlib w;
    w.show();
    return a.exec();
}

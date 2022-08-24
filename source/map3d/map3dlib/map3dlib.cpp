#include "map3dlib.h"


Map3dlib::Map3dlib(QWidget *parent)
    : QMainWindow(parent)
{
    //    QDirIterator it(":", QDirIterator::Subdirectories);
    //    while (it.hasNext()) {
    //        qDebug() << it.next();
    //    }
    QIcon ic(":/res/map3d.ico");
    setWindowIcon(ic);

    //auto map3dWidget = new Map3dWidget(true, this);
    auto map3dWidget = new Map3dWidget(QString("../map3dlib/res/earth_files/geocentric.earth"), this);
    setCentralWidget(map3dWidget);
}


#ifndef MAP3DLIB_H
#define MAP3DLIB_H

#include "map3dlib_global.h"
#include <QMainWindow>

class MAP3DLIB_EXPORT Map3dlib: public QMainWindow
{
    Q_OBJECT

public:
    Map3dlib(QWidget *parent = nullptr);
};

#endif // MAP3DLIB_H

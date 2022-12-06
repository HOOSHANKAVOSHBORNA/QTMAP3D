#ifndef OSGQUICKWINDOW_H
#define OSGQUICKWINDOW_H

#include <QWindow>
#include <QQuickWindow>
#include <QOpenGLFunctions_2_0>
#include <OpenThreads/ReadWriteMutex>
#include <osgEarthUtil/Sky>
#include <osgEarth/ImageLayer>
#include <osgEarth/MapOptions>
#include <osgEarth/Profile>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <QTime>

#include "osgrenderer.h"
#include "mapcontroller.h"


class OsgQuickWindow : public QQuickWindow
{
    Q_OBJECT

public:
    OsgQuickWindow(QWindow *parent = nullptr);
    ~OsgQuickWindow();

    MapController* mapController() const;


public slots:
    void cleanup();
    void frame();

    void restoreContext();

private:
    void initializeGL();
    void resizeGL();
    void paintGL();

protected:
    void resizeEvent(QResizeEvent *) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;



signals:
    void clicked();
    void osgInitialized();

private:
    QOpenGLFunctions_2_0 *mOGLF = nullptr;

    bool mResized     = false;

    int mViewportWidth = 0;
    int mViewportHeight = 0;

    bool mIsFirstFrame = true;


    QTime mLastMousePressTime = QTime::currentTime();
    QPoint mLastPressPoint = QPoint();

    QOpenGLContext *mContext = nullptr;
    QSurface       *mSurface = nullptr;


    bool mMousePressOusideClickProcess = false;
    bool mInClickProcess = false;

protected:
    MapController *mMapController = nullptr;
};

#endif // OSGQUICKWINDOW_H

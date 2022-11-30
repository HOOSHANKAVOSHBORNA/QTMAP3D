#ifndef OsgController_H
#define OsgController_H

#include <QObject>
#include <QQuickWindow>
#include <QObject>
#include <QEvent>
#include <QScreen>
#include <GL/gl.h>
#include <osgEarth/MapNode>
#include <osgEarth/Map>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/Sky>

#include "osgrenderer.h"

class OsgController : public QObject
{
    friend class OsgQuickWindow;

    Q_OBJECT

public:

protected:
    explicit OsgController(QQuickWindow *window);
    virtual ~OsgController();

    void cleanup();

    void initializeGL(int width, int height, QScreen *screen, GLuint renderTargetId); void resizeGL(int width, int height, QScreen *screen);
    void paintGL();

    virtual void installEventHandler() = 0;

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    void createOsgRenderer(int width, int height, QScreen *screen);
    void initializeOsgEarth();
    void createMapNode(bool bGeocentric);
    void createCameraManipulator();

protected:
    osg::ref_ptr<osgEarth::MapNode> mMapNode;
    osg::ref_ptr<osgEarth::Util::SkyNode> mSkyNode;
    osg::ref_ptr<osg::Group> mMapRoot;

protected:
    OSGRenderer *mpOsgRenderer = nullptr;
    bool misFirstFrame = true;
    GLuint mrenderTargetId = 0;
    QQuickWindow *mpWindow = nullptr;

    osgEarth::Util::EarthManipulator *mpEarthManipulator = nullptr;

    bool mbGeocentric = true;

};

#endif // OsgController_H

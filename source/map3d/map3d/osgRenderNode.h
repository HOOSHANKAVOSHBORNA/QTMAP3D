#ifndef OSGRENDERNODE_H
#define OSGRENDERNODE_H

#include <QSGRenderNode>
#include <osgViewer/Viewer>
#include <QOpenGLFunctions_2_0>

class MapItem;
class QQuickItem;
class QInputEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QHoverEvent;

namespace eveBIM
{
    class ViewerWidget;
}

class OSGRenderNode: public QSGRenderNode , public osgViewer::Viewer, public QObject
{
public:
    OSGRenderNode(MapItem *parent);
    ~OSGRenderNode() override;
    void render(const RenderState *state) override;
    void releaseResources() override;
    StateFlags changedStates() const override;
    RenderingFlags flags() const override;
    QRectF rect() const override;
    void sync(QQuickItem *item);
    //osg----------------------------------------
    bool continuousUpdate() const
    {
        return mcontinuousUpdate;
    }
    void setContinuousUpdate(bool continuousUpdate)
    {
        mcontinuousUpdate = continuousUpdate;
    }

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void hoverMoveEvent(QHoverEvent *event);
    virtual void wheelEvent(QWheelEvent* event);

    virtual void resize(int windowX, int windowY, int windowWidth, int windowHeight, float windowScale);

    void setupOSG(int windowX, int windowY, int windowWidth, int windowHeight, float windowScale);

    // overrided from osgViewer::Viewer
    virtual bool checkNeedToDoFrame() override;

    // overrided from osgViewer::ViewerBase
    void frame(double simulationTime = USE_REFERENCE_TIME) override;

    // overrided from osgViewer::Viewer
    void requestRedraw() override;
    // overrided from osgViewer::Viewer
    bool checkEvents() override;
    void update();

protected:
    void timerEvent(QTimerEvent* event) override;
    void setKeyboardModifiers(QInputEvent* event);
private:
    friend class eveBIM::ViewerWidget;
    bool mosgInitialized {false};
    osg::ref_ptr<osgViewer::GraphicsWindow> mosgWinEmb;
    float mwindowScale {1.0f};
    bool mcontinuousUpdate {true};
    int  _timerId{0};
    osg::Timer _lastFrameStartTime;
    bool _applicationAboutToQuit {false};
    bool _osgWantsToRenderFrame{true};

    int mWidth = 0;
    int mHeight = 0;
    int mX = 0;
    int mXOld = -1;
    int mY = 0;
    int mYOld = -1;

    MapItem* mOSGItem{nullptr};
    QOpenGLFunctions_2_0 *mGLFunctions = nullptr;
};

#endif // OSGRENDERNODE_H

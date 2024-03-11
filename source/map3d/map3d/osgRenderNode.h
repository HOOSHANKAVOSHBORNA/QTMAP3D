#ifndef OSGRENDERNODE_H
#define OSGRENDERNODE_H

#include <QSGRenderNode>
#include <osgViewer/Viewer>
#include <QOpenGLFunctions_2_0>

class OsgViewerItem;
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

class myViewer: public osgViewer::Viewer
{
public:
    myViewer(): osgViewer::Viewer() {}
    ~myViewer(){
        qDebug() << "~myViewer";
    }
};

class OSGRenderNode: public QObject, public myViewer, public QSGRenderNode
{
public:
    OSGRenderNode(OsgViewerItem *parent);
    ~OSGRenderNode() override;
    void render(const RenderState *state) override;
    void releaseResources() override;
    StateFlags changedStates() const override;
    RenderingFlags flags() const override;
    QRectF rect() const override;
    void sync(QQuickItem *item);
    //osg----------------------------------------

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

    virtual bool checkNeedToDoFrame() override;

    void frame(double simulationTime = USE_REFERENCE_TIME) override;

    void requestRedraw() override;
    bool checkEvents() override;
    void update();

protected:
    void timerEvent(QTimerEvent* event) override;
    void setKeyboardModifiers(QInputEvent* event);
private:
    friend class eveBIM::ViewerWidget;
    bool mosgInitialized {false};
    osg::ref_ptr<osgViewer::GraphicsWindow> mosgWinEmb{nullptr};
    float mwindowScale {1.0f};
    // bool mcontinuousUpdate {true};
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

    OsgViewerItem* mOSGItem{nullptr};
    QOpenGLFunctions_2_0 *mGLFunctions = nullptr;
};

#endif // OSGRENDERNODE_H

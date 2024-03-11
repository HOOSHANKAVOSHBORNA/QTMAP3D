#include "osgRenderNode.h"
#include <QScreen>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLFunctions_2_0>
#include <QQuickItem>
#include <QQuickWindow>
#include <QWheelEvent>
#include <QWindow>

#include <QThread>
#include <osgGA/GUIEventAdapter>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgViewer/config/SphericalDisplay>
#include <QOpenGLFramebufferObject>
#include <QQuickOpenGLUtils>
#include "osgViewerItem.h"
#include <osg/GraphicsContext>
OSGRenderNode::OSGRenderNode(OsgViewerItem *parent):
    QObject(parent),
    myViewer(),
    QSGRenderNode(),
    mOSGItem(parent)
{
    releaseResources();
}

OSGRenderNode::~OSGRenderNode()
{
    qDebug() << "~OSGRenderNode";
    releaseResources();
}

void OSGRenderNode::releaseResources()
{
    QSGRenderNode::releaseResources();
}

void OSGRenderNode::render(const RenderState *state)
{
    if(mOSGItem) {
        QQuickOpenGLUtils::resetOpenGLState();
        frame();
        mOSGItem->frame();
        QQuickOpenGLUtils::resetOpenGLState();
    }
}

//! [4]
QSGRenderNode::StateFlags OSGRenderNode::changedStates() const
{
    return DepthState|BlendState | ScissorState | StencilState |ColorState|BlendState|CullState|ViewportState|RenderTargetState;
}

QSGRenderNode::RenderingFlags OSGRenderNode::flags() const
{
    return BoundedRectRendering ;
}

QRectF OSGRenderNode::rect() const
{
    return QRect(0, 0, mWidth, mHeight);
}
//! [4]

void OSGRenderNode::sync(QQuickItem *item)
{

    QPointF scenePoint(0, 0);
    scenePoint = item->mapToScene(scenePoint);
    mX = static_cast<int>(scenePoint.x());
    mY = static_cast<int>(item->window()->height() - item->height() - scenePoint.y());
    if(mY != mYOld || mXOld != mX || mWidth != item->width() || mHeight != item->height())
    {
        mWidth = static_cast<int>(item->width());
        mHeight = static_cast<int>(item->height());
        mXOld = mX;
        mYOld = mY;
        resize(mX, mY, mWidth, mHeight, 1);
    }
}

//osg renderer------------------------------------------------------------------
namespace
{

    class QtKeyboardMap
    {
    public:
        QtKeyboardMap()
        {
            mKeyMap[Qt::Key_Escape     ] = osgGA::GUIEventAdapter::KEY_Escape;
            mKeyMap[Qt::Key_Delete     ] = osgGA::GUIEventAdapter::KEY_Delete;
            mKeyMap[Qt::Key_Home       ] = osgGA::GUIEventAdapter::KEY_Home;
            mKeyMap[Qt::Key_Enter      ] = osgGA::GUIEventAdapter::KEY_KP_Enter;
            mKeyMap[Qt::Key_End        ] = osgGA::GUIEventAdapter::KEY_End;
            mKeyMap[Qt::Key_Return     ] = osgGA::GUIEventAdapter::KEY_Return;
            mKeyMap[Qt::Key_PageUp     ] = osgGA::GUIEventAdapter::KEY_Page_Up;
            mKeyMap[Qt::Key_PageDown   ] = osgGA::GUIEventAdapter::KEY_Page_Down;
            mKeyMap[Qt::Key_Left       ] = osgGA::GUIEventAdapter::KEY_Left;
            mKeyMap[Qt::Key_Right      ] = osgGA::GUIEventAdapter::KEY_Right;
            mKeyMap[Qt::Key_Up         ] = osgGA::GUIEventAdapter::KEY_Up;
            mKeyMap[Qt::Key_Down       ] = osgGA::GUIEventAdapter::KEY_Down;
            mKeyMap[Qt::Key_Backspace  ] = osgGA::GUIEventAdapter::KEY_BackSpace;
            mKeyMap[Qt::Key_Tab        ] = osgGA::GUIEventAdapter::KEY_Tab;
            mKeyMap[Qt::Key_Space      ] = osgGA::GUIEventAdapter::KEY_Space;
            mKeyMap[Qt::Key_Delete     ] = osgGA::GUIEventAdapter::KEY_Delete;
            mKeyMap[Qt::Key_Alt        ] = osgGA::GUIEventAdapter::KEY_Alt_L;
            mKeyMap[Qt::Key_Shift      ] = osgGA::GUIEventAdapter::KEY_Shift_L;
            mKeyMap[Qt::Key_Control    ] = osgGA::GUIEventAdapter::KEY_Control_L;
            mKeyMap[Qt::Key_Meta       ] = osgGA::GUIEventAdapter::KEY_Meta_L;
            mKeyMap[Qt::Key_F1             ] = osgGA::GUIEventAdapter::KEY_F1;
            mKeyMap[Qt::Key_F2             ] = osgGA::GUIEventAdapter::KEY_F2;
            mKeyMap[Qt::Key_F3             ] = osgGA::GUIEventAdapter::KEY_F3;
            mKeyMap[Qt::Key_F4             ] = osgGA::GUIEventAdapter::KEY_F4;
            mKeyMap[Qt::Key_F5             ] = osgGA::GUIEventAdapter::KEY_F5;
            mKeyMap[Qt::Key_F6             ] = osgGA::GUIEventAdapter::KEY_F6;
            mKeyMap[Qt::Key_F7             ] = osgGA::GUIEventAdapter::KEY_F7;
            mKeyMap[Qt::Key_F8             ] = osgGA::GUIEventAdapter::KEY_F8;
            mKeyMap[Qt::Key_F9             ] = osgGA::GUIEventAdapter::KEY_F9;
            mKeyMap[Qt::Key_F10            ] = osgGA::GUIEventAdapter::KEY_F10;
            mKeyMap[Qt::Key_F11            ] = osgGA::GUIEventAdapter::KEY_F11;
            mKeyMap[Qt::Key_F12            ] = osgGA::GUIEventAdapter::KEY_F12;
            mKeyMap[Qt::Key_F13            ] = osgGA::GUIEventAdapter::KEY_F13;
            mKeyMap[Qt::Key_F14            ] = osgGA::GUIEventAdapter::KEY_F14;
            mKeyMap[Qt::Key_F15            ] = osgGA::GUIEventAdapter::KEY_F15;
            mKeyMap[Qt::Key_F16            ] = osgGA::GUIEventAdapter::KEY_F16;
            mKeyMap[Qt::Key_F17            ] = osgGA::GUIEventAdapter::KEY_F17;
            mKeyMap[Qt::Key_F18            ] = osgGA::GUIEventAdapter::KEY_F18;
            mKeyMap[Qt::Key_F19            ] = osgGA::GUIEventAdapter::KEY_F19;
            mKeyMap[Qt::Key_F20            ] = osgGA::GUIEventAdapter::KEY_F20;
            mKeyMap[Qt::Key_hyphen         ] = '-';
            mKeyMap[Qt::Key_Equal         ] = '=';
            mKeyMap[Qt::Key_division      ] = osgGA::GUIEventAdapter::KEY_KP_Divide;
            mKeyMap[Qt::Key_multiply      ] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
            mKeyMap[Qt::Key_Minus         ] = '-';
            mKeyMap[Qt::Key_Plus          ] = '+';
            mKeyMap[Qt::Key_Insert        ] = osgGA::GUIEventAdapter::KEY_KP_Insert;
        }

        ~QtKeyboardMap()
        {
        }

        int remapKey(QKeyEvent* event)
        {
            KeyMap::iterator itr = mKeyMap.find(event->key());

            if(itr == mKeyMap.end())
            {
                return int(*(event->text().toLatin1().data()));
            }
            else
                return itr->second;
        }

    private:
        typedef std::map<unsigned int, int> KeyMap;
        KeyMap mKeyMap;
    };

    static QtKeyboardMap s_QtKeyboardMap;
} // namespace

void OSGRenderNode::update()
{
//    QQuickItem* osgItem = dynamic_cast<QQuickItem*>(QObject::parent());

    if(mOSGItem != nullptr)
    {
        mOSGItem->update();
    }
}

void OSGRenderNode::resize(int windowX, int windowY, int windowWidth, int windowHeight, float windowScale)
{
    if(!mosgInitialized)
        return;

    mwindowScale = windowScale;

    /*  _camera->setViewport(new osg::Viewport(0, 0, windowWidth * windowScale,
                                           windowHeight * windowScale));*/
    mosgWinEmb->resized(windowX * windowScale, windowX * windowScale,
                         windowWidth * windowScale,
                         windowHeight * windowScale);
    mosgWinEmb->getEventQueue()->windowResize(windowX * windowScale, windowY * windowScale,
                                               windowWidth * windowScale,
                                               windowHeight * windowScale);
    _camera->setViewport(new osg::Viewport(windowX * windowScale, windowY *windowScale, windowWidth * windowScale,
                                           windowHeight * windowScale));

    update();
}


void OSGRenderNode::setupOSG(int windowX, int windowY, int windowWidth, int windowHeight, float windowScale)
{


    mosgInitialized = true;
    mwindowScale = windowScale;
    mosgWinEmb = new osgViewer::GraphicsWindowEmbedded(windowX * windowScale, windowY * windowScale,
                                                        windowWidth * windowScale, windowHeight * windowScale);
    //mosgWinEmb = new osgViewer::GraphicsWindowEmbedded(0, 0, windowWidth * windowScale, windowHeight * windowScale);
    // make sure the event queue has the correct window rectangle size and input range
    mosgWinEmb->getEventQueue()->syncWindowRectangleWithGraphicsContext();
    _camera->setViewport(new osg::Viewport(windowX * windowScale, windowY *windowScale, windowWidth * windowScale,
                                           windowHeight * windowScale));
    _camera->setGraphicsContext(mosgWinEmb.get());
    // disable key event (default is Escape key) that the viewer checks on each
    // frame to see
    // if the viewer's done flag should be set to signal end of viewers main
    // loop.
    setKeyEventSetsDone(0);
    setReleaseContextAtEndOfFrameHint(false);
    setThreadingModel(osgViewer::Viewer::SingleThreaded);

    osgViewer::Viewer::Windows windows;
    getWindows(windows);

    _timerId = startTimer(10, Qt::PreciseTimer);
    _lastFrameStartTime.setStartTick(0);
}

void OSGRenderNode::setKeyboardModifiers(QInputEvent* event)
{
    unsigned int modkey = event->modifiers() & (Qt::ShiftModifier |
                                                Qt::ControlModifier |
                                                Qt::AltModifier);
    unsigned int mask = 0;

    if(modkey & Qt::ShiftModifier) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;

    if(modkey & Qt::ControlModifier) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;

    if(modkey & Qt::AltModifier) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;

    mosgWinEmb->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

void OSGRenderNode::keyPressEvent(QKeyEvent* event)
{
    setKeyboardModifiers(event);
    int value = s_QtKeyboardMap.remapKey(event);
    auto eventAdapter = mosgWinEmb->getEventQueue()->keyPress(value);
    //--run event immediately---
    eventTraversal();
    event->setAccepted(eventAdapter->getHandled());
}

void OSGRenderNode::keyReleaseEvent(QKeyEvent* event)
{
    if(event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        setKeyboardModifiers(event);
        int value = s_QtKeyboardMap.remapKey(event);
        auto eventAdapter = mosgWinEmb->getEventQueue()->keyRelease(value);
        //--run event immediately---
        eventTraversal();
        event->setAccepted(eventAdapter->getHandled());

    }
}

void OSGRenderNode::mousePressEvent(QMouseEvent* event)
{
    int button = 0;

    switch(event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    case Qt::NoButton:
        button = 0;
        break;

    default:
        button = 0;
        break;
    }

    setKeyboardModifiers(event);
    auto eventAdapter = mosgWinEmb->getEventQueue()->mouseButtonPress(event->scenePosition().x() * mwindowScale,
                                                  event->position().y() * mwindowScale, button);
//    qDebug()<<event->scenePosition();
    //--run event immediately---
    eventTraversal();
//    qDebug()<< eventAdapter->getHandled();
    event->setAccepted(eventAdapter->getHandled());
}

void OSGRenderNode::mouseReleaseEvent(QMouseEvent* event)
{
    int button = 0;

    switch(event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    case Qt::NoButton:
        button = 0;
        break;

    default:
        button = 0;
        break;
    }

    setKeyboardModifiers(event);
    int y = mOSGItem->height() - event->position().y();
    auto eventAdapter = mosgWinEmb->getEventQueue()->mouseButtonRelease(event->scenePosition().x() * mwindowScale,
                                                    event->position().y() * mwindowScale,
                                                     button);
    //--run event immediately---
    eventTraversal();
    event->setAccepted(eventAdapter->getHandled());
}

void OSGRenderNode::mouseDoubleClickEvent(QMouseEvent* event)
{
    int button = 0;

    switch(event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MiddleButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    case Qt::NoButton:
        button = 0;
        break;

    default:
        button = 0;
        break;
    }

    setKeyboardModifiers(event);
    auto eventAdapter = mosgWinEmb->getEventQueue()->mouseDoubleButtonPress(event->scenePosition().x() * mwindowScale,
                                                        event->position().y() * mwindowScale, button);
    //--run event immediately---
    eventTraversal();
    event->setAccepted(eventAdapter->getHandled());
}

void OSGRenderNode::mouseMoveEvent(QMouseEvent* event)
{
    setKeyboardModifiers(event);
//    mosgWinEmb->getEventQueue()->mouseMotion(event->position().x() * mwindowScale,
//                                             event->position().y() * mwindowScale);
    auto eventAdapter = mosgWinEmb->getEventQueue()->mouseMotion(event->scenePosition().x() * mwindowScale,
                                             event->position().y() * mwindowScale);
    //--run event immediately---
    eventTraversal();
    event->setAccepted(eventAdapter->getHandled());
}

void OSGRenderNode::hoverMoveEvent(QHoverEvent *event)
{
//    qDebug()<<"event->scenePosition(): "<<event->scenePosition();
//    qDebug()<<"event->position(): "<<event->position();

//    setKeyboardModifiers(event);
//    int y = event->position().y();
    auto eventAdapter = mosgWinEmb->getEventQueue()->mouseMotion(event->scenePosition().x() * mwindowScale,
                                             event->position().y() * mwindowScale);
    //--run event immediately---
    eventTraversal();
    event->setAccepted(eventAdapter->getHandled());
}

void OSGRenderNode::wheelEvent(QWheelEvent* event)
{
    setKeyboardModifiers(event);
    mosgWinEmb->getEventQueue()->mouseMotion(event->scenePosition().x() * mwindowScale,
                                             event->position().y() * mwindowScale);
    auto eventAdapter = mosgWinEmb->getEventQueue()->mouseScroll(
        event->angleDelta().y() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
    //--run event immediately---
    eventTraversal();
    event->setAccepted(eventAdapter->getHandled());
}

bool OSGRenderNode::checkEvents()
{
    // check events from any attached sources
    for(Devices::iterator eitr = _eventSources.begin();
        eitr != _eventSources.end();
        ++eitr)
    {
        osgGA::Device* es = eitr->get();

        if(es->getCapabilities() & osgGA::Device::RECEIVE_EVENTS)
        {
            if(es->checkEvents())
                return true;
        }

    }

    // get events from all windows attached to Viewer.
    Windows windows;
    getWindows(windows);

    for(Windows::iterator witr = windows.begin();
        witr != windows.end();
        ++witr)
    {
        if((*witr)->checkEvents())
            return true;
    }

    return false;
}

bool OSGRenderNode::checkNeedToDoFrame()
{
    // check if any event handler has prompted a redraw
    if(_requestRedraw)
        return true;

    if(_requestContinousUpdate)
        return true;

    // check if the view needs to update the scene graph
    // this check if camera has update callback and if scene requires to update scene graph
    if(requiresUpdateSceneGraph())
        return true;

    // check if the database pager needs to update the scene
    if(getDatabasePager()->requiresUpdateSceneGraph())
        return true;

    // check if the image pager needs to update the scene
    if(getImagePager()->requiresUpdateSceneGraph())
        return true;


    // check if the scene needs to be redrawn
    if(requiresRedraw())
        return true;

    // check if events are available and need processing
    if(checkEvents())
        return true;

    // and check again if any event handler has prompted a redraw
    if(_requestRedraw)
        return true;

    if(_requestContinousUpdate)
        return true;

    return false;
}

// called from ViewerWidget paintGL() method
void OSGRenderNode::frame(double simulationTime)
{
    // limit the frame rate
    if(getRunMaxFrameRate() > 0.0)
    {
        double dt = _lastFrameStartTime.time_s();
        double minFrameTime = 1.0 / getRunMaxFrameRate();

        if(dt < minFrameTime)
            QThread::usleep(static_cast<unsigned int>(1000000.0 * (minFrameTime - dt)));
    }

    // avoid excessive CPU loading when no frame is required in ON_DEMAND mode
    if(getRunFrameScheme() == osgViewer::ViewerBase::ON_DEMAND)
    {
        double dt = _lastFrameStartTime.time_s();

        if(dt < 0.01)
            OpenThreads::Thread::microSleep(static_cast<unsigned int>(1000000.0 *
                                                                      (0.01 - dt)));
    }

    // record start frame time
    _lastFrameStartTime.setStartTick();
    // make frame

#if 1
    osgViewer::Viewer::frame(simulationTime);
#else

    if(_done) return;

    // OSG_NOTICE<<std::endl<<"CompositeViewer::frame()"<<std::endl<<std::endl;

    if(_firstFrame)
    {
        viewerInit();

        if(!isRealized())
        {
            realize();
        }

        _firstFrame = false;
    }

    advance(simulationTime);

    eventTraversal();
    updateTraversal();
    //    renderingTraversals();
#endif
}

void OSGRenderNode::requestRedraw()
{
    osgViewer::Viewer::requestRedraw();
}

void OSGRenderNode::timerEvent(QTimerEvent* /*event*/)
{
    // application is about to quit, just return
    if(_applicationAboutToQuit)
    {
        return;
    }

    // ask ViewerWidget to update 3D view
    if(getRunFrameScheme() != osgViewer::ViewerBase::ON_DEMAND ||
       checkNeedToDoFrame())
    {
        update();
    }
}

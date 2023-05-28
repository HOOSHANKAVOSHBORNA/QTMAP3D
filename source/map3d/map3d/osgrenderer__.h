
// Copyright (C) 2017 Mike Krus
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef OSGRENDERER_H
#define OSGRENDERER_H

#include <QObject>
#include <osgViewer/Viewer>
#include <QQuickItem>
#include <QSGRenderNode>

class QInputEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
namespace eveBIM
{
    class ViewerWidget;
}

class OSGRenderer : public QObject, public osgViewer::Viewer, public QSGRenderNode
{
    bool                                       mosgInitialized {false};
    osg::ref_ptr<osgViewer::GraphicsWindow>    mosgWinEmb;
    float                                      mwindowScale {1.0f};
    bool                                       mcontinuousUpdate {true};

    int                                        _timerId{0};
    osg::Timer                                 _lastFrameStartTime;
    bool                                       _applicationAboutToQuit {false};
    bool                                       _osgWantsToRenderFrame{true};

    Q_OBJECT

    friend class eveBIM::ViewerWidget;

public:

    explicit OSGRenderer(QQuickItem* parent = nullptr);
    explicit OSGRenderer(osg::ArgumentParser* arguments, QQuickItem* parent = nullptr);

    ~OSGRenderer() override;

    // override from QSGRenderNode
    void render(const RenderState *state) override;
    void releaseResources() override;
    StateFlags changedStates() const override;
    RenderingFlags flags() const override;
    QRectF rect() const override;
    void sync(QQuickItem *item);
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


signals:
    void needsUpdate();
private:
    int mWidth = 0;
    int mHeight = 0;
    int mX = 0;
    int mXOld = -1;
    int mY = 0;
    int mYOld = -1;
    QQuickItem *mOSGItem{nullptr};

};

#endif // OSGRENDERER_H

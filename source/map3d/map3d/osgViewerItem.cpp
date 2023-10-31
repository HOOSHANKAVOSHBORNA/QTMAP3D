#include "osgViewerItem.h"

#include <QSGRendererInterface>
#include <QQuickWindow>


//--MapItem---------------------------------------------------------------------------------------------------------
OsgViewerItem::OsgViewerItem(QQuickItem *parent) :
    QQuickItem(parent)
{
    setAcceptHoverEvents(true);
    setFlags(ItemHasContents/*|ItemAcceptsDrops*/);
    setAcceptedMouseButtons(Qt::MouseButton::AllButtons);
    mOSGRenderNode = new OSGRenderNode(this);
}

OsgViewerItem::~OsgViewerItem()
{
    delete mOSGRenderNode;
}

osgViewer::Viewer *OsgViewerItem::getViewer() const
{
    return dynamic_cast<osgViewer::Viewer*>(mOSGRenderNode);
}

OSGRenderNode *OsgViewerItem::oSGRenderNode() const
{
    return mOSGRenderNode;
}

QSGNode *OsgViewerItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    QSGRenderNode *n = static_cast<QSGRenderNode *>(node);
    QSGRendererInterface *ri = window()->rendererInterface();
    if (!ri)
        return nullptr;

    if (!n)
    {
        //        mOSGRenderNode = new OSGRenderNode(this);
        //        createOsgRenderer();
        ////        mOSGRenderNode->setupOSG(x(), y(), width(), height(), 1);

        ////        setNode(mSource);
        //        initializeOsgEarth();

        mOSGRenderNode->setupOSG(0, 0, 800, 620, 1);

        n = mOSGRenderNode;
    }
    static_cast<OSGRenderNode *>(n)->sync(this);

    if (!n)
        qWarning("QSGRendererInterface reports unknown graphics API %d", ri->graphicsApi());

    return n;
}

void OsgViewerItem::frame()
{

}

void OsgViewerItem::keyPressEvent(QKeyEvent *event)
{
    if (mOSGRenderNode)
        mOSGRenderNode->keyPressEvent(event);
}

void OsgViewerItem::keyReleaseEvent(QKeyEvent *event)
{
    if (mOSGRenderNode)
        mOSGRenderNode->keyReleaseEvent(event);
}

void OsgViewerItem::mousePressEvent(QMouseEvent *event)
{
    if (mOSGRenderNode) {
        mOSGRenderNode->mousePressEvent(event);
    }

}

void OsgViewerItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (mOSGRenderNode) {
        mOSGRenderNode->mouseReleaseEvent(event);
    }
}

void OsgViewerItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (mOSGRenderNode)
        mOSGRenderNode->mouseDoubleClickEvent(event);
}

void OsgViewerItem::mouseMoveEvent(QMouseEvent *event)
{
    if (mOSGRenderNode) {
        mOSGRenderNode->mouseMoveEvent(event);
    }
}

void OsgViewerItem::wheelEvent(QWheelEvent *event)
{
    if (mOSGRenderNode)
        mOSGRenderNode->wheelEvent(event);
}

void OsgViewerItem::hoverMoveEvent(QHoverEvent *event)
{
    if (mOSGRenderNode) {
        mOSGRenderNode->hoverMoveEvent(event);
    }
}

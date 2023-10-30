#ifndef OSGVIEWERITEM_H
#define OSGVIEWERITEM_H

#include "osgRenderNode.h"
#include <QQuickItem>

class OsgViewerItem : public QQuickItem
{
    Q_OBJECT

public:
    explicit OsgViewerItem(QQuickItem *parent = nullptr);
    ~OsgViewerItem() override;
    Q_INVOKABLE osgViewer::Viewer *getViewer()const;
    OSGRenderNode *oSGRenderNode() const;

    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

public:
    virtual void frame();
    //events------------------------------
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void hoverMoveEvent(QHoverEvent *event) override;
private:
    OSGRenderNode *mOSGRenderNode{nullptr};
//    bool mIsFirstFrame{true};
};

#endif // OSGVIEWERITEM_H

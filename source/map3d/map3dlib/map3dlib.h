#ifndef MAP3DLIB_H
#define MAP3DLIB_H

#include <QMainWindow>

#include "map3dlib_global.h"
#include "map3dwidget.h"

class ToolBarWidget;
class WebSocketClient;

class MAP3DLIB_EXPORT Map3dlib: public QMainWindow
{
    Q_OBJECT
public:
    Map3dlib(QWidget *parent = nullptr);
    void addMenuWidget(QWidget* widget);
    void removeMenuWidget(QWidget* widget);
    WebSocketClient *getWebSocket() const;

public slots:

signals:
    void onPressEvent();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void onToolBarWidgetPin(bool isPin);
    void reorderMenuWidgets();
private:
    QDockWidget *mToolBarDocWidget;
    ToolBarWidget* mToolBarWidget;
    QList<QWidget*> mWidgets;
    WebSocketClient* mWebSocket{nullptr};
};

#endif // MAP3DLIB_H

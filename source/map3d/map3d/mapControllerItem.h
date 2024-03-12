#ifndef MAPCONTROLLERITEM_H
#define MAPCONTROLLERITEM_H

#include <QObject>
#include <QQmlEngine>
#include <QTime>

#include "mapItem.h"
#include "searchNodeModel.h"
#include "statusBar.h"

Q_DECLARE_METATYPE(SearchNodeModel)
class SmallMap;
class FilterManager;
class MapControllerItem : public MapItem
{
    Q_OBJECT

    Q_PROPERTY(QVector2D compassDirection READ compassDirection WRITE setCompassDirection NOTIFY compassDirectionChanged FINAL)
    Q_PROPERTY(QVector3D mapRotation READ getMapRotation WRITE setMapRotation NOTIFY mapRotationChanged FINAL)
    Q_PROPERTY(double fps READ fps WRITE setFps NOTIFY fpsChanged)

    Q_PROPERTY(bool zoomInButtonPressed WRITE setZoomInButtonPressed)
    Q_PROPERTY(bool zoomOutButtonPressed WRITE setZoomOutButtonPressed)

    Q_PROPERTY(QVector2D movePosition WRITE setMovePosition)
    Q_PROPERTY(QVector2D rotatePosition WRITE setRotatePosition)

    Q_PROPERTY(QVector3D mapMouseGeoLocation READ mapMouseGeoLocation NOTIFY mouseLocationChanged)
    Q_PROPERTY(QVector3D mapMouseLocation READ mapMouseLocation NOTIFY mouseLocationChanged)

    Q_PROPERTY(QQuickItem* topMenuItem READ topMenuItem WRITE setTopMenuItem NOTIFY topMenuItemChanged FINAL)
    Q_PROPERTY(bool topMenuVisible READ topMenuVisible WRITE setTopMenuVisible NOTIFY topMenuVisibleChanged FINAL)

public:
    static MapControllerItem *instance();
    ~MapControllerItem();
    void clearMap();
    void initialize();

    QVector3D mapMouseGeoLocation() const;
    QVector3D mapMouseLocation() const;
    virtual void frame() override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void hoverMoveEvent(QHoverEvent *event) override;

    QVector2D compassDirection() const;
    void setCompassDirection(const QVector2D &newCompassDirection);

    QVector3D getMapRotation();
    void setMapRotation(QVector3D angle);

    void setQmlEngine(QQmlEngine *newQmlEngine);

    QQuickItem *topMenuItem() const;
    void setTopMenuItem(QQuickItem *newTopMenuItem);

    bool topMenuVisible() const;
    void setTopMenuVisible(bool newTopMenuVisible);
    double fps() const;

    Q_INVOKABLE void setFps(double fps);
    Q_INVOKABLE void home();
    Q_INVOKABLE void setHeadingToNorth();
    Q_INVOKABLE void setZoomInButtonPressed(bool pressed);
    Q_INVOKABLE void setZoomOutButtonPressed(bool pressed);
    Q_INVOKABLE void setMovePosition(QVector2D &position);
    Q_INVOKABLE void setRotatePosition(QVector2D &position);

    Q_INVOKABLE StatusBarSearchModel *statusBar() const;
    Q_INVOKABLE SearchNodeProxyModel *searchNodeProxyModel() const;
    Q_INVOKABLE SearchNodeManager *searchNodeManager() const;
    Q_INVOKABLE FilterManager *filterManager() const;

signals:
    void fpsChanged();
    void mouseLocationChanged();
    void mousePointingLocationChanged(QVector3D location);
    void clicked();
    void searchChange();
    void compassDirectionChanged();
    void mapRotationChanged();

    void topMenuItemChanged();

    void topMenuVisibleChanged();

private:
    MapControllerItem(QQuickItem *parent = nullptr);
    void tickNavigation(double deltaTime);
    void calculateNavigationStep();
    void calculateFps();
    void createSearchNodeManager();
private:
    bool mZoomInButtonPressed{false};
    bool mZoomOutButtonPressed{false};

    QVector2D mMovePosition;
    QVector2D mRotatePosition;

    bool mMoveTick{false};
    bool mRotateTick{false};
    osgEarth::GeoPoint mCurrentMouseGeoPoint;

    QTime mLastMousePressTime = QTime::currentTime();
    QPoint mLastPressPoint = QPoint();
    bool mMousePressOusideClickProcess = false;
    bool mInClickProcess = false;
    double mFps = 0.0f;
    SearchNodeManager* mSearchNodeManager{nullptr};
    StatusBarSearchModel *mStatusBar{nullptr};
    QVector2D mCompassDirection;
    QVector3D mMapRotation;
    QQmlEngine *mQmlEngine{nullptr};
    QQuickItem *mTopMenuItem = nullptr;
    bool mTopMenuVisible = false;
    FilterManager *mFilterManager{nullptr};
    static inline MapControllerItem *mInstance{nullptr};
};

#endif // MAPCONTROLLERITEM_H

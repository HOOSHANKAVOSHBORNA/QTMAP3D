#ifndef MainWindow_H
#define MainWindow_H

#include <QQuickWindow>
#include <QQmlComponent>
#include <QVector3D>
#include <QTime>
#include <QPoint>

#include "pluginmanager.h"
#include "layersmodel.h"
#include "mapItem.h"
#include "toolbox.h"
class QOpenGLFunctions_2_0;
class PluginInfo;
class PluginInterface;
class UIHandle;
class ListWindow;
class LayersModel;

Q_DECLARE_METATYPE(MapItem)
class MainWindow : public QQuickWindow
{
    Q_OBJECT
//    Q_PROPERTY(MapItem mapItem READ getMapItem WRITE setMapItem)

//    Q_PROPERTY(qreal headingAngle READ headingAngle WRITE setHeadingAngle NOTIFY headingAngleChanged)
//    Q_PROPERTY(QVector3D mousePointingLocationWgs84 READ mousePointingLocationWgs84 WRITE setMousePointingLocationWgs84 NOTIFY mousePointingLocationWgs84Changed)
//    Q_PROPERTY(QVector3D mousePointingLocation READ mousePointingLocation WRITE setMousePointingLocation NOTIFY mousePointingLocationChanged)


//    Q_PROPERTY(qreal focalPointLat   READ focalPointLat   WRITE setFocalPointLat   NOTIFY focalPointLatChanged  )
//    Q_PROPERTY(qreal focalPointLong  READ focalPointLong  WRITE setFocalPointLong  NOTIFY focalPointLongChanged )
//    Q_PROPERTY(qreal focalPointRange READ focalPointRange WRITE setFocalPointRange NOTIFY focalPointRangeChanged)
//    Q_PROPERTY(qreal focalPointPitch READ focalPointPitch WRITE setFocalPointPitch NOTIFY focalPointPitchChanged)
//    Q_PROPERTY(qreal focalPointHead READ focalPointHead WRITE setFocalPointHead NOTIFY focalPointHeadChanged)
    Q_PROPERTY(qreal fps READ fps WRITE setFps NOTIFY fpsChanged)


//    Q_PROPERTY(bool zoomInButtonPressed      READ zoomInButtonPressed      WRITE setZoomInButtonPressed      NOTIFY zoomInButtonPressedChanged)
//    Q_PROPERTY(bool zoomOutButtonPressed     READ zoomOutButtonPressed     WRITE setZoomOutButtonPressed     NOTIFY zoomOutButtonPressedChanged)
//    Q_PROPERTY(bool upButtonPressed          READ upButtonPressed          WRITE setUpButtonPressed          NOTIFY upButtonPressedChanged)
//    Q_PROPERTY(bool downButtonPressed        READ downButtonPressed        WRITE setdownButtonPressed        NOTIFY downButtonPressedChanged)
//    Q_PROPERTY(bool leftButtonPressed        READ leftButtonPressed        WRITE setleftButtonPressed        NOTIFY leftButtonPressedChanged)
//    Q_PROPERTY(bool rightButtonPressed       READ rightButtonPressed       WRITE setrightButtonPressed       NOTIFY rightButtonPressedChanged)
//    Q_PROPERTY(bool rotateUpButtonPressed    READ rotateUpButtonPressed    WRITE setrotateUpButtonPressed    NOTIFY rotateUpButtonPressedChanged)
//    Q_PROPERTY(bool rotateDownButtonPressed  READ rotateDownButtonPressed  WRITE setrotateDownButtonPressed  NOTIFY rotateDownButtonPressedChanged)
//    Q_PROPERTY(bool rotateLeftButtonPressed  READ rotateLeftButtonPressed  WRITE setrotateLeftButtonPressed  NOTIFY rotateLeftButtonPressedChanged)
//    Q_PROPERTY(bool rotateRightButtonPressed READ rotateRightButtonPressed WRITE setrotateRightButtonPressed NOTIFY rotateRightButtonPressedChanged)


    Q_PROPERTY(LayersModel* layersModel READ layersModel WRITE setLayersModel NOTIFY layersModelChanged)
    Q_PROPERTY(ToolboxProxyModel* toolbox READ toolbox WRITE setToolbox NOTIFY layersModelChanged)

    enum class InfoWidgetType {
        Airplane,
        Station,
        System
    };

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow();

//    qreal headingAngle() const;
//    QVector3D mousePointingLocationWgs84() const;
//    QVector3D mousePointingLocation() const;

//    qreal focalPointLat  () const;
//    qreal focalPointLong () const;
//    qreal focalPointRange() const;
//    qreal focalPointPitch() const;
//    qreal focalPointHead() const;

    qreal fps() const;

//    bool zoomInButtonPressed() const;
//    bool zoomOutButtonPressed() const;
//    bool upButtonPressed() const;
//    bool downButtonPressed() const;
//    bool leftButtonPressed() const;
//    bool rightButtonPressed() const;
//    bool rotateUpButtonPressed() const;
//    bool rotateDownButtonPressed() const;
//    bool rotateLeftButtonPressed() const;
//    bool rotateRightButtonPressed() const;


    LayersModel *layersModel() const;
    ToolboxProxyModel *toolbox() const;

    UIHandle *uiHandle() const;

signals:
    void sideItemCreated(int index, QObject *pSideItem);
    void toolboxItemCreated(ItemDescProxy *itemProxy);
    void fileItemCreated(ItemDescProxy *itemProxy);
    void toolboxItemClicked(const QString& itemName, const QString& categoryName);
    void fileItemClicked(const QString& itemName, const QString& categoryName);
    void toolboxItemCheckedChanged(const QString& itemName,
                                   const QString& categoryName,
                                   bool checked);


//    void infoWidget2D3DButtonClicked();
//    void infoWidgetRouteButtonClicked();
//    void infoWidgetFollowButtonClicked();
//    void infoWidgetMoreButtonClicked();

//    void homeButtonClicked();
//    void view3DButtonClicked();

//    void headingAngleChanged(qreal angle);
//    void mousePointingLocationWgs84Changed();
//    void mousePointingLocationChanged();
//    void goToLocation(qreal lat, qreal lon, qreal range);

//    void focalPointLatChanged  ();
//    void focalPointLongChanged ();
//    void focalPointRangeChanged();
//    void focalPointPitchChanged();
//    void focalPointHeadChanged();

    void fpsChanged();


//    void zoomInButtonPressedChanged();
//    void zoomOutButtonPressedChanged();
//    void upButtonPressedChanged();
//    void downButtonPressedChanged();
//    void leftButtonPressedChanged();
//    void rightButtonPressedChanged();
//    void rotateUpButtonPressedChanged();
//    void rotateDownButtonPressedChanged();
//    void rotateLeftButtonPressedChanged();
//    void rotateRightButtonPressedChanged();

    void layersModelChanged();

    void toggleLayerEnabled(int layerIndex);

public slots:
    void initializePluginsUI(std::list<PluginInfo>& pluginsInfoList);
//    void setHeadingAngle(qreal angle);
//    void setMousePointingLocationWgs84(const QVector3D& pointedLoc);
//    void setMousePointingLocation(const QVector3D& pointedLoc);

//    void setFocalPointLat  (qreal focalPointLat) ;
//    void setFocalPointLong (qreal focalPointLong);
//    void setFocalPointRange(qreal focalPointRange) ;
//    void setFocalPointPitch(qreal focalPointPitch) ;
//    void setFocalPointHead(qreal focalPointHead) ;

    void setFps(qreal fps);

//    void setZoomInButtonPressed(bool pressed);
//    void setZoomOutButtonPressed(bool pressed);
//    void setUpButtonPressed(bool pressed);
//    void setdownButtonPressed(bool pressed);
//    void setleftButtonPressed(bool pressed);
//    void setrightButtonPressed(bool pressed);
//    void setrotateUpButtonPressed(bool pressed);
//    void setrotateDownButtonPressed(bool pressed);
//    void setrotateLeftButtonPressed(bool pressed);
//    void setrotateRightButtonPressed(bool pressed);

//    void travelToViewpoint(qreal latitude,
//                           qreal longitude,
//                           qreal range,
//                           qreal pitch,
//                           qreal heading);

    void showInfoWidget(InfoWidgetType infoWidgetType);
    void updateInfoWidgetData(const QString& dataJSON);
//    void setStatusBarText(const QString message, int time);


    void showListWindow();

    void setLayersModel(LayersModel *layersModel);
    void setToolbox(ToolboxProxyModel* toolbox);
    void onFrameSwapped();

//    void orientCameraToNorth();

public:

    MapItem* getMapItem();
//    void setMapItem(MapItem& mapItem);


public slots:
//    void cleanup();
    void frame();
    void tickNavigation(double deltaTime);

    void restoreContext();
    void setListWindow(ListWindow *listWindow);

private:
    void initializeGL();
//    void resizeGL();
//    void paintGL();

protected:
    void resizeEvent(QResizeEvent *) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    bool event(QEvent *ev) override;


signals:
//    void clicked();
//    void osgInitialized();

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
    MapItem *mMapItem = nullptr;

private:
//    qreal mheadingAngle = 0.0;
//    QVector3D mMousePointingLocationWgs84;
//    QVector3D mMousePointingLocation;

//    qreal mFocalPointLat   = 0.0;
//    qreal mFocalPointLong  = 0.0;
//    qreal mFocalPointRange = 0.0;
//    qreal mFocalPointPitch = 0.0;
//    qreal mFocalPointHead = 0.0;

    qreal mFps = 0.0f;

//    bool mZoomInButtonPressed{false};
//    bool mZoomOutButtonPressed{false};
//    bool mUpButtonPressed{false};
//    bool mDownButtonPressed{false};
//    bool mLeftButtonPressed{false};
//    bool mRightButtonPressed{false};
//    bool mRotateUpButtonPressed{false};
//    bool mRotateDownButtonPressed{false};
//    bool mRotateLeftButtonPressed{false};
//    bool mRotateRightButtonPressed{false};

    UIHandle *mUIHandle = nullptr;
    ListWindow *mListWindow = nullptr;
    LayersModel *mLayersModel = nullptr;
    ToolboxProxyModel *mToolbox = nullptr;
};

#endif // MainWindow_H

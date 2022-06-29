QT += core gui
QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#CONFIG += link_pkgconfig

#PKGCONFIG += osgDB
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Atlas.cpp \
    AtlasSplashScreen.cpp \
    main.cpp

HEADERS += \
    Atlas.h \
    AtlasSplashScreen.h \
    resource.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Atlas.rc \
    atlas.ico \
    resources/earth_files/geocentric.earth \
    resources/earth_files/projected.earth \
    resources/earth_files/world.tif \
    resources/fonts/arial.ttf \
    resources/fonts/simhei.ttf \
    resources/icons/3dglasses.png \
    resources/icons/ArcGIS.png \
    resources/icons/USGSTMS.png \
    resources/icons/addshp.png \
    resources/icons/angle.png \
    resources/icons/area.png \
    resources/icons/atlas.png \
    resources/icons/bulldozer.png \
    resources/icons/center.png \
    resources/icons/compare.png \
    resources/icons/compass.png \
    resources/icons/construct.png \
    resources/icons/contour.png \
    resources/icons/cow.png \
    resources/icons/crowd.png \
    resources/icons/cube.png \
    resources/icons/curve.png \
    resources/icons/drawline.png \
    resources/icons/drawpolygon.png \
    resources/icons/exit.png \
    resources/icons/find.png \
    resources/icons/fog.png \
    resources/icons/gdal.png \
    resources/icons/height.png \
    resources/icons/hide.png \
    resources/icons/hide_all.png \
    resources/icons/image.png \
    resources/icons/layer.png \
    resources/icons/loading.gif \
    resources/icons/mark.png \
    resources/icons/model.png \
    resources/icons/move.png \
    resources/icons/new.png \
    resources/icons/oblique_model.png \
    resources/icons/ogc.png \
    resources/icons/open.ico \
    resources/icons/pathroam.png \
    resources/icons/playback.png \
    resources/icons/point_cloud.png \
    resources/icons/rain.png \
    resources/icons/record.png \
    resources/icons/road.png \
    resources/icons/rotate.png \
    resources/icons/ruler.png \
    resources/icons/satellite.png \
    resources/icons/save.ico \
    resources/icons/save_as.ico \
    resources/icons/screenshot.png \
    resources/icons/settings.png \
    resources/icons/show_all.png \
    resources/icons/show_all_layers.png \
    resources/icons/smoke.png \
    resources/icons/snow.png \
    resources/icons/sunlight.png \
    resources/icons/terrain.png \
    resources/icons/tile.png \
    resources/icons/tree.png \
    resources/icons/triangulation.png \
    resources/icons/visibility.png \
    resources/icons/vision.png \
    resources/icons/weather.png \
    resources/icons/window.png \
    resources/images/atlas_big.png \
    resources/models/arrow.osgb \
    resources/models/cessna.osg \
    resources/models/cow.osgb \
    resources/models/dumptruck.osg \
    resources/models/man.osgb \
    resources/models/redmark.osgb \
    resources/models/tree.osgb \
    resources/scripts/a.path \
    resources/scripts/fly.path \
    resources/scripts/info.xml \
    resources/scripts/style.xml \
    resources/shaders/basicShader.frag \
    resources/shaders/basicShader.vert \
    resources/shaders/depthMap.frag \
    resources/shaders/depthMap.vert \
    resources/shaders/depthVisualizer.frag \
    resources/shaders/depthVisualizer.vert \
    resources/shaders/flattenShader.frag \
    resources/shaders/flattenShader.vert \
    resources/shaders/slope.frag \
    resources/shaders/slope.vert \
    resources/shaders/visibilityShader.frag \
    resources/shaders/visibilityShader.vert \
    resources/styles/Atlas.qss \
    resources/translations/atlas_zh.qm \
    resources/translations/atlas_zh.ts \
    resources/widgets/branch-closed.png \
    resources/widgets/branch-end.png \
    resources/widgets/branch-more.png \
    resources/widgets/branch-open.png \
    resources/widgets/checkbox_checked.png \
    resources/widgets/checkbox_unchecked.png \
    resources/widgets/close_dockwidget_hover.png \
    resources/widgets/close_dockwidget_normal.png \
    resources/widgets/close_dockwidget_pressed.png \
    resources/widgets/menu_dockwidget_hover.png \
    resources/widgets/menu_dockwidget_normal.png \
    resources/widgets/menu_dockwidget_pressed.png \
    resources/widgets/pin_dockwidget_hover.png \
    resources/widgets/pin_dockwidget_normal.png \
    resources/widgets/pin_dockwidget_pressed.png \
    resources/widgets/unpin_dockwidget_hover.png \
    resources/widgets/unpin_dockwidget_normal.png \
    resources/widgets/unpin_dockwidget_pressed.png \
    resources/widgets/vline.png

unix:!macx: LIBS += -L$$OUT_PWD/../atlasmap3d/ -latlasmap3d

INCLUDEPATH += $$PWD/../atlasmap3d
DEPENDPATH += $$PWD/../atlasmap3d

LIBS  += -L/usr/local/lib64/ -losg -losgViewer -losgGA -losgDB -losgManipulator -losgSim -losgParticle -losgText -losgUtil
LIBS  += -L/usr/local/lib64/ -lgdal -losgEarth -losgEarthFeatures -losgEarthUtil -losgEarthSymbology -losgEarthAnnotation
#unix:!macx: LIBS += -L$$PWD/../../osgEarth/build/lib/ -losgEarthFeatures

#unix:!macx: LIBS += -L$$PWD/../../OpenSceneGraph/build/lib/ -losgDB
#INCLUDEPATH += $$PWD/../../OpenSceneGraph/OpenSceneGraph/include
#DEPENDPATH += $$PWD/../../OpenSceneGraph/OpenSceneGraph/include

unix:!macx: LIBS += -L$$PWD/../../osgQt/build-topic-Qt4/lib/ -losgQt5

INCLUDEPATH += $$PWD/../../osgQt/osgQt-topic-Qt4/include
DEPENDPATH += $$PWD/../../osgQt/osgQt-topic-Qt4/include

RESOURCES +=

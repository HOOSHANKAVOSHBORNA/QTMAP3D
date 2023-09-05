import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Crystal 1.0
import "style"


MapController {
    id: map
    zoomInButtonPressed:      mapViewController.zoomInButtonPressed
    zoomOutButtonPressed:     mapViewController.zoomOutButtonPressed

    moveXY: mapViewController.moveXY
    rotateXY: mapViewController.rotateXY

    anchors.fill: parent
    objectName: "MainMap"
    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    readonly property int       _iconSize        : 24
    readonly property int       _margin          : 15
    readonly property int       _radius          : 10
    readonly property color     _colorRec        : "#404040"
    readonly property color     _colorHover      : "#01AED6"
    readonly property color     _colorPresed     : "#003569"
    readonly property color     _colorIcon       : "#DEE3E6"
    readonly property color     _colorButton     : "#55FFFFFF"
    readonly property string    _fontFamily      : "Srouce Sans Pro"
    readonly property int       _fontPointSize   : 11
    readonly property color     itemColor        : "#404040"
    readonly property real      widgetsMargins   : 10
    clip: true

    onClicked: function() {
        toggleWidgetsVisible();
    }

    function toggleWidgetsVisible() {
        if (map.widgetsVisible === true) {
            widgetsShowAnimation.stop();
            widgetsHideAnimation.start();
            map.widgetsVisible = false;
        } else {

            widgetsHideAnimation.stop();
            widgetsShowAnimation.start();
            map.widgetsVisible = true;
        }
    }

    PropertyAnimation {
        id: widgetsShowAnimation
        target: map
        property: "widgetsPositionFactor"
        from: map.widgetsPositionFactor
        to: 1.0
        duration: 200 * Math.abs(1.0 - map.widgetsPositionFactor)

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: widgetsHideAnimation
        target: map
        property: "widgetsPositionFactor"
        from: map.widgetsPositionFactor
        to: 0.0
        duration: 200 * Math.abs(map.widgetsPositionFactor)

        easing.type: Easing.InQuint
    }
    //
    Label {
        id: fpsLabel
        text: map.fps.toLocaleString(Qt.locale(), 'f', 2)
        color: 'springgreen'
        style: Text.Outline;
        styleColor: "black"
        font.pointSize: 20
        font.weight: Font.Bold
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.leftMargin: 20
    }
    SearchBar {
        id:searcbarContainer
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.rightMargin: 20
        anchors.right: parent.right
        model: map.searchNodeProxyModel()
    }
    MultiEffect {
        source: searcbarContainer
        enabled: true
        anchors.fill: searcbarContainer
        shadowColor: "black"
        shadowEnabled: true
        shadowBlur: 0.6
        shadowHorizontalOffset: 3.5
        shadowVerticalOffset:2.5
        shadowOpacity:0.35
        paddingRect: Qt.rect(0,0,20,20)
        shadowScale: 0.98
    }
    Rectangle {
        color: "transparent"
        id:compass
        anchors.left: parent.left
        anchors.leftMargin: widgetsMargins
        anchors.bottomMargin: widgetsMargins
        height: compassWidget.height+10
        width: compassWidget.width
        y: parent.height  - widgetsPositionFactor * (compassWidget.height + (widgetsMargins) + statusBar.height)

        Drag.hotSpot.x: 32
        Drag.hotSpot.y: 32


        Compass {
            id: compassWidget
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10

            headingAngle:  map.compassDirection.x
            pitchAngle: map.compassDirection.y+90

            onCompassClicked: function() {
                console.log("headingAngle: " + map.compassDirection.x + ", pitch: " + map.compassDirection.y)
                print(Screen.desktopAvailableHeight )

            }
        }
    }


    MapViewController{
        id : mapViewController

        anchors.rightMargin: widgetsMargins

        y: parent.height/2
        x:parent.width - widgetsPositionFactor * (width + widgetsMargins)
        onBtnHomeClicked: function() {
            map.home();
        }
        onBtnProjectionClicked: function() {
            map.changeMode();
        }
    }

    StatusBar {
        id: statusBar
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        width: parent.width
        height: childrenRect.height
        latitude: map.mapMouseGeoLocation.x
        longitude: map.mapMouseGeoLocation.y
        altitude: map.mapMouseGeoLocation.z
        coordinate1: map.mapMouseLocation.x
        coordinate2: map.mapMouseLocation.y
        coordinate3: map.mapMouseLocation.z
        message: "Ready"
        timer: -1

    }

}


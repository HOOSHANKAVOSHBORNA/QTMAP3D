import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Crystal 1.0
import "style"


MapController {
    id: map
    zoomInButtonPressed:      cameraHandlerItem.zoomInButtonPressed
    zoomOutButtonPressed:     cameraHandlerItem.zoomOutButtonPressed

    movePosition: cameraHandlerItem.movePosition
    rotatePosition: cameraHandlerItem.rotatePositon

    anchors.fill: parent
    objectName: "MainMap"
    property real itemPositionFactor: 1.0
    property bool itemVisible: true
    readonly property int       iconSize      : 26/Style.monitorRatio
    readonly property real      itemMargin   : 10

    onClicked: function() {
        toggleItemsVisible();
    }

    function toggleItemsVisible() {
        if (map.itemVisible === true) {
            itemsShowAnimation.stop();
            itemsHideAnimation.start();
            map.itemVisible = false;
        } else {

            itemsHideAnimation.stop();
            itemsShowAnimation.start();
            map.itemVisible = true;
        }
    }

    PropertyAnimation {
        id: itemsShowAnimation
        target: map
        property: "itemPositionFactor"
        from: map.itemPositionFactor
        to: 1.0
        duration: 200 * Math.abs(1.0 - map.itemPositionFactor)

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: itemsHideAnimation
        target: map
        property: "itemPositionFactor"
        from: map.itemPositionFactor
        to: 0.0
        duration: 200 * Math.abs(map.itemPositionFactor)

        easing.type: Easing.InQuint
    }
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
        id:searcbar
        x: parent.width - itemPositionFactor * (width + itemMargin)
        anchors.top: parent.top
        anchors.topMargin: itemMargin
        model: map.searchNodeProxyModel()
    }
    MultiEffect {
        source: searcbar
        enabled: true
        anchors.fill: searcbar
        shadowColor: "black"
        shadowEnabled: true
        shadowBlur: 0.6
        shadowHorizontalOffset: 3.5
        shadowVerticalOffset:2.5
        shadowOpacity:0.35
        shadowScale: 0.98
    }
    Compass {
        id: compassItem
        anchors.left: parent.left
        anchors.leftMargin: itemMargin
        anchors.bottomMargin: itemMargin
        y: parent.height  - itemPositionFactor * (compassItem.height + (itemMargin) + statusBar.height)
        headingAngle:  map.compassDirection.x
        pitchAngle: map.compassDirection.y+90
        color: mouseArea.hovered ? Style.selectColor: Style.backgroundColor
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onDoubleClicked: {
                map.setHeadingToNorth()
            }
            hoverEnabled: true
            property bool hovered: false
            onEntered: hovered = true
            onExited: hovered = false
        }
    }
    MultiEffect {
        source: compassItem
        enabled: true
        anchors.fill: compassItem
        shadowColor: "black"
        shadowEnabled: true
        shadowBlur: 0.6
        shadowHorizontalOffset: 3.5
        shadowVerticalOffset:2.5
        shadowOpacity:0.35
        shadowScale: 0.98
    }
    CameraHandlerItem{
        id : cameraHandlerItem
        anchors.rightMargin: itemMargin
        y: parent.height/2
        x:parent.width - itemPositionFactor * (width + itemMargin)
        onBtnHomeClicked: function() {
            map.home();
        }
        onBtnProjectionClicked: function() {
            map.changeMode();
        }
    }
//    SmallMap {
//        width: 100
//        height: 100
//        x: parent.width - (100 + itemMargin)
//        y: parent.height - itemPositionFactor * (100 + itemMargin + statusBar.height)
////        anchors.rightMargin: 10
////        anchors.bottomMargin: 50
//        objectName: "SmallMap"
//    }

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


import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import "style"

Item {
    id: map

    property var mapItem

    property bool itemVisible: true
    readonly property int iconSize: 26 / Style.monitorRatio
    readonly property real itemMargin: 20 / Style.monitorRatio

    function toggleItemsVisible() {
        if (mapItem.itemVisible === true) {
            itemsShowAnimation.stop()
            itemsHideAnimation.start()
            mapItem.itemVisible = false
        } else {
            itemsHideAnimation.stop()
            itemsShowAnimation.start()
            mapItem.itemVisible = true
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: function () {
            toggleItemsVisible()
        }
    }

    StackLayout {
        id: mapContainer
        anchors.fill: parent
        data: mapItem
    }

    Item {
        id: topMenuContainer
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        visible: mapItem.topMenuVisible
        width: 706
        height: 75

        children: mapItem.topMenuItem ?? []
    }

    //    PropertyAnimation {
    //        id: itemsShowAnimation
    //        target: map
    //        property: "mapItem.itemPositionFactor"
    //        from: mapItem.itemPositionFactor
    //        to: 1.0
    //        duration: 200 * Math.abs(1.0 - mapItem.itemPositionFactor)

    //        easing.type: Easing.OutQuint
    //    }

    //    PropertyAnimation {
    //        id: itemsHideAnimation
    //        target: map
    //        property: "mapItem.itemPositionFactor"
    //        from: mapItem.itemPositionFactor
    //        to: 0.0
    //        duration: 200 * Math.abs(mapItem.itemPositionFactor)

    //        easing.type: Easing.InQuint
    //    }
    Label {
        id: fpsLabel
        text: mapItem.fps.toLocaleString(Qt.locale(), 'f', 2)
        color: 'springgreen'
        style: Text.Outline
        styleColor: "black"
        font.pointSize: 20
        font.weight: Font.Bold
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.leftMargin: 80
    }

    SearchBar {
        id: searcbar

        anchors.right: parent.right
        anchors.rightMargin: itemMargin
        anchors.top: parent.top
        anchors.topMargin: itemMargin + 3
        model: SearchNodeManagerInstance.searchNodeProxyModel()
        filterManager: mapItem.filterManager()
    }

    MultiEffect {
        source: searcbar
        enabled: true
        anchors.fill: searcbar
        shadowColor: "black"
        shadowEnabled: true
        shadowBlur: 0.6
        shadowHorizontalOffset: 3.5
        shadowVerticalOffset: 2.5
        shadowOpacity: 0.25
        shadowScale: 1.04
    }

    Compass {
        id: compassItem
        anchors.left: parent.left
        anchors.leftMargin: itemMargin + 80 / Style.monitorRatio
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2*itemMargin
        headingAngle: mapItem.compassDirection.x
        pitchAngle: mapItem.compassDirection.y + 90
        color: mouseArea.hovered ? Style.selectColor : Style.backgroundColor

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onDoubleClicked: {
                mapItem.setHeadingToNorth()
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
        shadowVerticalOffset: 2.5
        shadowOpacity: 0.35
        shadowScale: 0.98
    }

    //  zoomInButtonPressed: cameraHandlerItem.zoomInButtonPressed
    //    zoomOutButtonPressed: cameraHandlerItem.zoomOutButtonPressed

    //    movePosition: cameraHandlerItem.movePosition
    //    rotatePosition: cameraHandlerItem.rotatePositon
    CameraHandlerItem {
        id: cameraHandlerItem
        //        anchors.fill: parent
        //        anchors.rightMargin: itemMargin
        //        y: parent.height / 2
        //        x: parent.width - mapItem.itemPositionFactor * (width + itemMargin)
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: itemMargin


        onZoomInButtonPressedChanged: mapItem.zoomInButtonPressed = zoomInButtonPressed
        onZoomOutButtonPressedChanged: mapItem.zoomOutButtonPressed = zoomOutButtonPressed

        onMovePositionChanged: mapItem.movePosition = movePosition
        onRotatePositonChanged: mapItem.rotatePosition = rotatePositon

        width: 160
        height: 200

        onBtnHomeClicked: function () {
            mapItem.home()
        }

        onBtnProjectionClicked: function () {
            mapItem.changeMode()
        }
    }

    //    Rectangle {
    //        id: recct
    //        width: 100
    //        height: 100
    //        x: parent.width - (100 + itemMargin)
    //        y: parent.height - (100 + itemMargin + statusBar.height)
    //        opacity: 0.5
    //        radius: 50
    ////        layer.enabled: true
    //        layer.effect: OpacityMask {
    //            maskSource: Item {
    //                width: 100
    //                height: 100
    //                Rectangle {
    //                    anchors.centerIn: parent
    //                    width: recct.adapt ? recct.width : Math.min(recct.width, recct.height)
    //                    height: recct.adapt ? recct.height : width
    //                    radius: 50
    //                }
    //            }
    //        }
    //    SmallMap {
    //        id: miniMap
    //        objectName: "SmallMap"
    //        //        x: mapItem.width - 100-21
    //        //        y: mapItem.height - 120-20
    //        anchors.right: cameraHandlerItem.right
    //        anchors.bottom: compassItem.bottom
    //        width: 110
    //        height: 110
    //        Component.onCompleted: {
    //            setMainMapItem(map)
    //        }
    //        Rectangle {
    //            anchors.centerIn: parent
    //            width: parent.width / 15
    //            height: width
    //            color: "transparent"
    //            border.color: "#01AED6"
    //            border.width: 2
    //        }
    //    }
    StatusBar {
        id: statusBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 75 / Style.monitorRatio
        anchors.rightMargin: 0
        width: parent.width
        height: 22 / Style.monitorRatio
        latitude: mapItem.mapMouseGeoLocation.x
        longitude: mapItem.mapMouseGeoLocation.y
        altitude: mapItem.mapMouseGeoLocation.z
        coordinate1: mapItem.mapMouseLocation.x
        coordinate2: mapItem.mapMouseLocation.y
        coordinate3: mapItem.mapMouseLocation.z
        message: "Ready"
        timer: -1
        model: mapItem.statusBar()
        sourceModel: mapItem.statusBar().getSourceModel()
    }
}

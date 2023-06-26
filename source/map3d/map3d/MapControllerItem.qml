import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Crystal 1.0


MapController {
    id: map
    zoomInButtonPressed:      navigationWidget.zoomInButtonPressed
    zoomOutButtonPressed:     navigationWidget.zoomOutButtonPressed
    upButtonPressed:          navigationWidget.upButtonPressed
    downButtonPressed:        navigationWidget.downButtonPressed
    leftButtonPressed:        navigationWidget.leftButtonPressed
    rightButtonPressed:       navigationWidget.rightButtonPressed
    rotateUpButtonPressed:    navigationWidget.rotateUpButtonPressed
    rotateDownButtonPressed:  navigationWidget.rotateDownButtonPressed
    rotateLeftButtonPressed:  navigationWidget.rotateLeftButtonPressed
    rotateRightButtonPressed: navigationWidget.rotateRightButtonPressed
    anchors.fill: parent
    objectName: "MainMap"
    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    readonly property int       _iconSize   : 24
    readonly property int       _margin     : 15
    readonly property int       _radius     : 10
    readonly property color     _colorRec   : "#404040"
    readonly property color     _colorHover : "#FFCC00"
    readonly property color     _colorPresed : "#908000"
    readonly property color     _colorIcon  : "#FFFFFF"
    readonly property color     _colorButton: "#55FFFFFF"
    readonly property string    _fontFamily : "Srouce Sans Pro"
    readonly property int       _fontPointSize : 11
    readonly property color     itemColor: "#404040"
    readonly property real      widgetsMargins: 10
    DropArea {
        id: dragTarget

        property string colorKey

        width: 64
        height: 64
        keys: [ colorKey ]

        Rectangle {
            id: dropRectangle

            anchors.fill: parent
            color: dragMouseArea.drag.active ? (dragTarget.containsDrag ? "grey" : dragTarget.colorKey) : "transparent"
        }
    }
    //    MouseArea {
    //        id: mouseArea

    //        width: 64
    //        height: 64
    //        anchors.centerIn: parent

    //        drag.target: tile

    //        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : rootItem

    //        Rectangle {
    //            id: tile

    //            width: 64
    //            height: 64
    //            anchors {
    //                verticalCenter: parent.verticalCenter
    //                horizontalCenter: parent.horizontalCenter
    //            }

    //            color: "#505050"

    //            Drag.keys: [ rootItem.colorKey ]
    //            Drag.active: mouseArea.drag.active
    //            Drag.hotSpot.x: 32
    //            Drag.hotSpot.y: 32
    //            states: State {
    //                when: mouseArea.drag.active
    //                AnchorChanges {
    //                    target: tile
    //                    anchors {
    //                        verticalCenter: undefined
    //                        horizontalCenter: undefined
    //                    }
    //                }
    //            }
    //        }
    //    }

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
    //        id:mapItem
    Rectangle {
        color: "transparent"
        id:compass
        anchors.left: parent.left
        anchors.leftMargin: widgetsMargins
        anchors.bottomMargin: widgetsMargins
        height: compassWidget.height+10
        width: compassWidget.width
        y: parent.height  - widgetsPositionFactor * (compassWidget.height + (widgetsMargins) + statusBar.height)
        Drag.active: dragMouseArea.drag.active
        Drag.hotSpot.x: 32
        Drag.hotSpot.y: 32
        states: State {
            when: dragMouseArea.drag.active
            AnchorChanges {
                target: compass
                anchors {
                    verticalCenter: undefined
                    horizontalCenter: undefined
                }
            }
        }
        MouseArea {
            id: dragMouseArea

            width: 64
            height: 64
            anchors.fill: dragImg

            drag.target: compass

            onReleased: parent = compass.Drag.target !== null ? compass.Drag.target : rootItem
        }
        Image {
            id: dragImg
            source: "qrc:/Resources/drag.png"
            anchors.top: parent.top
            anchors.left: parent.left
            width: 14
            height: 14
        }

        Compass{
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            id:compassWidget
            headingAngle: map.headingAngle
            //            anchors.left: parent.left
            //            anchors.leftMargin: widgetsMargins
            //            anchors.bottomMargin: widgetsMargins
            //            y: parent.height  - widgetsPositionFactor * (height + (widgetsMargins) + statusBar.height)

            onCompassDoubleClicked: function() {
                map.orientCameraToNorth();
            }
        }
    }


    NavigationWidget{
        id : navigationWidget
        //anchors.right: parent.right
        anchors.rightMargin: widgetsMargins
        //            y:25 + parent.height  - (wnd.widgetsPositionFactor * (height + ((widgetsMargins)/2+3)+25))
        //y: parent.height  - widgetsPositionFactor * (height + (widgetsMargins/2+3) + statusBar.height)
        // slot button
        y: parent.height/2
        x:parent.width - widgetsPositionFactor * (width + widgetsMargins)
        onBtnHomeClicked: function() {
            map.goToHome();
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

    //    }
}


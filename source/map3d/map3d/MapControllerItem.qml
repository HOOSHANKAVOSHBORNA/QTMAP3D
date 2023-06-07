import QtQuick 2.0
import Crystal 1.0

MapController {
    id: rootItem
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

    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
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
        if (rootItem.widgetsVisible === true) {
            widgetsShowAnimation.stop();
            widgetsHideAnimation.start();
            rootItem.widgetsVisible = false;
        } else {

            widgetsHideAnimation.stop();
            widgetsShowAnimation.start();
            rootItem.widgetsVisible = true;
        }
    }

    PropertyAnimation {
        id: widgetsShowAnimation
        target: rootItem
        property: "widgetsPositionFactor"
        from: rootItem.widgetsPositionFactor
        to: 1.0
        duration: 200 * Math.abs(1.0 - rootItem.widgetsPositionFactor)

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: widgetsHideAnimation
        target: rootItem
        property: "widgetsPositionFactor"
        from: rootItem.widgetsPositionFactor
        to: 0.0
        duration: 200 * Math.abs(rootItem.widgetsPositionFactor)

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
            id:compassWidget
            headingAngle: rootItem.headingAngle
            //            anchors.left: parent.left
            //            anchors.leftMargin: widgetsMargins
            //            anchors.bottomMargin: widgetsMargins
            //            y: parent.height  - widgetsPositionFactor * (height + (widgetsMargins) + statusBar.height)

            onCompassDoubleClicked: function() {
                rootItem.orientCameraToNorth();
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
            rootItem.goToHome();
        }
        onBtnProjectionClicked: function() {
            rootItem.changeMode();
        }

        onBtnProjectionClicked: function() {
            rootItem.changeMode();
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
        latitude: rootItem.mapMouseGeoLocation.x
        longitude: rootItem.mapMouseGeoLocation.y
        altitude: rootItem.mapMouseGeoLocation.z
        coordinate1: rootItem.mapMouseLocation.x
        coordinate2: rootItem.mapMouseLocation.y
        coordinate3: rootItem.mapMouseLocation.z
        message: "Ready"
        timer: -1

    }

    //    }
}

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
//    MapItem {
//        anchors.fill: parent
//        readonly property real      widgetsMargins: 10



//        id:mapItem
        Compass{
            id:compass
            headingAngle: rootItem.headingAngle
            anchors.left: parent.left
            anchors.leftMargin: widgetsMargins
            anchors.bottomMargin: widgetsMargins
            y: parent.height  - (height + (widgetsMargins) + statusBar.height)

            onCompassDoubleClicked: function() {
                rootItem.orientCameraToNorth();
            }
        }


        NavigationWidget{
            id : navigationWidget
            anchors.right: parent.right
            anchors.rightMargin: widgetsMargins
            //            y:25 + parent.height  - (wnd.widgetsPositionFactor * (height + ((widgetsMargins)/2+3)+25))
            y: parent.height  - (height + (widgetsMargins/2+3) + statusBar.height)
            // slot button
            onBtnHomeClicked: function() {
                rootItem.goToHome();
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

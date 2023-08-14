import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: rootPopUp
    title: "SnapShot Taken"
    width: Screen.desktopAvailableHeight
    height: Screen.desktopAvailableHeight
    y: 0
    x: 0
    flags: Qt.FramelessWindowHint

    color: "white"

    Rectangle{
        anchors.fill: parent
        color: "white"
    }

//    property string imagePath;

//    Item{
//        id:rootItem
//        anchors.fill: parent

//        Image {
//            id: img
//            source: imagePath
//            width: parent.width
//            height: parent.height*0.9
//        }
//        Rectangle{
//            width: parent.width
//            height: parent.height*0.1
//            color: "#323539"
//            anchors.bottom: parent.bottom
//            Text {
//                text: "SnapShot Saved in: "
//                color: "white"
//                anchors.top:  parent.top
//                anchors.horizontalCenter: parent.horizontalCenter
//            }
//            Text{
//                text:  imagePath
//                color: "white"
//                anchors.bottom:  parent.bottom
//                anchors.horizontalCenter: parent.horizontalCenter
//            }
//        }
//    }
//    PropertyAnimation{running: true; id: widthAnimation; target: rootPopUp;property: "width"; to: 30;duration: 2000 ;easing.type: Easing.InOutQuad }
//    PropertyAnimation{running: true; id: heightAnimation;target: rootPopUp;property: "height";to: 30;duration: 2000 ;easing.type: Easing.InOutQuad }

}





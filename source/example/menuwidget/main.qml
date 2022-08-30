import QtQuick 2.13
import QtQuick.Window 2.0

Item {
    width: anchors.width
    height: anchors.height
//    ButtonMenu{
//        id:button
//        anchors.left: parent.left
//        anchors.leftMargin: 5
//        anchors.verticalCenter: parent .verticalCenter
//        getMouseArea.onClicked: {
//            if(menu.state==="close"){
//            menu.getstart.start();
//            menu.state="open";
//             button.visible=false
//            }
//        }
//    }
    MenuSlide{
        id:menu
        width: parent.width/3
        height: parent.height
    }

}

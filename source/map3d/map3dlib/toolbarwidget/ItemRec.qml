import QtQuick 2.0
import QtQuick.Layouts 1.12

Rectangle {
    id: rectangle
    property var category
    property var name:"item"
    property string icon :"qrc:/res/icon/puzzle.png"
   width: 220
   height: 22
   color:"transparent"
   Layout.alignment: Qt.AlignHCenter
   Image {

       width: 20
       height: 20
       anchors.verticalCenter: parent.verticalCenter
       id:image
       anchors.left: parent.left
       anchors.leftMargin: 5
       source:icon
   }
   RotationAnimation{
       id:anim
       targets: image
       from :-60
       to:30
       duration: 200
       running: false
       loops: Animation.Infinite


   }
   Text {
       id:txt
       anchors.left: image.right
       anchors.leftMargin: 8
       text : String(name)
       anchors.verticalCenter: parent.verticalCenter
       width: parent.width - image.width
       //horizontalAlignment: Text.AlignHCenter
       font.pointSize: 10
       color: "white"
   }
   MouseArea{
       anchors.fill : parent
       hoverEnabled: true
       onEntered:{
           txt.font.bold =true
           anim.running =true
       }
       onExited: {
           txt.font.bold =false
           anim.running =false
       }

       onClicked: {
           NamePlugin.onGetItemClicked(category,name)
       }
   }
}

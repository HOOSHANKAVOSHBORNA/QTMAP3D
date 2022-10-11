import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
Rectangle {
    property string title

    height:120
    width: 200
    color: "#22000000"
    Connections {
        target: DetaliObject
        onModelPosition:{
            if (name === n && type === t){
            element.text = String(latitude + " , " + longitude + " , " + altitude)
            }
        }
        onModelInfo:{
            if(name === n && type === t)
            textarea.text = str
        }
    }
    Rectangle {
        id: long_info
        color: "transparent"
        radius: 3
        width: parent.width
        height: 20
        anchors.top:parent.top
        anchors.topMargin: 3
        anchors.left: parent.left
        anchors.leftMargin: 2

        Text {
            id: element
            color: "#fdfdfd"
            padding:4

            text: ""
            font.family: "Times New Roman"
            font.weight: Font.Normal
            style: Text.Normal
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            anchors.fill: parent
            font.pointSize: textsize

        }

    }

    Rectangle {
        id: info
        //color: "#006efa"
        color: "lightgray"
        layer.smooth: true
        width: parent.width -10
        height: parent.height -30
        anchors.top: long_info.bottom
        anchors.topMargin: 3
        anchors.left: parent.left
        anchors.leftMargin: 5
        radius: 5
        Flickable {
             id: flickable
             anchors.fill: parent

             TextArea.flickable: TextArea {
                 id:textarea
                 readOnly: true
                 font.family: "Times New Roman"
                 font.weight: Font.Normal
                 font.pointSize: textsize
                 text:""
                 color: "black"
                 wrapMode: TextArea.Wrap

             }

             ScrollBar.vertical: ScrollBar { }
         }

    }
}

/*##^##
Designer {
    D{i:2;anchors_x:20;anchors_y:4}D{i:4;anchors_x:20;anchors_y:4}D{i:6;anchors_x:20;anchors_y:4}
D{i:8;anchors_x:63;anchors_y:16}
}
##^##*/

import QtQuick 2.12
import QtQuick.Layouts 1.12

Item {
    id:root
    property Component content
    property string title: "panel"
    property bool isSelected: false
    height: container.height + bar.height

    Rectangle {
        id: bar
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: 30
        radius: 5
        color:  root.current ? "#55000000" : "#88000000"
        Text {
            anchors.fill: parent
            anchors.margins: 10
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: root.title
            color: "white"
        }

        Image {
            source: "qrc:/res/down-r.png"
            width: 16
            height: 16
            anchors.right: parent.right
            anchors.rightMargin:  10
            anchors.verticalCenter: parent.verticalCenter
            rotation: root.isSelected ? "180" :0
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: isSelected=!isSelected

        }
    }
    Rectangle {
        id: container
        color: "#88000000"
        border.color: "black"
        border.width: 1
        radius: 5
        anchors.top: bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: loader.item && isSelected ? loader.item.height : 0
        Loader{
            id:loader
            visible: isSelected
            sourceComponent: content
            anchors.top: container.top
        }

        Behavior on height {
            PropertyAnimation { duration: 50; }
        }
    }

}


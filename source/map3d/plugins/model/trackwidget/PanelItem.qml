import QtQuick 2.12
import QtQuick.Layouts 1.12

Item {
    id:root
    property string title: "panel"
    property string name
    property string type
    property bool isSelected: false
    property bool selected: false
    height: container.height + bar.height


    Rectangle {

        id: bar
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
            leftMargin: 2
            rightMargin: 2
        }

        height: 22
        color:  "#404142"
        radius: 2
        Text {
            anchors.fill: parent
            font.family: "Times New Roman"
            font.weight: Font.Normal
            style: Text.Normal
            anchors.margins: 10
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: root.title
            color: "white"
            font.pointSize: textsize

        }
        Image {
            source: "qrc:/res/sort-down.png"
            width: iconsize2
            height: iconsize2
            anchors.right: parent.right
            anchors.rightMargin:  10
            anchors.verticalCenter: parent.verticalCenter
            rotation: root.isSelected ? "180" :0
            z:1;
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: isSelected=!isSelected

            }
        }
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                for (var index in listobject){
                    if ((listobject[index].type + " : " + listobject[index].name) === type+ " : "+ name){
                        if (!selected){
                             listobject[index].children[0].color = "#006eff"
                            listobject[index].selected = true

                            DetaliObject.onModelClicked(type,name, selected)
                        }else{
                            bar.color = "#404142"
                            selected = false
                            DetaliObject.onModelClicked(type,name, selected)
                        }
                    }else{

                        listobject[index].children[0].color = "#404142"
                        listobject[index].selected = false
                    }
                }
            }
            onEntered: bar.color = "#33006eff"
            onExited:{
                bar.color = "#404142"
                if(selected){
                    bar.color = "#006eff"
                }
            }
        }

    }
    Rectangle {
        id: container
        color: "#282A31"
        anchors.top: bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        radius: 2
        height: loader.item && isSelected ? loader.item.height : 0
        Loader{
            id:loader
            visible: isSelected
            sourceComponent: Panel{title:title}
            anchors.top: container.top

        }

        Behavior on height {
            PropertyAnimation { duration: 50; }
        }
    }

}


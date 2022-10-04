import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle{
    id: rectangle
    width: 100
    height: 80
    radius: 5
    border.width: 1
    border.color: "black"
    color: "#282A31"
    clip: true
    Column{
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 4
        spacing: 2
        TextField{
            id:lat
            anchors.left:parent.left
            anchors.leftMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 3
            height: 18
            placeholderText: "latitude"
            color: "black"
            rightPadding: 0
            bottomPadding: 0
            topPadding: 0
            readOnly: false

            leftPadding: 5
            font.pointSize: 10
            background: Rectangle{
                color: "lightgray"
                radius: 5
            }
        }


        TextField{
            id:lon
            placeholderText: "longitude"
            anchors.left:parent.left
            anchors.leftMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 3
            height: 18
            leftPadding: 5
            color: "black"
            rightPadding: 0
            bottomPadding: 0
            topPadding: 0
            font.pointSize: 10
            background: Rectangle{
                color: "lightgray"
                radius: 5
            }
        }

        TextField{
            id:rang
            placeholderText: "range"
            anchors.left:parent.left
            anchors.leftMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 3
            height: 18
            leftPadding: 5
            color: "black"
            rightPadding: 0
            bottomPadding: 0
            topPadding: 0
            font.pointSize: 10
            background: Rectangle{
                color: "lightgray"
                radius: 5
            }
        }
        Rectangle{
            id :rec
            width: 30
            height: 15
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#88000000"
            radius: 10
            Text {
                anchors.centerIn: parent
                text: qsTr("GO")
                font.pointSize: 10
                color: "white"

            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    Location.goPosition(parseFloat(lat.text),parseFloat(lon.text), parseFloat(rang.text))
                }
                onEntered : {
                    rec.color = "#88006eff"
                }
                onExited: {
                    rec.color = "#88000000"
                }
            }
        }
    }
}

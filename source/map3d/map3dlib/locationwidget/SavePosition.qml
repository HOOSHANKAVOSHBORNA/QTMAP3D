import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle{
    id: rectangle
    width: 150
    height: 40
    radius: 5
    border.width: 1
    border.color: "black"
    color: "#282A31"
    clip: true
    Connections{
        target: Location
        onCurrentLocation:{
            lat.text = "Lat: " + latitude
            lon.text ="Lon: " + longitude
        }
    }

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
            id:name
            anchors.left:parent.left
            anchors.leftMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 3
            height: 18
            placeholderText: "name"
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
        Rectangle{
            anchors.left:parent.left
            anchors.leftMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 3
            height: 18
            radius: 5
            color: "#55000000"
            Text {
                id: lat
                text: ""
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 10
                leftPadding: 2
                color: "lightgray"
            }
        }
        Rectangle{
            anchors.left:parent.left
            anchors.leftMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 3
            height: 18
            radius: 5
            color: "#55000000"
            Text {
                id: lon
                text: ""
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 10
                leftPadding: 2
                color: "lightgray"
            }
        }
        Rectangle{
            id :rec
            width: 40
            height: 15
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#88000000"
            radius: 10
            Text {
                anchors.centerIn: parent
                text: qsTr("Save")
                font.pixelSize: 10
                color: "lightgray"

            }

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    opensave.start();
                    isshowcurrnt = true
                    closesavelocation.start()
                    isshowsave  = false
                    Location.onOpenWidget(isshow, isshowcurrnt, isshowsave)
                    Location.sendNamePosition(name.text)
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

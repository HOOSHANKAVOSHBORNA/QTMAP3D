import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {

    property bool changeMap


    id: rectangle
    height: 60
    width: 180
    color: "transparent"


    MouseArea{
        id: mouseArea
        width: 80
        height: 200
        anchors.rightMargin: 0
        anchors.fill: parent
        hoverEnabled: true



    }

    Rectangle {
        id: rectangle3
        width: 20
        radius: 100
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 5
        z:1
        color: "#88000000"
        Image {
            id:homemap
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 15
            height: 15

            source: "qrc:/image/icons8-home-96_w.png"
        }
        MouseArea{
            hoverEnabled: true
            id: homemouse
            anchors.fill: parent
            onPressed: homemap.source="qrc:/image/icons8-home-96.png"
            onReleased: homemap.source="qrc:/image/icons8-home-96_w.png"
            onClicked: {
                GetData.onHomeClicked()
            }

        }
    }


    Rectangle {
        id: rectangle1

        color: "#88000000"
        radius: 58.5
        anchors.right: rectangle4.left
        anchors.rightMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: rectanglezoom.right
        anchors.leftMargin: 5
        Rectangle {
            id: rectangle2
            anchors.centerIn: parent
            width: 12
            height: 12
            color: "#243949"
            radius: 17
        }
        Image {
            id: imageup
            anchors.top: parent.top
            anchors.topMargin: 2
            anchors.horizontalCenter: parent.horizontalCenter
            width: 12
            height: 12
            source: "qrc:/image/icons8-collapse-arrow-96_w.png"
            fillMode: Image.PreserveAspectFit
            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {

                    GetData.onUPClicked()

                }
                onPressed: imageup.source= "qrc:/image/icons8-collapse-arrow-96.png"
                onReleased: imageup.source= "qrc:/image/icons8-collapse-arrow-96_w.png"



            }
        }

        Image {
            id: imagedown
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2
            anchors.horizontalCenter: parent.horizontalCenter
            width: 12
            height: 12
            source: "qrc:/image/icons8-expand-arrow-96_w.png"
            fillMode: Image.PreserveAspectFit
            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    GetData.onDownClicked()
                }

                onPressed: imagedown.source= "qrc:/image/icons8-expand-arrow-96.png"
                onReleased: imagedown.source= "qrc:/image/icons8-expand-arrow-96_w.png"



            }
        }
        Image {
            id: imagef

            anchors.right: parent.right
            anchors.rightMargin:  2
            anchors.verticalCenter: parent.verticalCenter
            width: 12
            height: 12
            source: "qrc:/image/icons8-forward-96_w.png"
            fillMode: Image.PreserveAspectFit
            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    GetData.onRightClicked()
                }

                onPressed: imagef.source= "qrc:/image/icons8-forward-96.png"
                onReleased: imagef.source= "qrc:/image/icons8-forward-96_w.png"



            }
        }

        Image {
            id: imageb
            anchors.left: parent.left
            anchors.leftMargin: 2
            anchors.verticalCenter: parent.verticalCenter
            width: 12
            height: 12
            source: "qrc:/image/icons8-back-96_w.png"
            fillMode: Image.PreserveAspectFit
            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {

                    GetData.onLeftClicked()
                }

                onPressed: imageb.source= "qrc:/image/icons8-back-96.png"
                onReleased: imageb.source = "qrc:/image/icons8-back-96_w.png"

            }
        }
    }





    Rectangle {
        id: rectangle1d

        width: 40
        color: "#88000000"
        radius: 58.5
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: rectangle3.right
        anchors.leftMargin: 5
        Rectangle {
            id: rectangle2d
            anchors.centerIn: parent
            width: 12
            height: 12
            color: "#243949"
            radius: 17
        }
        Image {
            id: imageup_down
            anchors.top: parent.top
            anchors.topMargin: 2
            anchors.horizontalCenter: parent.horizontalCenter
            width: 12
            height: 12
            source: "qrc:/image/icons8-undo-96_w.png"
            fillMode: Image.PreserveAspectFit
            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {

                    GetData.onHeadUpClicked()
                }
                onPressed: imageup_down.source= "qrc:/image/icons8-undo-96.png"
                onReleased: imageup_down.source= "qrc:/image/icons8-undo-96_w.png"



            }
        }

        Image {
            id: imagedown_down
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2
            anchors.horizontalCenter: parent.horizontalCenter
            width: 12
            height: 12
            source: "qrc:/image/icons8-undo-96_w.png"
            rotation: 180
            fillMode: Image.PreserveAspectFit
            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    GetData.onHeadDownClicked()
                }
                onPressed: imagedown_down.source= "qrc:/image/icons8-undo-96.png"
                onReleased: imagedown_down.source= "qrc:/image/icons8-undo-96_w.png"



            }
        }
        Image {
            id: imagef_down
            anchors.right: parent.right
            anchors.rightMargin:  2
            anchors.verticalCenter: parent.verticalCenter
            width: 10
            height: 10
            source: "qrc:/image/icons8-rotate-right-96_w.png"
            fillMode: Image.PreserveAspectFit
            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    GetData.onPitchUpClicked()
                }
                onPressed: imagef_down.source= "qrc:/image/icons8-rotate-right-96.png"
                onReleased: imagef_down.source= "qrc:/image/icons8-rotate-right-96_w.png"


            }
        }

        Image {
            id: imageb_down
            anchors.left: parent.left
            anchors.leftMargin: 2
            anchors.verticalCenter: parent.verticalCenter
            width: 10
            height: 10
            source: "qrc:/image/icons8-rotate-left-96_w.png"
            fillMode: Image.PreserveAspectFit
            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    GetData.onPitchDownClicked()
                }

                onPressed: imageb_down.source= "qrc:/image/icons8-rotate-left-96.png"
                onReleased: imageb_down.source = "qrc:/image/icons8-rotate-left-96_w.png"

            }
        }
    }

    Rectangle {
        id: rectanglezoom
        width: 18
        height: 18
        color: "#88000000"
        radius: 10
        anchors.left: rectangle1d.right
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 10
        Image {
            id:imagezoomin
            width: 16
            height: 16
            anchors.centerIn : parent
            source: "qrc:/image/icons8-plus-math-96w.png"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    GetData.onZoomInClicked()

                }
                onPressed: imagezoomin.source= "qrc:/image/icons8-plus-math-96.png"
                onReleased: imagezoomin.source = "qrc:/image/icons8-plus-math-96w.png"

            }
        }
    }

    Rectangle {
        id: rectanglezoomout
        width: 18
        height: 18
        color: "#88000000"
        radius: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: rectangle1d.right
        anchors.leftMargin: 5
        anchors.top: rectanglezoom.bottom
        anchors.topMargin: 5
        Image {
            id:imagezoomout
            width: 16
            height: 16
            anchors.centerIn: parent
            source: "qrc:/image/icons8-minus-96w.png"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    GetData.onZoomOutClicked()

                }
                onPressed: imagezoomout.source= "qrc:/image/icons8-minus-96.png"
                onReleased: imagezoomout.source ="qrc:/image/icons8-minus-96w.png"
            }
        }
    }

    Rectangle {
        id: rectangle4

        width: 20
        radius: 10
        state: "G"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 16
        Text {
            id: txtbutton
            text: qsTr("G")
            anchors.centerIn: parent
            color: "black"
        }
        MouseArea {
                id: button1area
                anchors.fill: parent
                onReleased:{
                    parent.state == "G" ? parent.state = "P" : parent.state = "G"
                    parent.state == "G" ? changeMap=false : changeMap=true
                    GetData.ontToggelMap(changeMap)
                }
            }
            states: [
                State {
                    name: "P"
                    PropertyChanges { target: rectangle4; color: button1area.pressed ? "#000000" : "#000000" }
                    PropertyChanges { target: txtbutton; text: button1area.pressed ? "P" : "P" }
                    PropertyChanges { target: txtbutton; color: button1area.pressed ? "#FFFFFF" : "#FFFFFF" }
                },
                State {
                    name: "G"
                    PropertyChanges { target: rectangle4; color: button1area.pressed ? "#88FFFFFF" : "#88FFFFFF" }
                }
            ]
    }

}

/*##^##
Designer {
    D{i:3;anchors_height:20}D{i:2;anchors_height:42;anchors_y:10}D{i:6;anchors_height:40;anchors_width:40}
D{i:7;anchors_height:49}D{i:14;anchors_height:100}D{i:5;anchors_height:40;anchors_width:40;anchors_y:86}
D{i:16;anchors_height:40;anchors_y:31}D{i:18;anchors_height:50}D{i:17;anchors_height:100}
D{i:20;anchors_height:50}D{i:19;anchors_height:50}D{i:21;anchors_height:50}D{i:24;anchors_height:100}
D{i:23;anchors_height:40;anchors_y:31}D{i:15;anchors_height:40;anchors_y:31}D{i:26;anchors_y:80}
D{i:25;anchors_height:50;anchors_y:80}D{i:29;anchors_height:20;anchors_x:27;anchors_y:109}
D{i:28;anchors_height:20;anchors_x:27;anchors_y:109}D{i:31;anchors_height:40}
}
##^##*/

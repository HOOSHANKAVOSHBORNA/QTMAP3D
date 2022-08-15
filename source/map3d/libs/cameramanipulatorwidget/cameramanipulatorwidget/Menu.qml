import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {

    property bool home: false


    id: rectangle
    height: 180
    width: 60
    color: "transparent"

    MouseArea{
        id: mouseArea
        width: 80
        height: 200
        anchors.fill: parent
        hoverEnabled: true

    }

    Rectangle {
        id: rectangle3
        width: 20
        height: 20
        radius: 100
        z:1
        color: "#88000000"
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenterOffset: 1
        anchors.horizontalCenter: parent.horizontalCenter
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

        width: 40
        height: 40
        color: "#88000000"
        radius: 58.5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: rectanglezoomout.bottom
        anchors.topMargin: 5
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
        anchors.top: rectangle3.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        width: 40
        height: 40
        color: "#88000000"
        radius: 58.5
        anchors.horizontalCenterOffset: 0
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
        width: 23
        height: 23
        color: "#88000000"
        radius: 10
        anchors.top: rectangle1d.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
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
        x: 27
        width: 23
        height: 23
        color: "#88000000"
        radius: 10
        anchors.top: rectanglezoom.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
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

}

/*##^##
Designer {
    D{i:4;anchors_y:86}D{i:6;anchors_height:49}D{i:13;anchors_height:100}D{i:5;anchors_height:49}
D{i:16;anchors_height:100}D{i:17;anchors_height:50}D{i:20;anchors_height:50}D{i:19;anchors_height:50}
D{i:15;anchors_height:100;anchors_y:31}D{i:25;anchors_y:80}D{i:28;anchors_y:109}
}
##^##*/

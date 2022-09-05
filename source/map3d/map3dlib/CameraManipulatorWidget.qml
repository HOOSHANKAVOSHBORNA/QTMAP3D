import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: name
    visible: true
    property bool changeMap ;
    readonly property int _sizeimage: 15
    Rectangle {


        id: rectangle
        height: 60
        width: 180
        color: "transparent"
        anchors.centerIn: parent
        MouseArea{
            id: mouseArea
            anchors.rightMargin: 0
            anchors.fill: parent
            hoverEnabled: true
        }

        Rectangle {
            id: rectangle3
            width: 25
            radius: 100
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 5
            color: "#88000000"
            Image {
                id:homemap
                width: 20
                height: 20
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenterOffset: -1

                source: "qrc:/res/icon/home-r.png"
            }
            MouseArea{
                hoverEnabled: true
                id: homemouse
                anchors.fill: parent
                onPressed: homemap.source="qrc:/res/icon/home-p.png"
                onReleased: homemap.source="qrc:/res/icon/home-r.png"
                onClicked: {
                    GetData.onHomeClicked()
                }

            }
        }


        Rectangle {
            id: rectangle1
            width: 39

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
                width: _sizeimage
                height: _sizeimage
                source: "qrc:/res/icon/up-r.png"
                fillMode: Image.PreserveAspectFit
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: parent
                    Timer {id:tUp
                        interval: 150; running: false; repeat: true
                        onTriggered:  GetData.onUPClicked()
                    }
                    onPressed: {
                        GetData.onUPClicked()
                        tUp.running=true
                        imageup.source= "qrc:/res/icon/up-p.png"
                    }
                    onReleased: {
                        tUp.running=false
                        imageup.source= "qrc:/res/icon/up-r.png"}



                }
            }

            Image {
                id: imagedown
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                width: _sizeimage
                height: _sizeimage
                source: "qrc:/res/icon/down-r.png"
                fillMode: Image.PreserveAspectFit
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: parent
                    Timer {id:tDown
                        interval: 150; running: false; repeat: true
                        onTriggered: GetData.onDownClicked()
                    }
                    onPressed:{
                        GetData.onDownClicked()
                        tDown.running= true
                        imagedown.source= "qrc:/res/icon/down-p.png"}
                    onReleased: {
                        tDown.running=false
                        imagedown.source= "qrc:/res/icon/down-r.png"}
                }
            }
            Image {
                id: imagef

                anchors.right: parent.right
                anchors.rightMargin:  2
                anchors.verticalCenter: parent.verticalCenter
                width: _sizeimage
                height: _sizeimage
                source: "qrc:/res/icon/right-r.png"
                fillMode: Image.PreserveAspectFit
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: parent
                    Timer {id:tRight
                        interval: 150; running: false; repeat: true
                        onTriggered: GetData.onRightClicked()
                    }
                    onPressed: {
                        GetData.onRightClicked()
                        tRight.running=true
                        imagef.source= "qrc:/res/icon/right-p.png"}
                    onReleased: {
                        tRight.running=false
                        imagef.source= "qrc:/res/icon/right-r.png"}
                }
            }

            Image {
                id: imageb
                anchors.left: parent.left
                anchors.leftMargin: 2
                anchors.verticalCenter: parent.verticalCenter
                width: _sizeimage
                height: _sizeimage
                source: "qrc:/res/icon/left-r.png"
                fillMode: Image.PreserveAspectFit
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: parent
                    Timer {id:tBack
                        interval: 150; running: false; repeat: true
                        onTriggered:  GetData.onLeftClicked()
                    }
                    onPressed: {
                        GetData.onLeftClicked()
                        tBack.running=true
                        imageb.source= "qrc:/res/icon/left-p.png"}
                    onReleased:{
                        tBack.running=false
                        imageb.source = "qrc:/res/icon/left-r.png"}

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
                width: _sizeimage
                height: _sizeimage
                color: "#243949"
                radius: 17
            }
            Image {
                id: imageup_down
                anchors.top: parent.top
                anchors.topMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                width: _sizeimage - 3
                height: _sizeimage - 3
                source: "qrc:/res/icon/head-r.png"
                fillMode: Image.PreserveAspectFit
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: parent
                    Timer {id:tHeadUp
                        interval: 150; running: false; repeat: true
                        onTriggered:  GetData.onHeadUpClicked()
                    }

                    onPressed:{
                        GetData.onHeadUpClicked()
                        tHeadUp.running=true
                        imageup_down.source= "qrc:/res/icon/head-p.png"}
                    onReleased: {
                        tHeadUp.running=false
                        imageup_down.source= "qrc:/res/icon/head-r.png"}
                }
            }

            Image {
                id: imagedown_down
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                width: _sizeimage - 3
                height: _sizeimage - 3
                source: "qrc:/res/icon/head-r.png"
                rotation: 180
                fillMode: Image.PreserveAspectFit
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: parent
                    Timer {id:tHeadDown
                        interval: 150; running: false; repeat: true
                        onTriggered:  GetData.onHeadDownClicked()
                    }
                    onPressed:{
                        GetData.onHeadDownClicked()
                        tHeadDown.running=true
                        imagedown_down.source= "qrc:/res/icon/head-p.png"}
                    onReleased: {
                        imagedown_down.source= "qrc:/res/icon/head-r.png"
                        tHeadDown.running=false
                    }
                }
            }
            Image {
                id: imagef_down
                anchors.right: parent.right
                anchors.rightMargin:  2
                anchors.verticalCenter: parent.verticalCenter
                width: _sizeimage -5
                height: _sizeimage -5
                source: "qrc:/res/icon/pitchup-r.png"
                fillMode: Image.PreserveAspectFit
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: parent
                    Timer {id:tPitchup
                        interval: 150; running: false; repeat: true
                        onTriggered:  GetData.onPitchUpClicked()
                    }
                    onPressed: {
                        GetData.onPitchUpClicked()
                        tPitchup.running=true
                        imagef_down.source= "qrc:/res/icon/pitchup-p.png"}
                    onReleased: {
                        tPitchup.running=false
                        imagef_down.source= "qrc:/res/icon/pitchup-r.png"}
                }
            }

            Image {
                id: imageb_down
                anchors.left: parent.left
                anchors.leftMargin: 2
                anchors.verticalCenter: parent.verticalCenter
                width: _sizeimage - 5
                height: _sizeimage -5
                source: "qrc:/res/icon/pitchdown-r.png"
                fillMode: Image.PreserveAspectFit
                Timer {id:ash
                    interval: 150; running: false; repeat: true
                    onTriggered: GetData.onPitchDownClicked()
                }
                MouseArea{
                    hoverEnabled: true
                    anchors.fill: parent
                    onPressed: {
                        GetData.onPitchDownClicked()
                        imageb_down.source= "qrc:/res/icon/pitchdown-p.png"
                        ash.running=true
                    }
                    onReleased: {
                        ash.running=false
                        imageb_down.source = "qrc:/res/icon/pitchdown-r.png"}

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
                width: _sizeimage
                height: _sizeimage
                anchors.centerIn : parent
                source: "qrc:/res/icon/zoomin-r.png"
                Timer {id:tZoom
                    interval: 150; running: false; repeat: true
                    onTriggered:  GetData.onZoomInClicked()
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        GetData.onZoomInClicked()
                        tZoom.running=true
                        imagezoomin.source= "qrc:/res/icon/zoomin-p.png"}
                    onReleased:{
                        tZoom.running=false
                        imagezoomin.source = "qrc:/res/icon/zoomin-r.png"}
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
                width: _sizeimage
                height: _sizeimage
                anchors.centerIn: parent
                source: "qrc:/res/icon/zoomout-r.png"
                Timer {id:tZoomOut
                    interval: 150; running: false; repeat: true
                    onTriggered:    GetData.onZoomOutClicked()
                }
                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        GetData.onZoomOutClicked()
                        tZoomOut.running=true
                        imagezoomout.source= "qrc:/res/icon/zoomout-p.png"}
                    onReleased:{
                        tZoomOut.running=false
                        imagezoomout.source ="qrc:/res/icon/zoomout-r.png"}
                }
            }
        }

        Rectangle {
            id: rectangle4
            x: 147

            width: 25
            radius: 100
            state: "G"
            color: "#88000000"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 8
            Image{
                id: imgbutton
                width: 20

                height: 20
                anchors.horizontalCenterOffset: -0.65
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/res/icon/geocentric.png"
            }


            MouseArea {
                id: button1area
                width: 20
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                anchors.fill: parent
                onReleased:{
                    parent.state == "G" ? parent.state = "P" : parent.state = "G"
                    parent.state == "G" ? changeMap=true : changeMap=false
                    GetData.ontToggelMap(changeMap)
                }


            }
            states: [
                State {
                    name: "P"

                    PropertyChanges { target: imgbutton; source: button1area.pressed ? "qrc:/res/icon/geocentric.png" : "qrc:/res/icon/projection.png" }

                },
                State {
                    name: "G"
                    PropertyChanges { target: rectangle4; color: button1area.pressed ? "#88000000" : "#88000000" }
                }
            ]

            Connections{
                target: GetData
                onStateMapChange:{

                   changeMap =GetData.getStateMap()
                    if(changeMap){

                        rectangle4.state = "G"
                    }
                    else{
                        rectangle4.state = "P"

                    }
                }
            }
        }

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2;anchors_height:42;anchors_y:10}D{i:5;anchors_height:40;anchors_width:40;anchors_y:86}
D{i:3;anchors_height:20}D{i:7;anchors_height:49}D{i:16;anchors_height:40;anchors_y:31}
D{i:15;anchors_height:40;anchors_y:31}D{i:14;anchors_height:100}D{i:19;anchors_height:50}
D{i:18;anchors_height:50}D{i:17;anchors_height:100}D{i:6;anchors_height:40;anchors_width:40}
D{i:21;anchors_height:50}D{i:24;anchors_height:100}D{i:23;anchors_height:40;anchors_y:31}
D{i:26;anchors_y:80}D{i:25;anchors_height:50;anchors_y:80}D{i:29;anchors_height:20;anchors_x:27;anchors_y:109}
D{i:28;anchors_height:20;anchors_x:27;anchors_y:109}D{i:31;anchors_height:40}D{i:20;anchors_height:50}
}
##^##*/

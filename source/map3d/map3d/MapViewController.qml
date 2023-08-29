
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects


Item {
    id:rootItem
    width: mainRowLayout.implicitWidth
    height: mainRowLayout.implicitHeight

    property string modeMap: "geocentric"
    property real positionFactor: 0.0
    property bool showRecMov: true
    property real myDuration: 300.0

    property bool zoomInButtonPressed: positive.pressed
    property bool zoomOutButtonPressed: negative.pressed

    //    property bool upButtonPressed:    moveControlCamera.upButtonPressed
    //    property bool downButtonPressed:  moveControlCamera.downButtonPressed
    //    property bool leftButtonPressed:  moveControlCamera.leftButtonPressed
    //    property bool rightButtonPressed: moveControlCamera.rightButtonPressed

    //    property bool rotateUpButtonPressed:   rotationControlCamera.upButtonPressed
    //    property bool rotateDownButtonPressed: rotationControlCamera.downButtonPressed
    //    property bool rotateLeftButtonPressed: rotationControlCamera.leftButtonPressed
    //    property bool rotateRightButtonPressed:rotationControlCamera.rightButtonPressed


    signal btnHomeClicked()
    signal btnProjectionClicked()

    RowLayout {
        id: mainRowLayout
        anchors.fill: parent
        spacing: 5

        ///////// ------------ Move and Rotation control Area
        /*
          a transparent rectangle holds place for
          Move and Rotation control Button
        */

        ColumnLayout{
            id: control
            spacing: 5

            //            Layout.fillWidth: true
            //            Layout.fillHeight: true
            //            Layout.alignment:  Qt.AlignVCenter



            clip: true
            ControlCamera{
                id: rotationControlCamera
                width: 65
                height: 65

                x: positionFactor * 65
//                anchors.top: parent.top
//                anchors.topMargin: 17
                centerIcon: "qrc:/Resources/eye.png"

            }


            //            ///////// -------------------  rotation control

            //            Rectangle{
            //                id: rotationControlCamera
            //                Layout.preferredWidth:65
            //                Layout.preferredHeight: 65
            ////                anchors.centerIn: parent
            ////                x: positionFactor * 65
            //                radius: 40

            //                Button {
            //                    id:middleEye
            //                    display: AbstractButton.IconOnly
            ////                    width:30
            ////                    height: 30
            //                    anchors.centerIn: parent
            //                    icon.source : "qrc:/Resources/eye.png"
            //                    icon.width :20
            //                    icon.height : 20
            //                    icon.color :_colorIcon
            //                    background: Rectangle{

            //                        color: "#003569"
            //                        border.color: "black"
            //                        border.width: 2
            //                        radius:20

            //                    }
            //                    smooth: true
            //                }
            //                MultiEffect {
            //                    source: middleEye
            //                    enabled: true
            //                    anchors.fill: middleEye
            //                    shadowColor: "black"
            //                    shadowEnabled: true
            //                    shadowBlur: 1
            //                    shadowHorizontalOffset: 3.5
            //                    shadowVerticalOffset:2.5
            //                    shadowOpacity:1
            //                    paddingRect: Qt.rect(0,0,20,20)
            //                    shadowScale: 0.98

            //                }

            //            }

            ControlCamera{
                id: moveControlCamera
                width: 65
                height: 65
                x: positionFactor * 65
//                anchors.top: rotationControlCamera.bottom
//                anchors.topMargin: 17
                //color: "transparent"
            }




            //////// -----------------  move control


            //            Rectangle{
            //                id: moveControlCamera
            //                Layout.preferredWidth:65
            //                Layout.preferredHeight: 65

            ////                anchors.centerIn: parent
            ////                x: positionFactor * 65
            //                radius: parent.height/2

            //                Button {
            //                    id:middleHand
            //                    display: AbstractButton.IconOnly
            //                    anchors.centerIn: parent
            //                    icon.source : "qrc:/Resources/hand.png"
            //                    icon.width :20
            //                    icon.height : 20
            //                    icon.color :"white"

            //                    background: Rectangle{

            //                        color: "#003569"
            //                        border.color: "black"
            //                        border.width: 2
            //                        radius:20

            //                    }
            //                    smooth: true
            //                }

            //                MultiEffect {
            //                    source: middleHand
            //                    enabled: true
            //                    anchors.fill: middleHand
            //                    shadowColor: "black"
            //                    shadowEnabled: true
            //                    shadowBlur: 1
            //                    shadowHorizontalOffset: 3.5
            //                    shadowVerticalOffset:2.5
            //                    shadowOpacity:1
            //                    paddingRect: Qt.rect(0,0,20,20)
            //                    shadowScale: 0.98
            //                }
            //            }
        }
        //------------------ Navigation
        /*
          a column layout which contains every Element
          in our desired navigartion bar
        */

        ColumnLayout {
            id:navigationLayout
            spacing: 4
//            Layout.preferredWidth: 40
//            Layout.preferredHeight: 40
            Item {

                Layout.preferredWidth: 40
                Layout.preferredHeight: 82

                Rectangle {
                    id: zoomBtnContainer
                    width: 40
                    height: 82
                    anchors.fill: parent
                    color: _colorIcon
                    radius: 20
                    ColumnLayout{
                        anchors.fill: parent
                        Button {
                            id: positive
                            Layout.leftMargin: 3
                            icon.source : "qrc:/Resources/add.png"
                            icon.width : 26
                            icon.height :26
                            icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                                   (pressed ? _colorHover : _colorPresed);
                            background:Rectangle {
                                color:"transparent"
                            }
                        }
                        Rectangle{
                            Layout.leftMargin:6
                            visible: true
                            width: parent.width/1.4
                            color:"black"
                            opacity: 0.3
                            height: 2
                        }

                        Button {
                            id: negative
                            Layout.leftMargin:3
                            text: qsTr("Button")
                            display: AbstractButton.IconOnly
                            icon.source : "qrc:/Resources/minus.png"
                            icon.width : 26
                            icon.height : 26
                            icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                                   (pressed ? _colorHover : _colorPresed);
                            background:Rectangle {
                                color:"transparent"
                            }
                        }

                    }
                }
                MultiEffect {
                    source: zoomBtnContainer
                    enabled: true
                    anchors.fill: zoomBtnContainer
                    shadowColor: "black"
                    shadowEnabled: true
                    shadowBlur: 1
                    shadowHorizontalOffset: 3.5
                    shadowVerticalOffset:2.5
                    shadowOpacity:1
                    paddingRect: Qt.rect(0,0,20,20)
                    shadowScale: 0.98
                }
            }
            Item {
                Layout.preferredWidth:40
                Layout.preferredHeight: 40
                Button {
                    id: directionBtn
                    rotation:0 /*-90 - 180*(positionFactor)*/
                    hoverEnabled: true
                    width: 40
                    height: 40
                    display: AbstractButton.IconOnly
                    icon.source : "qrc:/Resources/direction.png"
                    icon.width : 26
                    icon.height : 26
                    icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                           (pressed ? _colorHover : _colorPresed);

                    background: Rectangle{
                        color:_colorIcon
                        radius:20
                    }

                    onClicked: {
                        if(rootItem.positionFactor){
                            showSlider.stop()
                            hideSlider.start()
                        } else {
                            hideSlider.stop()
                            showSlider.start()
                        }
                    }
                }
                MultiEffect {
                    source: directionBtn
                    enabled: true
                    rotation: directionBtn.rotation
                    anchors.fill: directionBtn
                    shadowColor: "black"
                    shadowEnabled: true
                    shadowBlur: 1
                    shadowHorizontalOffset: 3.5
                    shadowVerticalOffset:2.5
                    shadowOpacity:1
                    paddingRect: Qt.rect(0,0,20,20)
                    shadowScale: 0.98
                }
            }
            Item {
                Layout.preferredWidth:40
                Layout.preferredHeight: 40
                Button {
                    id: home
                    width: 40
                    height: 40
                    hoverEnabled: true
                    display: AbstractButton.IconOnly
                    icon.source : "qrc:/Resources/home.png"
                    icon.width :_iconSize
                    icon.height : _iconSize
                    icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                           (pressed ? _colorHover : _colorPresed);
                    background: Rectangle{
                        color:_colorIcon
                        radius:20
                    }
                    onClicked: btnHomeClicked()
                }
                MultiEffect {
                    source: home
                    enabled: true
                    rotation: home.rotation
                    anchors.fill: home
                    shadowColor: "black"
                    shadowEnabled: true
                    shadowBlur: 1
                    shadowHorizontalOffset: 3.5
                    shadowVerticalOffset:2.5
                    shadowOpacity:1
                    paddingRect: Qt.rect(0,0,20,20)
                    shadowScale: 0.98
                }
            }

            Item {
                Layout.preferredWidth:40
                Layout.preferredHeight: 40

                Button {
                    id: project
                    display: AbstractButton.IconOnly
                    width:40
                    height:40
                    icon.source : modeMap === "projection" ? "qrc:///Resources/threeD.png": "qrc:/Resources/twoD.png"
                    icon.width :26
                    icon.height : 26
                    icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                           (pressed ? _colorHover : _colorPresed);
                    background: Rectangle{
                        color: _colorIcon
                        radius:20
                    }
                    smooth: true

                    onClicked:{
                        if (modeMap==="projection")
                            modeMap = "geocentric"
                        else
                            modeMap = "projection"
                        btnProjectionClicked()
                    }

                }
                MultiEffect {
                    source: project
                    enabled: true
                    anchors.fill: project
                    shadowColor: "black"
                    shadowEnabled: true
                    shadowBlur: 1
                    shadowHorizontalOffset: 3.5
                    shadowVerticalOffset:2.5
                    shadowOpacity:1
                    paddingRect: Qt.rect(0,0,20,20)
                    shadowScale: 0.98
                }
            }

        }
    }


    PropertyAnimation {
        id: showSlider
        target: rootItem
        property: "positionFactor"
        from: rootItem.positionFactor
        to: 1.0
        duration: myDuration * Math.abs(1.0 - positionFactor)

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: hideSlider
        target: rootItem
        property: "positionFactor"
        from: rootItem.positionFactor
        to: 0.0
        duration: myDuration * Math.abs(positionFactor)

        easing.type: Easing.InQuint
    }

}

/*##^##
Designer {
    D{i:4;anchors_x:8;anchors_y:64}D{i:6;anchors_y:18}D{i:5;anchors_y:18}D{i:7;anchors_y:"-8"}
D{i:9;anchors_width:24;anchors_x:9}D{i:11;anchors_width:24;anchors_y:"-8"}D{i:1;anchors_height:200;anchors_width:200;anchors_x:8;anchors_y:0}
}
##^##*/

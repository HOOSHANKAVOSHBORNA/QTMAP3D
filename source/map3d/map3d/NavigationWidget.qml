
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Effects

Item {
    id:rootItem
    width: mainRowLayout.implicitWidth
    height: mainRowLayout.implicitHeight

    property real positionFactor: 1.0
    property bool showRecMov: true
    property real myDuration: 300.0
    // signal Button
    //    signal btnUpClicked()
    //    signal btnDownClicked()
    //    signal btnLeftClicked()
    //    signal btnRightClicked()
    //
    //    signal btnRotateUpClicked()
    //    signal btnRotateDownClicked()
    //    signal btnRotateLeftClicked()
    //    signal btnRotateRightClicked()
    //
    //    signal btnZoomInClicked()
    //    signal btnZoomOutClicked()

    property bool zoomInButtonPressed: positive.pressed
    property bool zoomOutButtonPressed: negative.pressed

    property bool upButtonPressed:    moveControlCamera.upButtonPressed
    property bool downButtonPressed:  moveControlCamera.downButtonPressed
    property bool leftButtonPressed:  moveControlCamera.leftButtonPressed
    property bool rightButtonPressed: moveControlCamera.rightButtonPressed

    property bool rotateUpButtonPressed:   rotationControlCamera.upButtonPressed
    property bool rotateDownButtonPressed: rotationControlCamera.downButtonPressed
    property bool rotateLeftButtonPressed: rotationControlCamera.leftButtonPressed
    property bool rotateRightButtonPressed:rotationControlCamera.rightButtonPressed


    signal btnHomeClicked()
    signal btnProjectionClicked()

    RowLayout {
        id: mainRowLayout
        spacing: 5
        Rectangle {
            id: control
            color: "transparent"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 65
            Layout.minimumHeight: 150
            anchors.rightMargin: 5
            clip: true
            ControlCamera{
                id: rotationControlCamera
                width: 65
                height: 65
                //radius: _radius
                anchors.top: parent.top
                x: positionFactor * 65
                anchors.topMargin: 17
                centerIcon: "qrc:/Resources/eye.png"
                //color: "transparent"
                //buttonIcon: "qrc:/Resources/rotate.png"
            }

            ControlCamera{
                id: moveControlCamera
                width: 65
                height: 65
                x: positionFactor * 65
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 17
                //color: "transparent"
            }
        }


        //        Button {
        //            id: sliderMenuBtn
        //            Layout.preferredWidth: 5
        //            Layout.preferredHeight: 75
        //            Layout.alignment: Qt.AlignCenter

        //            display: AbstractButton.IconOnly
        //            icon.source: "qrc:/Resources/chevron.png"
        ////            icon.width : 32
        ////            icon.height : 32
        //            icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
        //                                   (pressed ? _colorHover : "#FFFFFF");
        //            background: Rectangle{
        //                radius: _radius
        //                color: "green"
        //            }
        //            checkable: true
        //            states: [
        //                State {
        //                    name: "rotated"
        //                    PropertyChanges { target: sliderMenuBtn; rotation: 180; }
        //                },
        //                State {
        //                    name: "default"
        //                    PropertyChanges { target: sliderMenuBtn; rotation: 0 }
        //                }

        //            ]

        //            onClicked: function() {
        //                sliderMenuBtn.state = showRecMov ? "rotated" : "default"
        //                if(showRecMov){
        //                    showSlider.stop()
        //                    hideSlider.start()
        //                    showRecMov = false
        //                } else {
        //                    hideSlider.stop()
        //                    showSlider.start()
        //                    showRecMov = true
        //                }
        //            }

        //            transitions: Transition {
        //                RotationAnimation { duration: myDuration; direction: RotationAnimation.Counterclockwise }
        //            }
        //        }


        GridLayout {
            id:grL
            columns:  1
            rows: 5
            rowSpacing: 3
            columnSpacing: 0

            Layout.preferredWidth: implicitWidth
            Layout.preferredHeight: implicitHeight

            Button {
                id: positive
                icon.source : "qrc:/Resources/zoomin-r.png"
                icon.width : _iconSize
                icon.height : _iconSize
                icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                       (pressed ? _colorHover : "#FFFFFF");
                background:Rectangle {
                    color:_colorRec
                    //opacity: 0.8
                    //radius: _radius
                }
            }


            Button {
                id: negative
                text: qsTr("Button")
                display: AbstractButton.IconOnly
                icon.source : "qrc:/Resources/zoomout-r.png"
                icon.width : _iconSize
                icon.height : _iconSize
                icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                       (pressed ? _colorHover : "#FFFFFF");
                background:Rectangle {
                    color:_colorRec
                }
            }
            Rectangle {
                id: sliderMenuBtn
                width: 32
                height: 30
                color: _colorRec

                Image {
                    id: direction
                    source: "./Resources/direction.png"
                    sourceSize: ("24 x 24")
                    anchors.centerIn: parent
                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered:
                            chevronRecolor.colorizationColor = _colorHover
                        onExited:
                            chevronRecolor.colorizationColor = "#FFFFFF"
                    }
                }
                MultiEffect{
                    id:chevronRecolor
                    source: direction
                    anchors.fill: direction
                    colorization: 1
                    colorizationColor: "#FFFFFF"
                }
                states: [
                    State {
                        name: "rotated"
                        PropertyChanges { target: direction; rotation: 180; }
                        PropertyChanges { target: chevronRecolor; rotation: 180; }
                    },
                    State {
                        name: "default"
                        PropertyChanges { target: direction; rotation: 0 }
                        PropertyChanges { target: chevronRecolor; rotation: 0 }
                    }

                ]
                MouseArea{
                    anchors.fill: parent
                    onClicked: function() {
                        sliderMenuBtn.state = showRecMov ? "rotated" : "default"
                        if(showRecMov){
                            showSlider.stop()
                            hideSlider.start()
                            showRecMov = false
                        } else {
                            hideSlider.stop()
                            showSlider.start()
                            showRecMov = true
                        }
                    }
                }

                transitions: Transition {
                    RotationAnimation { duration: myDuration; direction: RotationAnimation.Counterclockwise }
                }

            }
            Button {
                id: home
                hoverEnabled: true
                display: AbstractButton.IconOnly
                icon.source : "qrc:/Resources/home-r.png"
                icon.width : _iconSize
                icon.height : _iconSize
                icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                       (pressed ? _colorHover : "#FFFFFF");
                background: Rectangle{
                    //radius: 10
                    //opacity: 0.8
                    color: _colorRec
                }

                onClicked: btnHomeClicked()
            }


            Button {
                id: project
                display: AbstractButton.IconOnly
                icon.source : modeMap === "projection" ? "qrc:///Resources/threeD.png": "qrc:/Resources/twoD.png"
                icon.width : _iconSize
                icon.height : _iconSize
                icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                       (pressed ? _colorHover : "transparent");
                background: Rectangle{
                    //radius: _radius
                    color: _colorRec
                    //opacity: 0.8
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

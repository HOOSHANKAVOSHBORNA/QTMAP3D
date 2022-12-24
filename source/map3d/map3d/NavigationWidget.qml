
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

Item {
    id:rootItem
    width: mainRowLayout.implicitWidth
    height: mainRowLayout.implicitHeight

    property real positionFactor: 1.0
    property bool showRecMov: true
    property real myDuration: 300.0
    // signal Button
    signal btnUpClicked()
    signal btnDownClicked()
    signal btnLeftClicked()
    signal btnRightClicked()

    signal btnRotateUpClicked()
    signal btnRotateDownClicked()
    signal btnRotateLeftClicked()
    signal btnRotateRightClicked()

    signal btnZoomInClicked()
    signal btnZoomOutClicked()

    signal btnHomeClicked()
    signal btnProjectionClicked()

    RowLayout {
        id: mainRowLayout
        anchors.fill: parent


        Rectangle {
            id: control
            color: "transparent"

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.minimumWidth: 215
            Layout.maximumWidth: 215


            clip: true
            ControlCamera{
                id: recRotation
                width: 100
                radius: _radius
                anchors.leftMargin: _margin
                anchors.left: recMove.right
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 1
                anchors.top: parent.top
                anchors.topMargin: 3
                color: "transparent"
                buttonIcon: "qrc:/Resources/rotate.png"
                onItemClicked: {
                    switch (direction){
                    case "UP" :
                        btnRotateUpClicked()
                        break
                    case "DOWN" :
                        btnRotateDownClicked()
                        break
                    case "LEFT" :
                        btnRotateLeftClicked()
                        break
                    case "RIGHT" :
                        btnRotateRightClicked()
                    }
                }

            }

            ControlCamera{
                id: recMove
                width: 100
                x: positionFactor * 215
                anchors.rightMargin: _margin
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 1
                anchors.top: parent.top
                anchors.topMargin: 3
                color: "transparent"
                onItemClicked: {
                    switch (direction){
                    case "UP" :
                        btnUpClicked()
                        break
                    case "DOWN" :
                        btnDownClicked()
                        break
                    case "LEFT" :
                        btnLeftClicked()
                        break
                    case "RIGHT" :
                        btnRightClicked()
                    }
                }
            }
        }


        Button {
            id: sliderMenuBtn
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
            Layout.alignment: Qt.AlignCenter

            display: AbstractButton.IconOnly
            icon.source: "qrc:/Resources/chevron.png"
            icon.width : 64
            icon.height : 64
            icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                   (pressed ? _colorHover : "#FFFFFF");
            background: Rectangle{
                radius: _radius
                color: "transparent"
            }
            checkable: true
            states: [
                State {
                    name: "rotated"
                    PropertyChanges { target: sliderMenuBtn; rotation: 180; }
                },
                State {
                    name: "default"
                    PropertyChanges { target: sliderMenuBtn; rotation: 0 }
                }

            ]

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

            transitions: Transition {
                RotationAnimation { duration: myDuration; direction: RotationAnimation.Counterclockwise }
            }
        }


        GridLayout {
            columns:  2
            rows: 2
            rowSpacing: 5
            columnSpacing: 5

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
                    radius: _radius
                }
                //onClicked: btnZoomInClicked()
                onPressed:{
                    timerPositive.running =true
                }
                onReleased:{
                    timerPositive.running = false
                }
                Timer {
                    id:timerPositive
                    interval: 150; running: false; repeat: true
                    onTriggered:  btnZoomInClicked()
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
                    radius: 10
                    color: _colorRec
                }

                onClicked: btnHomeClicked()
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
                    radius: _radius
                }

                onPressed:{
                    timerNegative.running =true
                }
                onReleased:{
                    timerNegative.running = false
                }
                Timer {
                    id:timerNegative
                    interval: 150; running: false; repeat: true
                    onTriggered:  btnZoomOutClicked()
                }
            }


            Button {
                id: project
                display: AbstractButton.IconOnly
                icon.source : modeMap === "projection" ? "qrc:///Resources/3d_l.png": "qrc:/Resources/TwoD.png"
                icon.width : _iconSize
                icon.height : _iconSize
                icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                       (pressed ? _colorHover : "transparent");
                background: Rectangle{
                    radius: _radius
                    color: _colorRec
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

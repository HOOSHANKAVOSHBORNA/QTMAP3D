
import QtQuick 2.13
import QtQuick.Controls 2.13

Item {
    id:rootItem
    width: 330
    height: 80

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
    Rectangle {
        id: rootRec
        color: "transparent"
        radius: _radius
        opacity: 0.8
        anchors.fill: parent
        Button {
            id: home
            width: rootItem.height /2 - 4
            height: rootItem.height /2 - 4
            anchors.top: parent.top
            anchors.topMargin: 3
            hoverEnabled: true
            display: AbstractButton.IconOnly
            anchors.left: parent.left
            anchors.leftMargin: _margin
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

        ControlCamera{
            id: recMove
            width: 100
            anchors.left: home.right
            anchors.leftMargin: _margin
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
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

        Button {
            id: positive
            width: rootItem.height /2 - 4
            height: rootItem.height /2 - 4
            anchors.leftMargin: _margin
            anchors.left: recRotation.right
            anchors.top: parent.top
            anchors.topMargin: 3
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
            id: negative
            width: rootItem.height /2 - 4
            height: rootItem.height /2 - 4
            text: qsTr("Button")
            anchors.leftMargin: _margin
            display: AbstractButton.IconOnly
            anchors.left: recRotation.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
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
        ControlCamera{
            id: recRotation
            width: 100
            radius: _radius
            anchors.left: recMove.right
            anchors.leftMargin: _margin
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
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


        Button {
            id: project
            width: rootItem.height /2 - 4
            height: rootItem.height /2 - 4
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 3
            anchors.leftMargin: _margin
            display: AbstractButton.IconOnly
            anchors.left: parent.left
            icon.source : modeMap === "projection" ? "qrc:/Resources/projection.png" :"qrc:/Resources/geocentric.png"
            icon.width : _iconSize
            icon.height : _iconSize
            icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                                   (pressed ? _colorHover : "#FFFFFF");
            background: Rectangle{
                radius: _radius
                color: _colorRec
            }

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

/*##^##
Designer {
    D{i:4;anchors_x:8;anchors_y:64}D{i:6;anchors_y:18}D{i:5;anchors_y:18}D{i:7;anchors_y:"-8"}
D{i:9;anchors_width:24;anchors_x:9}D{i:11;anchors_width:24;anchors_y:"-8"}D{i:1;anchors_height:200;anchors_width:200;anchors_x:8;anchors_y:0}
}
##^##*/

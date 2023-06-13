import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Effects

Rectangle {
    property url buttonIcon: "qrc:/Resources/arrow.png"
    property url centerIcon: "qrc:/Resources/hand.png"
    signal itemClicked(string direction)
    color: "transparent"
    id:sp
    radius: sp.height/2
    property bool upButtonPressed: btn_up.pressed
    property bool downButtonPressed: btn_down.pressed
    property bool leftButtonPressed: btn_left.pressed
    property bool rightButtonPressed: btn_right.pressed

    Rectangle{
        anchors.fill: parent
        color: _colorRec
        opacity: 0.7
        radius: sp.height/2

    } 

    Image {
        id: center
        source: centerIcon
        anchors.centerIn: parent
        width: 17
        height: 17

    }

//    MultiEffect{
//        id: centerRecolor
//        source: center
//        anchors.fill: center
//        colorization: 1
//        colorizationColor: "#FFFFFF"
//    }

    Button {
        id: btn_right
        width: 30
        height: 30
        anchors.right: parent.right
        anchors.rightMargin: -5
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 0
        display: AbstractButton.IconOnly
        rotation: 180
        icon.source : buttonIcon
        icon.width : _iconSize
        icon.height : _iconSize
        icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                               (pressed ? _colorHover : "white");
        background: Rectangle{
            color:"transparent"
            radius: _radius * 10
        }
    }

    Button {
        id: btn_up
        width: 30
        height: 30
        anchors.top: parent.top
        anchors.topMargin: -5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        display: AbstractButton.IconOnly
        rotation: 90
        icon.source : buttonIcon
        icon.width : _iconSize
        icon.height : _iconSize
        icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                               (pressed ? _colorHover : "white");
        background: Rectangle{
            color:"transparent"
            radius: _radius * 10
        }
    }

    Button {
        id: btn_left
        width: 30
        height: 30
        anchors.left: parent.left
        anchors.leftMargin: -5
        anchors.rightMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        display: AbstractButton.IconOnly
        icon.source : buttonIcon
        icon.width : _iconSize
        icon.height : _iconSize
        icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                               (pressed ? _colorHover : "white");
        background: Rectangle{
            color:"transparent"
            radius: _radius * 10
        }
    }

    Button {
        id: btn_down
        width: 30
        height: 30
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        display: AbstractButton.IconOnly
        rotation: -90
        icon.source : buttonIcon
        icon.width : _iconSize
        icon.height : _iconSize
        icon.color : hovered ? (pressed ? _colorPresed: _colorHover) :
                               (pressed ? _colorHover : "white");
        background: Rectangle{
            color:"transparent"
            radius: _radius * 10
        }
    }
}

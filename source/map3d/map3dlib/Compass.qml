import QtQuick 2.12

Item {
    id: name
    visible: true
    readonly property int  _widthparent : 40
    readonly property int  _heightparent:  40
    readonly property int  _widthimage : 90
    readonly property int  _heightimage:  90
    Rectangle {

        id: compass
        width: _widthparent
        height: _widthparent
        color: "transparent"
        smooth: true
        radius: 100
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#a1c4fd"
            }

            GradientStop {
                position: 1
                color: "#c2e9fb"
            }
        }
        anchors.centerIn: parent

        Image {
            id: shadow
            width: _widthimage
            height: _widthimage
            anchors.centerIn: compass
            source: "qrc:/res/icon/compass.png"
            opacity: 1
        }
        Connections{
            target: GetData
            onRotateChange:{
                shadow.rotation = newRotate
            }
        }
        Connections{
            target: GetData
            onPointChange:{
                shadow.rotation = newRotate
            }
        }
    }
}

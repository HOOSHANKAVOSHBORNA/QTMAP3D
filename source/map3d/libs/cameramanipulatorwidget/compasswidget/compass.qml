import QtQuick 2.12

Rectangle {
    id: compass
    width: 40
    height: 40
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
        width: 90
        height: 90
        anchors.centerIn: compass
        source: "qrc:/image/compress.png"
        opacity: 1
    }
    Connections{
        target: GetData
        onRotateChange:{
            shadow.rotation+=newRotate
        }
    }
}

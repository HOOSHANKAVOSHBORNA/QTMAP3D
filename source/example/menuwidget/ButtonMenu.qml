import QtQuick 2.0

Item {
    width: 40
    height: 40
    property alias getMouseArea: m
    Rectangle{
        id : backgrand
        anchors.fill: parent
        color: "#55000000"
        border.color: "black"
        border.width: 1
        radius: 10
        MouseArea{
          id:m
          anchors.fill: parent
          hoverEnabled: true
          onEntered: {
              anim_color0.running=true
          }
          onExited: {
              anim_color1.running=true

          }

        }
        ColorAnimation on color {
            id:anim_color0
            from: "#55000000"
            to: "#DD000000"
            duration: 1000
            running: false

        }
        ColorAnimation on color {
            id:anim_color1
            from: "#DD000000"
            to: "#55000000"
            duration: 1000
            running: false

        }
        Image {
            id :earth

            width: 32
            height: 32
            anchors.centerIn: backgrand
            source: "qrc:/res/map3d.ico"

        }

        RotationAnimation{
            id:anim1
            target: earth
            from :0
            to:360
            duration: 10000
            running: true
            loops: Animation.Infinite
        }
    }

}

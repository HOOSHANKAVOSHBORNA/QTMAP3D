import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import "style"

Item {
    readonly property double positionX: (mainCircle.width/2 - draggableCircle.x-draggableCircle.width/2)
    readonly property double positionY: -(mainCircle.height/2 -draggableCircle.y-draggableCircle.height/2)

    property vector2d position: Qt.vector2d(positionX,positionY)
    property url source: "qrc:/Resources/hand.png"

    Rectangle {
        id: mainCircle
        width: 80/Style.monitorRatio
        height: 80/Style.monitorRatio
        color:Style.backgroundColor
        radius: width/2
        anchors.centerIn: parent
        IconImage {
            id: bottomIconImage
            anchors.bottom:  parent.bottom
            anchors.horizontalCenter:  parent.horizontalCenter
            source: "qrc:/Resources/down"
            width:  10 /Style.monitorRatio
            height: 10 /Style.monitorRatio

        }
        IconImage {
            id: topIconImage
            anchors.top:  parent.top
            anchors.horizontalCenter:  parent.horizontalCenter
            source: "qrc:/Resources/down"
            width:  10 /Style.monitorRatio
            height: 10 /Style.monitorRatio
            rotation: -180

        }
        IconImage {
            id: leftIconImage
            anchors.left:  parent.left
            anchors.verticalCenter:  parent.verticalCenter
            source: "qrc:/Resources/down"
            width:  10 /Style.monitorRatio
            height: 10 /Style.monitorRatio
            rotation: 90

        }
        IconImage {
            id: rightIconImage
            anchors.right:  parent.right
            anchors.verticalCenter:  parent.verticalCenter
            source: "qrc:/Resources/down"
            width:  10 /Style.monitorRatio
            height: 10 /Style.monitorRatio
            rotation: -90

        }
        Rectangle{
            width: 62/Style.monitorRatio
            height: 62/Style.monitorRatio
            color:"transparent"
            radius: width/2
            anchors.centerIn: parent
            border.color: Qt.rgba(Style.foregroundColor.r,Style.foregroundColor.g,Style.foregroundColor.b,0.25)

        }
        Rectangle{
            id: middleHole
            width:draggableCircle.width
            height:draggableCircle.height
            anchors.centerIn:parent
            radius : height/2
            opacity : 0.2
            gradient: Gradient {
                GradientStop { position: 0.0; color: "black" }
                GradientStop { position: 1.0; color: "white" }
            }
        }

        RoundButton {
            property double radiusDiff : mainCircle.width/2 - draggableCircle.width /2

            id: draggableCircle
            width: 30/Style.monitorRatio
            height: 30/Style.monitorRatio
            x: radiusDiff
            y: radiusDiff
            icon.source : source
            icon.color : Style.backgroundColor
            icon.width: 20/Style.monitorRatio
            icon.height: 20/Style.monitorRatio
            clip: true
            background: Rectangle{
                color: Style.foregroundColor
                radius: parent.radius
            }
            Component.onCompleted: function (){
                dragArea.drag.minimumX = radiusDiff -Math.max(Math.sqrt(radiusDiff *radiusDiff -Math.pow(radiusDiff -draggableCircle.y,2)), radiusDiff )
                dragArea.drag.maximumX = /*Math.min(*/Math.sqrt(radiusDiff*radiusDiff-Math.pow(radiusDiff -draggableCircle.y,2)/*)*/)+radiusDiff
                dragArea.drag.minimumY = radiusDiff -Math.max(Math.sqrt(radiusDiff*radiusDiff-Math.pow((radiusDiff -draggableCircle.x), 2)))
                dragArea.drag.maximumY = /*Math.min(*/Math.sqrt(radiusDiff*radiusDiff-Math.pow((radiusDiff -draggableCircle.x), 2)/*)*/)+radiusDiff
            }
            MouseArea {
                id: dragArea
                anchors.fill: parent
                drag.target: parent

                onReleased: {
                    // Snap back to starting position
                    draggableCircle.x = (mainCircle.width - draggableCircle.width) / 2
                    draggableCircle.y = (mainCircle.height - draggableCircle.height) / 2
                }
            }
        }
        MultiEffect {
            source: draggableCircle
            enabled: true
            anchors.fill: draggableCircle
            shadowColor: "black"
            shadowEnabled: true
            shadowBlur: 1
            shadowHorizontalOffset: -1.5
            shadowVerticalOffset:1
            shadowOpacity:0.4
            shadowScale: 0.98
        }

    }
}






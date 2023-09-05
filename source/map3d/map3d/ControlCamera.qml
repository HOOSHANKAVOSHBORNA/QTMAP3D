import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import "style"

Rectangle {
    id:root
    visible: true
    width: Style.uiSecondContainerSize
    height: Style.uiSecondContainerSize
    color: "transparent"
    property double requestToMoveX: (containerCircle.width/2 - draggableCircle.x-12.5)
    property double requestToMoveY: -(containerCircle.height/2 -draggableCircle.y-12.5)

    property variant requstXY: Qt.vector2d(requestToMoveX,requestToMoveY)

    property url centerIcon: "qrc:/Resources/hand.png"

    property double _circleRadiusDiffrential : containerCircle.width/2 - draggableCircle.width /2

    Rectangle {
        id: containerCircle
        width: Style.uiSecondContainerSize
        height: Style.uiSecondContainerSize
        color:Style.uiWhite
        radius: width / 2
        anchors.centerIn: parent
        clip: true
        opacity:0.9
        Rectangle{
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


        Rectangle {
            id: draggableCircle
            width: Style._iconSize+1
            height: Style._iconSize+1
            radius: width / 2
            color: Style.uiBlue
            border.color: "black"
            border.width: 0.2

            clip: true
            x: _circleRadiusDiffrential
            y: _circleRadiusDiffrential
            Component.onCompleted: function (){
               dragArea.drag.minimumX = _circleRadiusDiffrential -Math.max(Math.sqrt(_circleRadiusDiffrential *_circleRadiusDiffrential -Math.pow(_circleRadiusDiffrential -draggableCircle.y,2)), _circleRadiusDiffrential )
               dragArea.drag.maximumX = Math.min(Math.sqrt(_circleRadiusDiffrential*_circleRadiusDiffrential-Math.pow(_circleRadiusDiffrential -draggableCircle.y,2)))+_circleRadiusDiffrential
               dragArea.drag.minimumY = _circleRadiusDiffrential -Math.max(Math.sqrt(_circleRadiusDiffrential*_circleRadiusDiffrential-Math.pow((_circleRadiusDiffrential -draggableCircle.x), 2)))
               dragArea.drag.maximumY = Math.min(Math.sqrt(_circleRadiusDiffrential*_circleRadiusDiffrential-Math.pow((_circleRadiusDiffrential -draggableCircle.x), 2)))+_circleRadiusDiffrential
            }

            Button {
                id: positive
                anchors.centerIn: parent
                icon.source : centerIcon
                icon.width : Style._iconSize-8
                icon.height :Style._iconSize-8
                icon.color : Style.uiWhite
                background:Rectangle {
                    color:"transparent"
                }
            }
            MouseArea {
                id: dragArea
                anchors.fill: parent
                drag.target: parent

                onReleased: {
                    // Snap back to starting position
                    draggableCircle.x = (containerCircle.width - draggableCircle.width) / 2
                    draggableCircle.y = (containerCircle.height - draggableCircle.height) / 2
                }

            /*
                this method can be implemented as well.
                instead of using drag.<something> in component.oncompleted, you can use it here!
                the functioning shall "not" be the same.

            */



//                drag.minimumX: _circleRadiusDiffrential -Math.max(Math.sqrt(_circleRadiusDiffrential *_circleRadiusDiffrential -Math.pow(_circleRadiusDiffrential -draggableCircle.y,2)), _circleRadiusDiffrential )
//                drag.maximumX: Math.min(Math.sqrt(_circleRadiusDiffrential*_circleRadiusDiffrential-Math.pow(_circleRadiusDiffrential -draggableCircle.y,2)))+_circleRadiusDiffrential
//                drag.minimumY: _circleRadiusDiffrential -Math.max(Math.sqrt(_circleRadiusDiffrential*_circleRadiusDiffrential-Math.pow((_circleRadiusDiffrential -draggableCircle.x), 2)))
//                drag.maximumY: Math.min(Math.sqrt(_circleRadiusDiffrential*_circleRadiusDiffrential-Math.pow((_circleRadiusDiffrential -draggableCircle.x), 2)))+_circleRadiusDiffrential

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
            paddingRect: Qt.rect(0,0,20,20)
            shadowScale: 0.98
        }

    }
}






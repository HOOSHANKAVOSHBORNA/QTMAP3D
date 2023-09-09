
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"

Item {
    property real positionFactor: 1.0
    property bool showRecMov: true
    property real animationDuration: 500.0
    property int iconSize: 26/Style.monitorRatio

    property string mode: "geocentric"
    property bool zoomInButtonPressed: zoomInBtn.pressed
    property bool zoomOutButtonPressed: zoomOutBtn.pressed
    property vector2d movePosition  : moveCameraHandler.position
    property vector2d rotatePositon: rotateCameraHandler.position

    signal btnHomeClicked()
    signal btnProjectionClicked()

    id:rootItem
    width: mainRowLayout.implicitWidth
    height: mainRowLayout.implicitHeight
    smooth: true

    RowLayout {
        id: mainRowLayout
        anchors.fill: parent
        spacing: 5

        RowLayout{
            id:cameraPositionLayout
            spacing: 5
            clip: true
            opacity: 1 - 1.7*positionFactor
            x: positionFactor * width
            CameraPositionHandlerItem{
                id: rotateCameraHandler
                width: 80/Style.monitorRatio
                height: 80/Style.monitorRatio
                source: "qrc:/Resources/eye.png"

            }
            CameraPositionHandlerItem{
                id: moveCameraHandler
                width: 80/Style.monitorRatio
                height: 80/Style.monitorRatio
                source: "qrc:/Resources/hand.png"
            }
        }

        ColumnLayout {
            spacing: 4
            Rectangle {
                id: zoomBtnContainer
                width: 40/Style.monitorRatio
                height: 80/Style.monitorRatio
                color: Style.backgroundColor
                radius: width/2
                ColumnLayout{
                    id:test
                    anchors.fill: parent
                    Button {
                        id: zoomInBtn
                        icon.source : "qrc:/Resources/add.png"
                        icon.width: iconSize
                        icon.height: iconSize
                        icon.color: hovered ? (pressed ? Style.foregroundColor: Style.hoverColor) :
                                              (pressed ? Style.hoverColor : Style.foregroundColor);
                        background:Rectangle {
                            color:"transparent"
                        }
                    }
                    Rectangle{
                        Layout.alignment: Qt.AlignCenter
                        width:(zoomBtnContainer.width *3/4)/Style.monitorRatio
                        color:"black"
                        opacity: 0.3
                        height: 2/Style.monitorRatio
                    }

                    Button {
                        id: zoomOutBtn
                        icon.source : "qrc:/Resources/minus.png"
                        icon.width : iconSize
                        icon.height : iconSize
                        icon.color : hovered ? (pressed ? Style.foregroundColor: Style.hoverColor) :
                                               (pressed ? Style.hoverColor : Style.foregroundColor);
                        background:Rectangle {
                            color:"transparent"
                        }
                    }

                }
            }
            Button {
                id: directionBtn
                rotation:0 /*-90 - 180*(positionFactor)*/
                hoverEnabled: true
                display: AbstractButton.IconOnly
                icon.source : "qrc:/Resources/direction.png"
                icon.width : iconSize
                icon.height : iconSize
                icon.color : hovered ? (pressed ? Style.foregroundColor: Style.hoverColor) :
                                       (pressed ? Style.hoverColor : Style.foregroundColor);

                background: Rectangle{
                    color:Style.backgroundColor
                    radius:width/2
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
            Button {
                id: homeBtn
                hoverEnabled: true
                display: AbstractButton.IconOnly
                icon.source : "qrc:/Resources/home.png"
                icon.width :iconSize
                icon.height : iconSize
                icon.color : hovered ? (pressed ? Style.foregroundColor: Style.hoverColor) :
                                       (pressed ? Style.hoverColor : Style.foregroundColor);
                background: Rectangle{
                    color:Style.uiWhite
                    radius:20
                }
                onClicked: btnHomeClicked()
            }
            Button {
                id: modeBtn
                display: AbstractButton.IconOnly
                icon.source : mode === "projection" ? "qrc:///Resources/threeD.png": "qrc:/Resources/twoD.png"
                icon.width :iconSize
                icon.height : iconSize
                icon.color : hovered ? (pressed ? Style.foregroundColor: Style.hoverColor) :
                                       (pressed ? Style.hoverColor : Style.foregroundColor);
                background: Rectangle{
                    color: Style.uiWhite
                    radius:20
                }
                smooth: true

                onClicked:{
                    if (mode==="projection")
                        mode = "geocentric"
                    else
                        mode = "projection"
                    btnProjectionClicked()
                }

            }

        }
    }
    MultiEffect {
        source: mainRowLayout
        enabled: true
        anchors.fill: mainRowLayout
        shadowColor: "black"
        shadowEnabled: true
        shadowBlur: 1
        shadowHorizontalOffset: 3.5
        shadowVerticalOffset:2.5
        shadowOpacity:1
        shadowScale: 0.98
    }
    PropertyAnimation {
        id: showSlider
        target: rootItem
        property: "positionFactor"
        from: rootItem.positionFactor
        to: 1.0
        duration: animationDuration * Math.abs(1.0 - positionFactor)

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: hideSlider
        target: rootItem
        property: "positionFactor"
        from: rootItem.positionFactor
        to: 0.0
        duration: animationDuration * Math.abs(positionFactor)

        easing.type: Easing.InQuint
    }
}

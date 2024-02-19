import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"

Item {
    id: rootItem
    property real positionFactor: 1.0
    property bool showRecMov: true
    property real animationDuration: 250.0
    property int iconSize: 26 / Style.monitorRatio

    property string mode: "geocentric"
    property bool zoomInButtonPressed: zoomInBtn.pressed
    property bool zoomOutButtonPressed: zoomOutBtn.pressed
    property vector2d movePosition: moveCameraHandler.position
    property vector2d rotatePositon: rotateCameraHandler.position
    signal btnHomeClicked
    signal btnProjectionClicked

    ColumnLayout{
        id:mainColumn
        spacing: 5 / Style.monitorRatio
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        Rectangle{
            id:zoomContainer
            Layout.alignment: Qt.AlignRight
            width : 40 / Style.monitorRatio
            height: 80 / Style.monitorRatio
            color: Style.backgroundColor
            radius: 30 / Style.monitorRatio

            Button {
                id: zoomInBtn
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                width: 40 / Style.monitorRatio
                height: 40 / Style.monitorRatio

                icon.source: "qrc:/Resources/add.png"
                icon.width: iconSize
                icon.height: iconSize
                icon.color: hovered ? (pressed ? Style.foregroundColor : Style.hoverColor) : (pressed ? Style.hoverColor : Style.foregroundColor)
                background: Rectangle {
                    color: "transparent"
                }
            }
            Rectangle {
                id: seperator
                anchors.top: zoomInBtn.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: zoomContainer.width * 3 / 4
                color: "black"
                opacity: 0.3
                height: 2 / Style.monitorRatio
            }

            Button {
                id: zoomOutBtn
                anchors.top: seperator.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: 40 / Style.monitorRatio
                height: 40 / Style.monitorRatio

                icon.source: "qrc:/Resources/minus.png"
                icon.width: iconSize
                icon.height: iconSize
                icon.color: hovered ? (pressed ? Style.foregroundColor : Style.hoverColor) : (pressed ? Style.hoverColor : Style.foregroundColor)
                background: Rectangle {
                    color: "transparent"
                }
            }
        }

        Button {
            id: directionBtn

            Layout.alignment: Qt.AlignCenter
            width: 26 / Style.monitorRatio
            height: 26 / Style.monitorRatio

            hoverEnabled: true
            display: AbstractButton.IconOnly
            icon.source: "qrc:/Resources/direction.png"
            icon.width: iconSize
            icon.height: iconSize
            icon.color: hovered ? (pressed ? Style.foregroundColor : Style.hoverColor) : (pressed ? Style.hoverColor : Style.foregroundColor)

            background: Rectangle {
                color: Style.backgroundColor
                radius: 30 / Style.monitorRatio
            }
            onClicked: {
                rotateCameraHandler.visible = true
                moveCameraHandler.visible = true

                if (rootItem.positionFactor) {

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
            width: 40 / Style.monitorRatio
            height: 40 / Style.monitorRatio
            Layout.alignment: Qt.AlignCenter
            hoverEnabled: true
            display: AbstractButton.IconOnly
            icon.source: "qrc:/Resources/home.png"
            icon.width: iconSize
            icon.height: iconSize
            icon.color: hovered ? (pressed ? Style.foregroundColor : Style.hoverColor) : (pressed ? Style.hoverColor : Style.foregroundColor)
            background: Rectangle {
                color: Style.uiWhite
                radius: 30 / Style.monitorRatio
            }
            onClicked: btnHomeClicked()
        }
        Button {
            id: modeBtn
            Layout.alignment: Qt.AlignCenter
            width: 40 / Style.monitorRatio
            height: 40 / Style.monitorRatio
            display: AbstractButton.IconOnly
            icon.source: mode === "projection" ? "qrc:///Resources/threeD.png" : "qrc:/Resources/twoD.png"
            icon.width: iconSize
            icon.height: iconSize
            icon.color: hovered ? (pressed ? Style.foregroundColor : Style.hoverColor) : (pressed ? Style.hoverColor : Style.foregroundColor)
            background: Rectangle {
                color: Style.backgroundColor
                radius:30 / Style.monitorRatio
            }
            smooth: true

            onClicked: {
                if (mode === "projection")
                    mode = "geocentric"
                else
                    mode = "projection"
                btnProjectionClicked()
            }
        }
    }
    RowLayout {
        id: cameraPositionLayout
        spacing: 5
        clip: true
        anchors.verticalCenter: rootItem.verticalCenter
        opacity: 1 - 1.7 * positionFactor
        x: width * positionFactor
        CameraPositionHandlerItem {
            id: rotateCameraHandler
            width: 80 / Style.monitorRatio
            height: 80 / Style.monitorRatio
            source: "qrc:/Resources/eye.png"
            visible: false
        }
        CameraPositionHandlerItem {
            id: moveCameraHandler
            width: 80 / Style.monitorRatio
            height: 80 / Style.monitorRatio
            source: "qrc:/Resources/hand.png"
            visible: false
        }
    }
    MultiEffect {
        source: mainColumn
        enabled: true
        anchors.fill: mainColumn
        shadowColor: "black"
        shadowEnabled: true
        shadowBlur: 1
        shadowHorizontalOffset: 3.5
        shadowVerticalOffset: 2.5
        shadowOpacity: 1
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

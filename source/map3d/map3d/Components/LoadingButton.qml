import QtQuick
import QtQuick.Controls

import "../style"

Button {
    id: button
    property alias loadingRec: loadingRec
    property alias loadingRecVisible: loadingRec.visible
    property alias loadingTimer: loadingTimer
    property alias buttonText: buttonText.text
    property alias buttonTextColor: buttonText.color
    property alias textId: buttonText
    property alias buttonColor: backgroundRec.color
    property alias backgroundColorOpacity: backgroundRec.color.a
    property alias waitingTimer: waitingTimer
    property alias opacityAnimation: opacityAnimation

    Timer {
        id: waitingTimer
        interval: 2000
        onTriggered: {
            buttonText.text = "Connect"
            backgroundRec.color = Style.backgroundColor
            backgroundRec.color = Style.foregroundColor
            reverseAnimation.start()
        }
    }

    PropertyAnimation {
        id: opacityAnimation
        target: button
        property: "backgroundColorOpacity"
        from: 0
        to: 0.2
        duration: 100
        onFinished: {
            waitingTimer.start()
        }
    }

    PropertyAnimation {
        id: reverseAnimation
        target: button
        property: "backgroundColorOpacity"
        from: 0.2
        to: 1
        duration: 100
        onFinished: {
            backgroundColorOpacity = 1
            button.enabled = true
            button.hoverEnabled = true
        }
    }

    Timer {
        id: loadingTimer
        interval: 500
        onTriggered: {
            var loopEnd = false
            if (loadingRec.anchors.leftMargin == -2 / Style.monitorRatio) {
                loadingRec.anchors.topMargin = -2 / Style.monitorRatio
                loadingRec.anchors.leftMargin = 0
                loopEnd = true
                loadingTimer.start()
            }
            if (loadingRec.anchors.topMargin == 2 / Style.monitorRatio) {
                loadingRec.anchors.leftMargin = -2 / Style.monitorRatio
                loadingRec.anchors.topMargin = 0
                loadingTimer.start()
            }
            if (loadingRec.anchors.leftMargin == 2 / Style.monitorRatio) {
                loadingRec.anchors.topMargin = 2 / Style.monitorRatio
                loadingRec.anchors.leftMargin = 0
                loadingTimer.start()
            }
            if (loadingRec.anchors.topMargin == -2 / Style.monitorRatio
                    && loopEnd === false) {
                loadingRec.anchors.leftMargin = 2 / Style.monitorRatio
                loadingRec.anchors.topMargin = 0
                loadingTimer.start()
            }
        }
    }

    background: Rectangle {
        id: backgroundRec
        radius: backgroundRec.height / 2

        Rectangle {
            id: loadingRec
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: parent.height
            radius: parent.radius
            color: Style.foregroundColor
            opacity: 0.5
            z: parent.z - 1
            visible: loadingRecVisible
        }
    }

    contentItem: Text {
        id: buttonText
        color: Style.backgroundColor
        font.pixelSize: Style.regularFontSize
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import "../style"

Item {
    property alias loadingRec: loadingRec
    property alias loadingTimer: loadingTimer
    property alias buttonText: buttonText.text
    property alias buttonTextColor: buttonText.color
    property alias textId: buttonText
    property alias buttonColor: backgroundRec.color
    property alias backgroundColorOpacity: backgroundRec.color.a
    property alias button: button

    function resetSignInBtn() {
        buttonColor = Style.foregroundColor
        loadingRec.anchors.leftMargin = 0
        loadingRec.anchors.topMargin = 0
        loadingTimer.stop()
    }
    function startButtonLoading() {
        loadingRec.anchors.topMargin = -2 / Style.monitorRatio
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
    Button {
        id: button
        anchors.fill: parent
        hoverEnabled: true
        z: 1
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
            }
        }

        contentItem: Text {
            id: buttonText
            color: Style.backgroundColor
            font.pixelSize: Style.regularFontSize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Keys.onReturnPressed: {
            button.clicked()
        }
    }

    MultiEffect {
        id: shadow
        source: button
        z: button.z - 1
        enabled: true
        anchors.fill: button
        shadowColor: "black"
        shadowEnabled: button.hovered && button.enabled ? true : false
        shadowHorizontalOffset: 10 / Style.monitorRatio
        shadowVerticalOffset: 10 / Style.monitorRatio
        shadowBlur: 1
        shadowOpacity: 1
        shadowScale: 0.98
        paddingRect: Qt.rect(button.x, button.y, button.width, button.height)
    }
}

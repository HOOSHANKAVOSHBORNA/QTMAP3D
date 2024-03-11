import QtQuick
import QtQuick.Controls

import "../style"

Button {
    id: button
    property alias loadingRec: loadingRec
    property alias loadingTimer: loadingTimer

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
        color: Style.foregroundColor
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
}

import QtQuick
import QtQuick.Controls

import "../style"

Button {
    id: button
    property alias loadingRec: loadingRec
    property alias loadingTimer: loadingTimer

    //    property bool isWaiting: false
    //    property alias loadingAnimation: loadingAnimation

    //    readonly property color foregroundColor‌Border: Qt.rgba(
    //                                                        Style.foregroundColor.r,
    //                                                        Style.foregroundColor.g,
    //                                                        Style.foregroundColor.b,
    //                                                        0.20)

    //    Rectangle {
    //        id: animationRect
    //        width: 3
    //        height: 3
    //        radius: width / 2
    //        color: "blue"
    //        visible: isWaiting
    //    }
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

        //        border.color: foregroundColor‌Border
        //        border.width: isWaiting ? 3 : 0
        Rectangle {
            id: loadingRec
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: parent.height
            radius: parent.radius
            color: "red"
            opacity: 0.5
            z: parent.z - 1
        }
    }

    //    PathAnimation {
    //        id: loadingAnimation
    //        target: animationRect
    //        loops: Animation.Infinite
    //        duration: 2500

    //        path: Path {
    //            startX: backgroundRec.x + backgroundRec.radius
    //            startY: backgroundRec.y - animationRect.width + backgroundRec.border.width
    //            PathLine {
    //                x: backgroundRec.x + backgroundRec.width - backgroundRec.radius
    //                y: backgroundRec.y - animationRect.width + backgroundRec.border.width
    //            }
    //            PathArc {
    //                x: backgroundRec.x + backgroundRec.width - backgroundRec.radius
    //                y: backgroundRec.y + backgroundRec.height - backgroundRec.border.width
    //                radiusX: backgroundRec.radius - (animationRect.radius) - backgroundRec.border.width
    //                radiusY: backgroundRec.radius - backgroundRec.border.width
    //            }
    //            PathLine {
    //                x: backgroundRec.x + backgroundRec.radius
    //                y: backgroundRec.y + backgroundRec.height - backgroundRec.border.width
    //            }
    //            PathArc {
    //                x: backgroundRec.x + backgroundRec.radius
    //                y: backgroundRec.y - animationRect.height + backgroundRec.border.width
    //                radiusX: backgroundRec.radius + (animationRect.radius) - backgroundRec.border.width
    //                radiusY: backgroundRec.radius - backgroundRec.border.width
    //            }
    //        }
    //    }
}

import QtQuick

Item {
    Rectangle {
        id: blueRect
        width: 5
        height: 5
        radius: width / 2
        color: "blue"
    }

    Rectangle {
        id: redRect
        width: 343
        height: 40
        radius: height / 2
        color: "red"
        opacity: 0.5
        anchors.centerIn: parent

        border.color: Qt.rgba(255, 255, 0, 1)
        border.width: 5
    }

    PathAnimation {
        target: blueRect
        loops: Animation.Infinite
        running: true
        duration: 1000

        path: Path {
            startX: redRect.x + redRect.radius
            startY: redRect.y - blueRect.width + redRect.border.width
            PathLine {
                x: redRect.x + redRect.width - redRect.radius
                y: redRect.y - blueRect.width + redRect.border.width
            }
            PathArc {
                x: redRect.x + redRect.width - redRect.radius
                y: redRect.y + redRect.height - redRect.border.width
                radiusX: redRect.radius - (blueRect.radius) - redRect.border.width
                radiusY: redRect.radius - redRect.border.width
            }
            PathLine {
                x: redRect.x + redRect.radius
                y: redRect.y + redRect.height - redRect.border.width
            }
            PathArc {
                x: redRect.x + redRect.radius
                y: redRect.y - blueRect.height + redRect.border.width
                radiusX: redRect.radius + (blueRect.radius) - redRect.border.width
                radiusY: redRect.radius - redRect.border.width
            }
        }
    }
}

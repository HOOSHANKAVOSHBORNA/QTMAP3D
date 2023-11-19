import QtQuick 2.15
import QtQuick.Shapes
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: rootItem
    // properties
    property string cutText: 'cutText'
    property color cutBackgroundColor: 'transparent'
    property color cutIconColor: 'blue'
    property color cutIconHoverColor: 'lightblue'
    property string cutIconSource: 'qrc:/Resources/hand.png'
    property double cutOuterRadius: 400
    property double cutInnerRadius: 300
    property double cutIconSize: 30

    required property double cutStartAngle
    property double cutLen: 90

    property alias checkable: cutButton.checkable
    property alias checked: cutButton.checked

    // functions
    function toDegree (radianAngle) {
        return radianAngle * Math.PI / 180;
    }

    // signals
    signal clicked()
    signal toggled()

    anchors.fill: parent

    Shape {
        id: cMenuShape

        anchors.centerIn: parent
        containsMode: Shape.FillContains

        // ------------------------ outer circle
//        ShapePath {
//            id: cutOuter
////            strokeWidth: 1
//            strokeColor: 'transparent'

//            fillColor: cutBackgroundColor

//            PathAngleArc {
//                centerX: 0
//                centerY: 0
//                radiusX: cutOuterRadius
//                radiusY: cutOuterRadius
//                startAngle: cutStartAngle
//                sweepAngle: cutLen
//            }
//            PathLine {
//                x: 0
//                y: 0
//            }
//        }

        // ------------------------ inner circle
//        ShapePath {
//            //            strokeWidth: 4
//                        strokeColor: 'transparent'

//            fillColor: 'transparent'

//            PathAngleArc {
//                centerX: 0
//                centerY: 0
//                radiusX: cutInnerRadius
//                radiusY: cutInnerRadius
//                startAngle: cutStartAngle
//                sweepAngle: cutLen
//            }
//            PathLine {
//                x: 0
//                y: 0
//            }
//        }

        Button {
            id: cutButton
            padding: 0
            width: cutIconSize
            height: cutIconSize

            anchors {
                centerIn: parent
                horizontalCenterOffset: (cutInnerRadius + cutOuterRadius) / 2 * Math.cos(toDegree(cutStartAngle + cutLen / 2))
                verticalCenterOffset: (cutInnerRadius + cutOuterRadius) / 2 * Math.sin(toDegree(cutStartAngle + cutLen / 2))
            }

            //            rotation: 90 + (2 * cutStartAngle + cutLen) / 2

            icon {
                source: cutIconSource
                width: cutIconSize
                height: cutIconSize
                color: hovered ? cutIconHoverColor : cutIconColor
            }

            hoverEnabled: true

            display: AbstractButton.IconOnly

            background: Rectangle {
                color: 'transparent'
            }

            onClicked: rootItem.clicked()
            onToggled: rootItem.toggled()

            PropertyAnimation on width {
                from: 0
                to: cutLen
                duration: 1000
                easing.type: Easing.OutExpo
            }

            ToolTip {
                visible: cutButton.hovered

                contentItem: Text {
                    id: tooltipText
                    anchors.margins: 7
                    text: cutText
                    font.pixelSize: 17
                    color: 'white'
                }

                background: Rectangle {
                    visible: cutButton.hovered
                    height: tooltipText.height + 14
                    width: tooltipText.contentWidth + 14
                    radius: 10

                    color: "#01AED6"
                }
            }
        }
    }
}

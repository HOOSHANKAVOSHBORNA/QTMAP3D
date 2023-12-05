import QtQuick 2.15
import QtQuick.Shapes
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: rootItem
    // properties
    property string text: 'text'
    property color backgroundColor: '#dee3e6'
    property color iconColor: '#003569'
    property color iconHoverColor: '#01AED6'
    property string iconSource: 'qrc:/Resources/hand.png'
    property string iconSourceChecked: 'qrc:/Resources/hand.png'
    property double outerRadius: 400
    property double innerRadius: 300
    property double iconSize: 35

    required property double startAngle
    property double len: 90

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

        //            fillColor: backgroundColor

        //            PathAngleArc {
        //                centerX: 0
        //                centerY: 0
        //                radiusX: outerRadius
        //                radiusY: outerRadius
        //                startAngle: startAngle
        //                sweepAngle: len
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
        //                radiusX: innerRadius
        //                radiusY: innerRadius
        //                startAngle: startAngle
        //                sweepAngle: len
        //            }
        //            PathLine {
        //                x: 0
        //                y: 0
        //            }
        //        }

        Button {
            id: cutButton
            x: 0
            y: 0
            padding: 0
            width: hovered ? iconSize + 10 : iconSize
            height: hovered ? iconSize + 10 : iconSize
            hoverEnabled: true

            anchors {
                centerIn: parent
                margins: 0
                horizontalCenterOffset: (innerRadius + outerRadius) / 2 * Math.cos(toDegree(startAngle + len / 2))
                verticalCenterOffset: (innerRadius + outerRadius) / 2 * Math.sin(toDegree(startAngle + len / 2))
            }

            //            rotation: 90 + (2 * startAngle + len) / 2

            icon {
                source: checkable && checked ? iconSourceChecked : iconSource
                width: hovered ? iconSize + 10 : iconSize
                height: hovered ? iconSize + 10 : iconSize
//                color: !checkable ? "transparent" : checked ? 'white' : "#01AED6"
            }

            display: AbstractButton.IconOnly

            background: Rectangle {
                id: btnBck
                x: -5
                y: -5
                width: iconSize + 10
                height: iconSize + 10
                color: 'transparent'/*backgroundColor*/
                radius: width / 2
            }

            onClicked: rootItem.clicked()
            onToggled: rootItem.toggled()

            PropertyAnimation on width {
                from: 0
                to: len
                duration: 1000
                easing.type: Easing.OutExpo
            }

            ToolTip {
                visible: cutButton.hovered

                contentItem: Text {
                    id: tooltipText
                    anchors.margins: 7
                    text: rootItem.text
                    font.pixelSize: 15
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
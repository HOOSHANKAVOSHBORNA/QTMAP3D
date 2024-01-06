import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../style"

Item {
    id: rootItem

    property color backgroundColor: '#000000'

    signal valueChanged(int newValue)

    Slider {
        id: control
        value: 0.5
        anchors.fill: parent

        background: Rectangle {
            x: control.leftPadding
            y: control.topPadding + control.availableHeight / 2 - height / 2

            radius: height / 2
            color: "transparent"
            border.color: Style.foregroundColor
            border.width: 1

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0.0
                    color: "#00" + backgroundColor.toString().replace("#", "")
                }
                GradientStop {
                    position: 1.0
                    color: "#FF" + backgroundColor.toString().replace("#", "")
                }
            }

            //            Rectangle {
            //                id: filledRectangle
            //                width: control.visualPosition * parent.width
            //                height: parent.height
            //                color: "red"
            //                radius: 2
            //            }
        }

        handle: Rectangle {
            x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
            y: control.topPadding + control.availableHeight / 2 - height / 2

            height: control.height
            width: control.height

            radius: height / 2
            color: 'transparent'
            border.color: Style.foregroundColor
            border.width: 1
        }

        onValueChanged: rootItem.valueChanged(value)
    }
}

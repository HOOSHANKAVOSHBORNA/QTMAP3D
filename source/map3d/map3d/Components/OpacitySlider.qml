import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../style"

// TODO: this file name does not seem to be very good
Item {
    id: rootItem

    property color backgroundColor: '#000000'
    property double initialValue: 1

    signal valueChanged(double newValue)

    function setColorAlpha(color, alpha) {
        return Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, alpha)

    }

    Slider {
        id: control
        value: rootItem.initialValue
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
                    color:  setColorAlpha(backgroundColor,0)
                }
                GradientStop {
                    position: 1.0
                    color: setColorAlpha(backgroundColor,1)
                }
            }

            //            Rectangle {
            //                id: filledRectangle
            //                width: control.visualPosition * parent.width
            //                height: parent.height
            //                color: "red"
            //                radius: height / 2
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

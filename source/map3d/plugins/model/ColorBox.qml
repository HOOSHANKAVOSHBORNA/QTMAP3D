import QtQuick.Effects
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick

Item{
    id:colorBox
    anchors.fill: parent

    property color colorValue: "#FFFF0000"
    property real opacityValue: 0.5
    property color colorFromColor: colorValue


    function colorOpacityUpdate(){
        colorBox.colorValue.a = colorBox.opacityValue
        colorBox.colorFromColor.a = 1.0

    }

    onColorValueChanged: {
        colorBox.opacityValue = colorBox.colorValue.a
    }


    Slider{
        id: sliderItem
        value: colorBox.opacityValue
        anchors.fill: parent
        onMoved:{
            colorBox.opacityValue = value
            colorBox.colorOpacityUpdate()
        }

        background:Rectangle{
            id: colorRecBorder
            anchors.fill: colorRec
            radius: colorBox.height
            border.width: 2
            border.color: "white"
            color: "transparent"
            Rectangle{
                id: colorRec
                anchors.centerIn: parent
                width: colorBox.width-3
                height: colorBox.height-3
                radius: colorBox.height-2
                gradient: Gradient{
                    orientation: Gradient.Horizontal
                    GradientStop{color:"#00000000";position: 0.0 }
                    GradientStop{color:colorBox.colorFromColor;position: 1.0 }
                }
                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        colorDialog.visible = true
                    }
                }
            }
        }

        handle: Rectangle{
            x: sliderItem.leftPadding + sliderItem.visualPosition * (sliderItem.availableWidth - width)
            id: indicator1color
            width: colorRecBorder.height
            border.width: 1
            border.color: colorRecBorder.border.color
            height: width
            color: colorBox.colorValue
            radius: width
            Text {
                id: indicatorText
                text: (colorBox.opacityValue*100).toFixed(0).toString()
                anchors.centerIn: parent
                font.pixelSize: parent.height *0.4
                color: "white"
            }
        }
    }
    ColorDialog{
        id: colorDialog
        visible: false
        onAccepted: {
            colorBox.colorValue = colorDialog.selectedColor
            colorBox.colorOpacityUpdate();
        }
    }
}


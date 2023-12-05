import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Effects
import "style"

RowLayout{

    property alias value:floatSpinBox.value
    property alias from: floatSpinBox.from
    property alias to: floatSpinBox.to
    property alias decimals: floatSpinBox.decimals
    property alias editable: floatSpinBox.editable

        ComboBox {
            id:stepCombobox
            implicitWidth: 69
            implicitHeight: 20
            model: [1000, 100, 10, 1, 0.1, 0.05, 0.01 ]
            currentIndex: 3
            font.pointSize: 10
            displayText:model[currentIndex].toString()

            contentItem: Rectangle{
                 color: "transparent"
                 radius: 15
                 Text{
                     text: stepCombobox.displayText
                     color: Style.foregroundColor
                     anchors.centerIn: parent
                 }
            }

            background: Rectangle{
                color: backgroundColor
                       radius: 15
                           IconImage{
                               width: 15
                               height: 15
                               source: "qrc:/Resources/down.png"
                               anchors.right: parent.right
                               anchors.verticalCenter: parent.verticalCenter
                               z:3
                           }
            }
            indicator: Item{

            }
        }

        FloatSpinbox{
              id:floatSpinBox
              stepSize: stepCombobox.currentValue
              Layout.fillWidth: true
        }

}





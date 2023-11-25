import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"


FloatSpinbox{
    id:stepSpinBox
    property alias stepComboboxVisible: stepCombobox.visible
    stepSize: stepCombobox.currentValue
    contentItem:RowLayout {
        spacing: 0
        ComboBox {
            id:stepCombobox
            Layout.preferredWidth: 45
            Layout.fillHeight: true
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

                color: foregroundColor
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
       Rectangle{
           color: foregroundColor
           radius: 15
           implicitHeight: 20
           Layout.fillWidth: true
           Layout.alignment: Qt.AlignVCenter
           Layout.leftMargin: 5
           RowLayout{
               spacing: 0
               anchors.fill: parent
               Button{
                   implicitHeight: 15
                   implicitWidth: 15
                   Layout.alignment: Qt.AlignVCenter
                   background:Rectangle{
                       color: "transparent"
                       Image {
                          anchors.fill: parent
                          source: "qrc:/Resources/minus.png"
                          }
                   }
                   onClicked: decrement()
               }

               TextInput{
                           id:textInput
                           text: textFromValue()
                           color: Style.foregroundColor
                           selectionColor: "#21be2b"
                           selectedTextColor: "#ffffff"
                           Layout.alignment: Qt.AlignCenter
                           //Layout.fillWidth: true
                           readOnly: !editable
                           validator: validator
                           inputMethodHints: Qt.ImhFormattedNumbersOnly
                           onAccepted: {
                               value = text
                           }
                       }

               Button{
                  implicitHeight: 15
                  implicitWidth: 15
                   Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                   background: Rectangle{
                       color: "transparent"
                       Image {
                          anchors.fill: parent
                          source: "qrc:/Resources/add.png"
                          }
                   }

                   onClicked: increment()
               }
           }
       }
    }

    up.indicator: Item {

    }
    down.indicator: Item{

    }
    background: Rectangle{
        color: foregroundColor
        radius: 15
    }
}





import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"


FloatSpinbox{
    id: spinBox
        stepSize: stepCombobox.currentValue * decimalFactor
                contentItem: RowLayout {
                    anchors.left: downIndicator.right
                    anchors.bottom: downIndicator.bottom
                    ComboBox {
                        id:stepCombobox
                        Layout.preferredWidth: 45
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignCenter
                        model: [1000, 100, 10, 1, 0.1, 0.05, 0.01 ]
                        currentIndex: 3
                        font.pointSize: 10
                        indicator: Item{

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

                    }
                    TextInput {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignCenter
                        z: 2
                        text: spinBox.textFromValue(spinBox.value, spinBox.locale)

                        font: spinBox.font
                        color: Style.foregroundColor
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        onAccepted: {
                            print(text)
                            spinBox.value = spinBox.valueFromText(text, spinBox.locale)
                        }

                        readOnly: !spinBox.editable
                        validator: spinBox.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }
                }
}

//      SpinBox {
//          id: spinBox
//      //    anchors.centerIn: parent
//          editable: true
//          width: 200

//          readonly property color foregroundColor: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.20)
//          readonly property color backgroundColor: Qt.rgba(Style.backgroundColor.r, Style.backgroundColor.g, Style.backgroundColor.b, 0.30)

//          property int decimals: 3
//          readonly property int decimalFactor: Math.pow(10, decimals)
//          property real realValue: value / decimalFactor
//          Binding{
//              target: spinBox
//              property: "value"
//              value: spinBox.realValue* spinBox.decimalFactor
//              delayed: true
//          }

//          stepSize: stepCombobox.currentValue * decimalFactor
//          from: 0
//          to: spinBox.to * decimalFactor
//          value: 0

//          validator: DoubleValidator {
//              bottom: Math.min(spinBox.from, spinBox.to)
//              top:  Math.max(spinBox.from, spinBox.to)
//          }

//          textFromValue: function(value, locale) {
//              return Number(value / decimalFactor).toLocaleString(locale, 'f', spinBox.decimals)
//          }

//          valueFromText: function(text, locale) {
//              return Number.fromLocaleString(locale, text) * decimalFactor
//          }
//          onValueChanged:{
//              realValue = value / decimalFactor
//          }

//            contentItem: RowLayout {
//                anchors.left: downIndicator.right
//                anchors.bottom: downIndicator.bottom
//                ComboBox {
//                    id:stepCombobox
//                    Layout.preferredWidth: 45
//                    Layout.fillHeight: true
//                    model: [1000, 100, 10, 1, 0.1, 0.05, 0.01 ]
//                    currentIndex: 3
//                    font.pointSize: 10
//                    indicator: Item{

//                    }
//                    background: Rectangle{

//                        color: foregroundColor
//                               radius: 15
//                                   IconImage{
//                                       width: 15
//                                       height: 15
//                                       source: "qrc:/Resources/down.png"
//                                       anchors.right: parent.right
//                                       anchors.verticalCenter: parent.verticalCenter
//                                       z:3
//                                   }
//                    }

//                }
//                TextInput {
//                    Layout.fillHeight: true
//                    Layout.fillWidth: true
//                    z: 2
//                    text: spinBox.textFromValue(spinBox.value, spinBox.locale)

//                    font: spinBox.font
//                    color: Style.foregroundColor
//                    selectionColor: "#21be2b"
//                    selectedTextColor: "#ffffff"
//                    horizontalAlignment: Qt.AlignHCenter
//                    verticalAlignment: Qt.AlignVCenter
//                    onAccepted: {
//                        print(text)
//                        spinBox.value = spinBox.valueFromText(text, spinBox.locale)
//                    }

//                    readOnly: !spinBox.editable
//                    validator: spinBox.validator
//                    inputMethodHints: Qt.ImhFormattedNumbersOnly
//                }
//            }


//                  up.indicator:Rectangle{
//                      anchors.right: spinBox.right
//                      anchors.verticalCenter: spinBox.verticalCenter
//                      width: 20
//                      height: 20
//                      color:"transparent"
//                      Image{
//                      source: "qrc:/Resources/add.png"
//                      anchors.fill: parent
//                  }
//                  }

//                  down.indicator:Rectangle{
//                      id:downIndicator
//                      anchors.left: spinBox.left
//                      anchors.verticalCenter: spinBox.verticalCenter
//                      width: 20
//                      height: 20
//                      color:"transparent"

//                      Image{
//                           source: "qrc:/Resources/minus.png"
//                           anchors.fill: parent
//                          }
//                  }


//                  background: Rectangle {
//                      color: foregroundColor
//                      radius: 15
//                  }
//      }









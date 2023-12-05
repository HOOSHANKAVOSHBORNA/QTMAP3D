import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

    Rectangle{
        id:colorBoxInput
        width: 288/ Style.monitorRatio
        height: 371/ Style.monitorRatio
        radius: 20/ Style.monitorRatio
        color: Style.backgroundColor
        property int oldIndex: 0
        property int newIndex: 0
        property int historyOldIndex: 0
        property int historyNewIndex: 0
        property color selectedColor : Style.backgroundColor
        property color opacityColor: Style.backgroundColor
        signal colorChosen()

        MouseArea{
            anchors.fill: parent
        }

        function textUpdate(){
            rInput.text = parseInt(colorCircle.color.toString().replace("#","").substring(0,2),16)
            gInput.text = parseInt(colorCircle.color.toString().replace("#","").substring(2,4),16)
            bInput.text = parseInt(colorCircle.color.toString().replace("#","").substring(4,6),16)
            hexInput.text = colorCircle.color.toString().replace("#","")
        }

        function addHistory(addColor){
          var counter = history.count
            if(counter <= 9){
                if(!counter)
                    history.insert(0,{"historyColorSelect":addColor.toString(),"historyImageVisible":false})
                else{
                    for(let listIndex = 0; listIndex < counter; listIndex++)
                        if(history.get(listIndex).historyColorSelect === addColor.toString())
                        {
                         history.setProperty(historyOldIndex,"historyImageVisible",false)
                         return
                        }
                          history.insert(0,{"historyColorSelect":addColor.toString(),"historyImageVisible":false})
                          historyOldIndex++
                          if(historyOldIndex === 9)
                              historyOldIndex = 0
                }
            }
             if(history.count === 10){
                history.remove(9)
            }
          history.setProperty(historyOldIndex,"historyImageVisible",false)
        }



        ListModel{
            id:history
        }
        ColumnLayout{
            anchors.fill: parent
            spacing: 0
            Text{
                text: "Select Color"
                font.pixelSize: 17 / Style.monitorRatio
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin: 15 / Style.monitorRatio
                Layout.topMargin: 15 / Style.monitorRatio
                Layout.bottomMargin: 10 / Style.monitorRatio
            }

            ListModel{
                id:lstModel

                ListElement {imageVisible:false; colorSelect: "#EF2929" }   ListElement {imageVisible:false; colorSelect: "#FCAF3E" } ListElement {imageVisible:false; colorSelect: "#FCE94F" }
                ListElement {imageVisible:false; colorSelect: "#8AE234" } ListElement {imageVisible:false; colorSelect: "#729FCF" }   ListElement {imageVisible:false; colorSelect: "#AD7FA8" }
                ListElement {imageVisible:false; colorSelect: "#E9B96E" }   ListElement {imageVisible:false; colorSelect: "#888A85" } ListElement {imageVisible:false; colorSelect: "#EEEEEC" }

                ListElement {imageVisible:false; colorSelect: "#C00" } ListElement {imageVisible:false; colorSelect: "#F57900" }   ListElement {imageVisible:false; colorSelect: "#EDD400" }
                ListElement {imageVisible:false; colorSelect: "#73D216" }   ListElement {imageVisible:false; colorSelect: "#346594" } ListElement {imageVisible:false; colorSelect: "#75507B" }
                ListElement {imageVisible:false; colorSelect: "#C17D11" } ListElement {imageVisible:false; colorSelect: "#555753" }   ListElement {imageVisible:false; colorSelect: "#D3D7CF" }

                ListElement {imageVisible:false; colorSelect: "#A40000" }   ListElement {imageVisible:false; colorSelect: "#CE5C00" } ListElement {imageVisible:false; colorSelect: "#C4A000" }
                ListElement {imageVisible:false; colorSelect: "#4E9A06" } ListElement {imageVisible:false; colorSelect: "#204A87" }   ListElement {imageVisible:false; colorSelect: "#5C3566" }
                ListElement {imageVisible:false; colorSelect: "#8F5902" }   ListElement {imageVisible:false; colorSelect: "#2E3436" } ListElement {imageVisible:false; colorSelect: "#BABDB6" }

            }
            GridLayout{
                Layout.fillWidth: true
                Layout.leftMargin: 15 / Style.monitorRatio
                Layout.bottomMargin: 15 / Style.monitorRatio
                rowSpacing: 3 / Style.monitorRatio
                columnSpacing: 3 / Style.monitorRatio
                rows: 3
                columns: 9
                Repeater{
                    id:lstModelRepeater
                    model: lstModel
                    Button{
                              required property color colorSelect
                              required property bool imageVisible
                              required property int index
                              implicitWidth: 26 / Style.monitorRatio
                              implicitHeight: 26 / Style.monitorRatio
                              checkable: true

                              background: Rectangle{
                                  radius: width
                                  color: colorSelect
                              }

                              Image {
                                  id:checkIcon
                                  anchors.centerIn: parent
                                  anchors.fill: parent
                                  source: "qrc:/Resources/add-place-color-select.png"
                                  visible: imageVisible
                              }
                                  onClicked: {
                                      /*to clear the previous index after selecting a new one*/
                                      lstModel.setProperty(oldIndex,"imageVisible",false)
                                      newIndex = index
                                      if(oldIndex !== newIndex){
                                          lstModelRepeater.itemAt(oldIndex).checked = false
                                      }
                                      lstModel.setProperty(newIndex,"imageVisible",checked)
                                      if(checked){
                                          opacityColor = colorSelect
                                          selectedColor = colorSelect
                                          addHistory(colorSelect)
                                          oldIndex = index
                                      }
                                      else{
                                          opacityColor = "#FFFFFF"
                                          selectedColor = "#FFFFFF"
                                      }
                                  }
                    }
                }
            }

            Text{
                text: "Custom"
                font.pixelSize: 15 / Style.monitorRatio
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin: 15 / Style.monitorRatio
                Layout.bottomMargin: 5 / Style.monitorRatio
            }
            RowLayout{
                Layout.leftMargin: 15 / Style.monitorRatio
                Layout.bottomMargin: 15 / Style.monitorRatio
                spacing: 0
                Rectangle{
                    id:colorCircle
                    width: 26 / Style.monitorRatio
                    height: 26 / Style.monitorRatio
                    radius: 26
                    border.color: Style.foregroundColor
                    border.width: 1 / Style.monitorRatio
                }
                Text{
                    text: "HEX"
                    font.pixelSize: 12 / Style.monitorRatio
                    Layout.leftMargin: 9 / Style.monitorRatio
                    color: Style.foregroundColor
                }
                Rectangle{
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.preferredWidth: 60 / Style.monitorRatio
                    Layout.leftMargin: 5 / Style.monitorRatio
                    border.color: "black"
                    border.width: 1 / Style.monitorRatio
                    color:Style.backgroundColor
                    radius: 5 / Style.monitorRatio
                    TextInput{
                      id:hexInput
                      anchors.fill: parent
                      font.pixelSize: 12 / Style.monitorRatio
                      horizontalAlignment: Text.AlignHCenter
                      verticalAlignment: Text.AlignVCenter
                      validator: RegularExpressionValidator {
                         regularExpression: /[0-9A-F]{1,6}/i
                      }
                      onAccepted: {
                          colorCircle.color = "#" + "0".repeat(6 - text.length) + text
                          opacityColor = colorCircle.color
                          selectedColor = colorCircle.color
                          rInput.text = parseInt(colorCircle.color.toString().replace("#","").substring(0,2),16)
                          gInput.text = parseInt(colorCircle.color.toString().replace("#","").substring(2,4),16)
                          bInput.text = parseInt(colorCircle.color.toString().replace("#","").substring(4,6),16)
                          addHistory(colorCircle.color)
                          lstModel.setProperty(oldIndex,"imageVisible",false)
                      }
                    }
                }
                Text{
                    text: "R"
                    font.pixelSize: 12/ Style.monitorRatio
                    Layout.leftMargin: 15 / Style.monitorRatio
                    color: Style.foregroundColor
                }
                Rectangle{
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.preferredWidth: 20 / Style.monitorRatio
                    Layout.leftMargin: 5 / Style.monitorRatio
                    border.color: "black"
                    border.width: 1 / Style.monitorRatio
                    color:Style.backgroundColor
                    radius: 5 / Style.monitorRatio
                    TextInput{
                      id:rInput
                      anchors.fill: parent
                      font.pixelSize: 12 / Style.monitorRatio
                      horizontalAlignment: Text.AlignHCenter
                      verticalAlignment: Text.AlignVCenter
                      validator: IntValidator{
                          bottom: 0
                          top: 255
                      }
                      onAccepted: {
                          var subStringValue = colorCircle.color.toString().substring(3,7)
                          colorCircle.color = "#" + "0".repeat(2 - parseInt(text, 10).toString(16).length) +
                                  parseInt(text, 10).toString(16) + subStringValue
                          opacityColor = colorCircle.color
                          selectedColor = colorCircle.color
                          hexInput.text = colorCircle.color.toString().replace("#","")
                          addHistory(colorCircle.color)
                          lstModel.setProperty(oldIndex,"imageVisible",false)
                      }
                    }
                }
                Text{
                    text: "G"
                    font.pixelSize: 12/ Style.monitorRatio
                    Layout.leftMargin: 10 / Style.monitorRatio
                    color: Style.foregroundColor
                }
                Rectangle{
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.preferredWidth: 20 / Style.monitorRatio
                    Layout.leftMargin: 5 / Style.monitorRatio
                    border.color: "black"
                    border.width: 1 / Style.monitorRatio
                    color:Style.backgroundColor
                    radius: 5/ Style.monitorRatio
                    TextInput{
                      id:gInput
                      anchors.fill: parent
                      font.pixelSize: 12 / Style.monitorRatio
                      horizontalAlignment: Text.AlignHCenter
                      verticalAlignment: Text.AlignVCenter
                      validator:IntValidator{
                          bottom: 0
                          top: 255
                      }
                      onAccepted: {
                          var subStringValue1 = colorCircle.color.toString().substring(1,3)
                          var subStringValue2 = colorCircle.color.toString().substring(5,7)
                          colorCircle.color = "#" + subStringValue1 + "0".repeat(2 - parseInt(text, 10).toString(16).length) +
                                  parseInt(text, 10).toString(16) + subStringValue2
                          opacityColor = colorCircle.color
                          selectedColor = colorCircle.color
                          hexInput.text = colorCircle.color.toString().replace("#","")
                          addHistory(colorCircle.color)
                          lstModel.setProperty(oldIndex,"imageVisible",false)
                      }
                    }
                }
                Text{
                    text: "B"
                    font.pixelSize: 12/ Style.monitorRatio
                    Layout.leftMargin: 10 / Style.monitorRatio
                    color: Style.foregroundColor
                }
                Rectangle{
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.preferredWidth: 20 / Style.monitorRatio
                    Layout.leftMargin: 5 / Style.monitorRatio
                    border.color: "black"
                    border.width: 1 / Style.monitorRatio
                    color:Style.backgroundColor
                    radius: 5/ Style.monitorRatio
                    TextInput{
                      id:bInput
                      anchors.fill: parent
                      font.pixelSize: 12 / Style.monitorRatio
                      horizontalAlignment: Text.AlignHCenter
                      verticalAlignment: Text.AlignVCenter
                      validator: IntValidator{
                          bottom: 0
                          top: 255
                      }
                      onAccepted: {
                          var subStringValue = colorCircle.color.toString().substring(1,5)
                          colorCircle.color = "#" + subStringValue + "0".repeat(2 - parseInt(text, 10).toString(16).length) +
                                  parseInt(text, 10).toString(16)
                          opacityColor = colorCircle.color
                          selectedColor = colorCircle.color
                          hexInput.text = colorCircle.color.toString().replace("#","")
                          addHistory(colorCircle.color)
                          lstModel.setProperty(oldIndex,"imageVisible",false)
                      }
                    }
                }
            }
            Text{
                text: "Opacity"
                font.pixelSize: 15/ Style.monitorRatio
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin: 15 / Style.monitorRatio
                Layout.bottomMargin: 3 / Style.monitorRatio
            }
            RowLayout{
                Layout.leftMargin: 15 / Style.monitorRatio
                Layout.bottomMargin: 15 / Style.monitorRatio
                spacing: 0
                Text{
                    text: Math.trunc(slider.value*100).toString() + "%"
                    color: Style.foregroundColor
                    font.pixelSize: 12 / Style.monitorRatio
                    Layout.preferredWidth: 30 / Style.monitorRatio
                }
                Slider {
                    id: slider
                    value: 1
                    Layout.leftMargin: 5 / Style.monitorRatio
                    background: Rectangle {
                        x: slider.leftPadding
                        y: slider.topPadding + slider.availableHeight / 2 - height / 2
                        implicitWidth: 225 / Style.monitorRatio
                        implicitHeight: 10 / Style.monitorRatio
                        border.width: 1 / Style.monitorRatio
                        border.color: Style.foregroundColor
                        width: slider.availableWidth
                        height: implicitHeight
                        radius: 10 / Style.monitorRatio
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop {
                                position: 0.0
                                color: "#00" + opacityColor.toString().replace("#","")
                            }
                            GradientStop {
                                position: 1.0
                                color: "#FF" + opacityColor.toString().replace("#","")
                            }
                        }
                    }
                    handle: Rectangle {
                        x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                        y: slider.topPadding + slider.availableHeight / 2 - height / 2
                        implicitWidth: 10 / Style.monitorRatio
                        implicitHeight: 10 / Style.monitorRatio
                        radius: 10 / Style.monitorRatio
                        color: Style.backgroundColor
                        border.color: Style.foregroundColor
                    }
                    onMoved: {
                       colorCircle.color.a = value
                    }
                }
            }

            Text{
                text: "Recent Color"
                font.pixelSize: 15/ Style.monitorRatio
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin: 15 / Style.monitorRatio
                Layout.bottomMargin: 10 / Style.monitorRatio
            }


               RowLayout{
                   Layout.leftMargin: 15 / Style.monitorRatio
                   Layout.bottomMargin: 15 / Style.monitorRatio
                   spacing: 3 / Style.monitorRatio
                   Repeater{
                       id:historyRepeater
                       model: history

                     Button{
                       required property color historyColorSelect
                       required property bool historyImageVisible
                       required property int index
                       implicitWidth: 26 / Style.monitorRatio
                       implicitHeight: 26 / Style.monitorRatio
                       checkable: true
                       background: Rectangle{
                           radius: width
                           color: historyColorSelect
                       }

                       Image {
                           id:historyCheckIcon
                           anchors.centerIn: parent
                           anchors.fill: parent
                           source: "qrc:/Resources/add-place-color-select.png"
                           visible: historyImageVisible
                       }
                           onClicked: {
                               history.setProperty(historyOldIndex,"historyImageVisible",false)
                               historyNewIndex = index
                               if(historyOldIndex !== historyNewIndex){
                                   historyRepeater.itemAt(historyOldIndex).checked = false
                               }
                               history.setProperty(historyNewIndex,"historyImageVisible",checked)
                               if(checked){
                                   opacityColor = historyColorSelect
                                   selectedColor = historyColorSelect
                                   historyOldIndex = index
                               }
                               else{
                                   opacityColor = "#FFFFFF"
                                   selectedColor = "#FFFFFF"
                               }
                               lstModel.setProperty(oldIndex,"imageVisible",false)
                           }
                   }
               }
             }

              Item{
                  Layout.fillHeight: true
                  Layout.fillWidth: true
              }
        }

                        Button{
                            id:doneBtn
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.rightMargin: 15 / Style.monitorRatio
                            anchors.bottomMargin: 10 / Style.monitorRatio
                            height: 24 / Style.monitorRatio
                            width: 55 / Style.monitorRatio
                            contentItem: Text{
                                text: "Done"
                                font.pixelSize: 15 / Style.monitorRatio
                                color: Style.backgroundColor
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            background: Rectangle{
                                radius: 10/ Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            onClicked: {
                                selectedColor.a = slider.value
                                colorChosen()
                                colorBoxInput.visible = false
                            }
                        }

                          Button{
                              height: 24 / Style.monitorRatio
                              width: 65 / Style.monitorRatio
                              anchors.rightMargin: 5 / Style.monitorRatio
                              anchors.right: doneBtn.left
                              anchors.bottom: doneBtn.bottom
                              contentItem: Text{
                                  text: "Cancel"
                                  font.pixelSize: 15 / Style.monitorRatio
                                  color: Style.foregroundColor
                                  horizontalAlignment: Text.AlignHCenter
                                  verticalAlignment: Text.AlignVCenter
                              }
                              background: Rectangle{
                                  radius: 10 / Style.monitorRatio
                                  color: Style.backgroundColor
                                  border.width: 1 / Style.monitorRatio
                                  border.color: Style.foregroundColor
                              }
                              onClicked: {
                                colorBoxInput.visible = false
                              }
                          }
                    }


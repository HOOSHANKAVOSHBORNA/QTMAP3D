import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

    Rectangle{
        id:colorBoxInput
        width: 250/ Style.monitorRatio
        height: 370/ Style.monitorRatio
        radius: 15/ Style.monitorRatio
        color: Style.backgroundColor
        property int oldIndex: 0
        property int newIndex: 0
        property int historyOldIndex: 0
        property int historyNewIndex: 0
        property color selectedColor : Style.backgroundColor
        property color opacityColor: Style.backgroundColor
        signal colorChosen()

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

            Text{
                text: "Select Color"
                font.pointSize: 12/ Style.monitorRatio
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin: 5/ Style.monitorRatio
                Layout.topMargin: 5/ Style.monitorRatio
            }

            ListModel{
                id:lstModel
                ListElement {imageVisible:false; colorSelect: "red" }   ListElement {imageVisible:false; colorSelect: "orange" } ListElement {imageVisible:false; colorSelect: "yellow" }
                ListElement {imageVisible:false; colorSelect: "green" } ListElement {imageVisible:false; colorSelect: "blue" }   ListElement {imageVisible:false; colorSelect: "pink" }
                ListElement {imageVisible:false; colorSelect: "red" }   ListElement {imageVisible:false; colorSelect: "orange" } ListElement {imageVisible:false; colorSelect: "yellow" }
                ListElement {imageVisible:false; colorSelect: "green" } ListElement {imageVisible:false; colorSelect: "blue" }   ListElement {imageVisible:false; colorSelect: "pink" }
                ListElement {imageVisible:false; colorSelect: "red" }   ListElement {imageVisible:false; colorSelect: "orange" } ListElement {imageVisible:false; colorSelect: "yellow" }
                ListElement {imageVisible:false; colorSelect: "green" } ListElement {imageVisible:false; colorSelect: "blue" }   ListElement {imageVisible:false; colorSelect: "pink" }
                ListElement {imageVisible:false; colorSelect: "red" }   ListElement {imageVisible:false; colorSelect: "orange" } ListElement {imageVisible:false; colorSelect: "yellow" }
                ListElement {imageVisible:false; colorSelect: "green" } ListElement {imageVisible:false; colorSelect: "blue" }   ListElement {imageVisible:false; colorSelect: "pink" }
                ListElement {imageVisible:false; colorSelect: "red" }   ListElement {imageVisible:false; colorSelect: "orange" } ListElement {imageVisible:false; colorSelect: "yellow" }
            }
            GridLayout{
                Layout.fillWidth: true
                Layout.leftMargin: 5 / Style.monitorRatio
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
                                  width: 17 / Style.monitorRatio
                                  height: 17 / Style.monitorRatio
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
                font.pointSize: 12 / Style.monitorRatio
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin: 5 / Style.monitorRatio
                Layout.topMargin: 10 / Style.monitorRatio
            }
            RowLayout{
                Layout.leftMargin: 5/ Style.monitorRatio
                spacing: 2 / Style.monitorRatio

                Rectangle{
                    id:colorCircle
                  //  color: Style.backgroundColor
                    width: 24 / Style.monitorRatio
                    height: 24 / Style.monitorRatio
                    radius: width/2
                    border.color: Style.foregroundColor
                    border.width: 1 / Style.monitorRatio
                }
                Text{
                    text: "HEX"
                    font.pointSize: 10 / Style.monitorRatio
                    color: Style.foregroundColor
                }
                Rectangle{
                    Layout.preferredHeight: 15 / Style.monitorRatio
                    Layout.preferredWidth: 80 / Style.monitorRatio
                    border.color: "black"
                    border.width: 1 / Style.monitorRatio
                    color:Style.backgroundColor
                    radius: 5/ Style.monitorRatio
                    TextInput{
                      id:hexInput
                      anchors.fill: parent
                  //    text: Style.backgroundColor.toString().replace("#","")
                      font.pointSize: 8 / Style.monitorRatio
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
                    font.pointSize: 10/ Style.monitorRatio
                    color: Style.foregroundColor
                }
                Rectangle{
                    Layout.preferredHeight: 15 / Style.monitorRatio
                    Layout.preferredWidth: 35 / Style.monitorRatio
                    border.color: "black"
                    border.width: 1 / Style.monitorRatio
                    color:Style.backgroundColor
                    radius: 5 / Style.monitorRatio
                    TextInput{
                      id:rInput
                      anchors.fill: parent
                  //    text: Style.backgroundColor.toString().replace("#","").substring(0,2)
                      font.pointSize: 8 / Style.monitorRatio
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
                    font.pointSize: 10/ Style.monitorRatio
                    color: Style.foregroundColor
                }
                Rectangle{
                    Layout.preferredHeight: 15 / Style.monitorRatio
                    Layout.preferredWidth: 35 / Style.monitorRatio
                    border.color: "black"
                    border.width: 1 / Style.monitorRatio
                    color:Style.backgroundColor
                    radius: 5/ Style.monitorRatio
                    TextInput{
                      id:gInput
                      anchors.fill: parent
                   //   text: Style.backgroundColor.toString().replace("#","").substring(2,4)
                      font.pointSize: 8 / Style.monitorRatio
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
                    font.pointSize: 10/ Style.monitorRatio
                    color: Style.foregroundColor
                }
                Rectangle{
                    Layout.preferredHeight: 15 / Style.monitorRatio
                    Layout.preferredWidth: 35 / Style.monitorRatio
                    border.color: "black"
                    border.width: 1 / Style.monitorRatio
                    color:Style.backgroundColor
                    radius: 5/ Style.monitorRatio
                    TextInput{
                      id:bInput
                      anchors.fill: parent
                   //   text: Style.backgroundColor.toString().replace("#","").substring(4,6)
                      font.pointSize: 8 / Style.monitorRatio
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
                font.pointSize: 12/ Style.monitorRatio
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin: 5 / Style.monitorRatio
                Layout.topMargin: 5 / Style.monitorRatio
            }
            RowLayout{
                Layout.leftMargin: 5 / Style.monitorRatio
                Text{
                    text: Math.trunc(slider.value*100).toString() + "%"
                    color: Style.foregroundColor
                    font.pointSize: 10 / Style.monitorRatio
                }
                Slider {
                    id: slider
                    value: 1
                    background: Rectangle {
                        x: slider.leftPadding
                        y: slider.topPadding + slider.availableHeight / 2 - height / 2
                        implicitWidth: 250 / Style.monitorRatio
                        implicitHeight: 10 / Style.monitorRatio
                        border.width: 1 / Style.monitorRatio
                        border.color: Style.foregroundColor
                        width: slider.availableWidth
                        height: implicitHeight
                        radius: 8 / Style.monitorRatio
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop {
                                position: 0.0
                               // color: "#00" + selectedColor.toString().replace("#","")
                                color: "#00" + opacityColor.toString().replace("#","")
                            }
                            GradientStop {
                                position: 1.0
                               // color: "#FF" + selectedColor.toString().replace("#","")
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
                font.pointSize: 12/ Style.monitorRatio
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.leftMargin: 5 / Style.monitorRatio
                Layout.topMargin: 5 / Style.monitorRatio
            }
            GridLayout{
                Layout.fillWidth: true
                Layout.leftMargin: 5 / Style.monitorRatio
                rows:1
                columns: 9
                Repeater{
                    id:historyRepeater
                    model: history

                  Button{
                    required property color historyColorSelect
                    required property bool historyImageVisible
                    required property int index
                    implicitWidth: 24 / Style.monitorRatio
                    implicitHeight: 24 / Style.monitorRatio
                    checkable: true
                    background: Rectangle{
                        radius: width
                        color: historyColorSelect
                    }

                    Image {
                        id:historyCheckIcon
                        anchors.centerIn: parent
                        width: 17 / Style.monitorRatio
                        height: 17 / Style.monitorRatio
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
                        }
                }
            }
          }


            RowLayout{
                Layout.fillWidth: true
                Item{
                    Layout.fillWidth: true
                }

                Button{
                    implicitHeight: 25 / Style.monitorRatio
                    implicitWidth: 50 / Style.monitorRatio
                    Layout.leftMargin: 25 / Style.monitorRatio
                    contentItem: Text{
                        text: "Cancel"
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

                Button{
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 5 / Style.monitorRatio
                    implicitHeight: 25 / Style.monitorRatio
                    implicitWidth: 50 / Style.monitorRatio
                    contentItem: Text{
                        text: "Done"
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
            }

            Item{
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }


import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent ? parent.height : 0



    property BoxProperties boxProperties
    property string headerTitleSTR: "box Properties"
    property string fillColor: "#91001d"
    property string lineColor: "#ffffff"




    onVisibleChanged: {
        boxProperties.color = fillColor
        boxProperties.opacity = opacityValue.value
        boxProperties.width = widthValue.value
        boxProperties.height = heightValue.value
        boxProperties.length = lengthValue.value
    }




    Item {
        id: dialog
        width: 250
        height: rootItem.height


        //////////////////////////Main Content////////////////////////
        Rectangle {
            id: item
            visible: true
            width: 240
            height: rootItem.height
            anchors.fill: parent
            color: "#404040"
            opacity: 0.9
            anchors.margins: 6
            radius: 10

            Rectangle{
                id:header
                width: parent.width -2
                anchors.horizontalCenter: parent.horizontalCenter
                y: 2
                height: 30
                radius: 0
                color: "#404040"

                Text {
                    id: headerTitle
                    text: headerTitleSTR
                    anchors.centerIn: parent
                    font.family: "SourceSansPro"
                    font.pointSize: 14
                    color: "white"
                }
            }

            ScrollView {
                id: frame
                clip: true
                anchors.top: header.bottom
                padding: 5
                width: parent.width
                height: parent.height - header.height
                //                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                Flickable {
                    contentHeight: 400
                    width: 240

                    Rectangle {
                        id : innerContainer
                        color: "transparent"
                        anchors.fill: parent

                        /////////////////////// components Grid ////////////////////////////
                        GridLayout{
                            id: props
                            width: 235
                            y: innerContainer.y +3
                            anchors.horizontalCenter: parent.horizontalCenter
                            columnSpacing: 0
                            rowSpacing: 1
                            columns:2
                            rows: 6
                            layoutDirection: Qt.RightToLeft

                            ////////////////////////////////////fill Color Property//////////////////////////////////
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/1.9
                                color: "#404040"
                                height: 35


                                Rectangle{
                                    height: 20
                                    width: 20
                                    color: fillColor
                                    border.width: 2
                                    border.color: "#c9c9c9"
                                    x: 5
                                    radius: 5
                                    anchors.verticalCenter: parent.verticalCenter

                                    MouseArea{
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: fillColorDialog.visible = true

                                    }
                                }
                                Rectangle{
                                    height: 20
                                    width: 100
                                    color: "#c9c9c9"
                                    x: 30
                                    anchors.verticalCenter: parent.verticalCenter
                                    radius: 5

                                    Text {
                                        id: fillolorField
                                        text: fillColor
                                        anchors.centerIn: parent
                                        font.pointSize: 10

                                    }
                                }
                                ColorDialog {
                                    visible: false
                                    id:  fillColorDialog
                                    title: "Please choose a color"
                                    onAccepted: {
                                        fillColor = fillColorDialog.color
                                        boxProperties.color= fillColor.toString()
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                width: parent.width/3
                                color: "#404040"
                                height: 35


                                Text {
                                    id: fillColorBox
                                    text: qsTr("Color :")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                            ///////////////////////////////////opacity/////////////////////////////////////

                            Rectangle{
                                id: opacityContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35
                                z:10
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                QSpinBox {
                                    id: opacityValue
                                    value: 50
                                    from: 0
                                    to: 100
                                    step: 10
                                    z:10
                                    anchors.fill:parent
                                    onValueChanged: {
                                        if(boxProperties){
                                            boxProperties.opacity = opacityValue.value
                                        }
                                    }
                                }
                            }


                            Rectangle{
                                id: opacityTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    id: opacityBox
                                    text: qsTr("Opacity:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }

                            ///////////////////////////////////Location Property///////////////////////////////
                            Rectangle{
                                id:locationContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 110
                                border.color: "#5f5f5f"
                                border.width: 1
                                z:9

                                Column{
                                    Row{
                                        spacing: 8
                                        //                                        leftPadding: 5
                                        topPadding: 2
                                        z:9
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        //                                        Text {
                                        //                                            color: "#e5e5e5"
                                        //                                            text: qsTr("X:")
                                        //                                            font.pointSize: 10
                                        //                                        }
                                        Rectangle{
                                            height: 35
                                            width: locationContainer.width
                                            color: "#c9c9c9"
                                            //                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            QSpinBox {
                                                id: mlocationX
                                                anchors.fill: parent
                                                step: 0.01
                                                //                                                value: xLoc
                                                showText:  boxProperties ? boxProperties.location.x  : 0
                                                onValueChanged: {
                                                    boxProperties.location.x = value
                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
                                        //                                        leftPadding: 5
                                        //                                        topPadding: 5
                                        z:8
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        //                                        Text {
                                        //                                            color: "#e5e5e5"
                                        //                                            text: qsTr("Y:")
                                        //                                            font.pointSize: 10
                                        //                                        }
                                        Rectangle{
                                            height: 35
                                            width: locationContainer.width
                                            color: "#c9c9c9"
                                            //                                            clip:  true
                                            radius: 5

                                            QSpinBox {
                                                id: mlocationY
                                                anchors.fill: parent
                                                step: 0.01
                                                //                                                value: yLoc
                                                showText : boxProperties ? boxProperties.location.y  : 0
                                                onValueChanged: {
                                                    boxProperties.location.y = value
                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
                                        //                                        leftPadding: 5
                                        //                                        topPadding: 5
                                        z:7
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        //                                        Text {
                                        //                                            color: "#e5e5e5"
                                        //                                            text: qsTr("Y:")
                                        //                                            font.pointSize: 10
                                        //                                        }
                                        Rectangle{
                                            height: 35
                                            width: locationContainer.width
                                            color: "#c9c9c9"
                                            //                                            clip:  true
                                            radius: 5

                                            QSpinBox {
                                                id: mlocationZ
                                                anchors.fill: parent
                                                step: 10
                                                showText : boxProperties ? boxProperties.location.z  : 0
                                                onValueChanged: {
                                                    boxProperties.location.z = value
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id: locationTitle
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 110
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: locationBox
                                    text: qsTr("Location :")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }
                                CheckBox {
                                    id: relative
                                    text: qsTr("Relative")
                                    font.pointSize: 10
                                    checked: false
                                    anchors.bottom: locationTitle.bottom
                                    onCheckStateChanged: if(checked === true){
                                                             boxProperties.relative = true
                                                         }
                                                         else{
                                                             boxProperties.relative = false
                                                         }

                                    indicator: Rectangle {
                                        implicitWidth: 15
                                        implicitHeight: 15
                                        x: relative.leftPadding
                                        y: parent.height / 2 - height / 2
                                        radius: 3
                                        border.color: relative.down ? "black" : "#313131"

                                        Rectangle {
                                            width: 10
                                            height: 10
                                            x: 3
                                            y: 3
                                            radius: 2
                                            color: relative.down ? "black" : "dark green"
                                            visible: relative.checked
                                        }
                                    }
                                    contentItem: Text {
                                        text: relative.text
                                        font: relative.font
                                        opacity: enabled ? 1.0 : 0.3
                                        color: relative.down ? "#black" : "white"
                                        verticalAlignment: Text.AlignVCenter
                                        leftPadding: relative.indicator.width + relative.spacing
                                    }
                                }
                            }

                            ///////////////////////////////////length/////////////////////////////////////
                            Rectangle{
                                id: lengthContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35
                                z:6
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                QSpinBox {
                                    id: lengthValue
                                    value: 40000.00
                                    step: 1000
                                    z:6
                                    anchors.fill:parent
                                    onValueChanged: {
                                        if(boxProperties && lengthValue && (lengthValue.value === 0 || lengthValue.value)){
                                            boxProperties.length = lengthValue.value
                                        }
                                    }

                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30


                                Text {
                                    text: qsTr("Length:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }


                            ///////////////////////////////////width/////////////////////////////////////
                            Rectangle{
                                id: widthContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35
                                z:5
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                QSpinBox {
                                    id: widthValue
                                    value: 40000.00
                                    step: 1000
                                    z:5
                                    anchors.fill:parent
                                    onValueChanged: {
                                        if(boxProperties && widthValue && (widthValue.value === 0 || widthValue.value)){
                                            boxProperties.width = widthValue.value
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    text: qsTr("Width:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }


                            ///////////////////////////////////height/////////////////////////////////////
                            Rectangle{
                                id: heightContainer
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 35
                                z:4
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                QSpinBox {
                                    id: heightValue
                                    value: 40000.00
                                    step: 1000
                                    z:4
                                    anchors.fill:parent
                                    onValueChanged: {
                                        if(boxProperties && heightValue && (heightValue.value === 0 || heightValue.value)){
                                            boxProperties.height = heightValue.value
                                        }
                                    }
                                }
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                color: "#404040"
                                height: 30
                                //                                border.color: "#5f5f5f"
                                //                                border.width: 1

                                Text {
                                    text: qsTr("Height:")
                                    font.pointSize: 10
                                    color: "white"
                                    anchors.verticalCenter:  parent.verticalCenter
                                    x:7
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

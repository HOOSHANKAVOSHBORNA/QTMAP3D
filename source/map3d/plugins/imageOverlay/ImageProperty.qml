import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent ? parent.height : 0



    property ImageProperty ImageProperty
    property string headerTitleSTR: "Image Properties"


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



                            ///////////////////////////////////center Property///////////////////////////////
                            Rectangle{
                                id:centerContainer
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.6
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Column{
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("X:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                decimals: 5
                                                id: mcenterX
                                                anchors.fill: parent
                                                 showText: ImageProperty ? ImageProperty.location.x : 0
                                                onValueChanged: {
                                                    ImageProperty.location.x = value

                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("Y:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                decimals: 5
                                                id: mcenterY
                                                anchors.fill: parent
                                                 showText: ImageProperty ? ImageProperty.location.y : 0
                                                onValueChanged: {
                                                    ImageProperty.location.y = value

                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id: centerTitle
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.4
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: centerImage
                                    text: qsTr("Center :")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }
                            }

                            ///////////////////////////////////tl Property///////////////////////////////
                            Rectangle{
                                id:tlContainer
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.6
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Column{
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("X:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                decimals: 5
                                                anchors.fill: parent
                                                 showText: ImageProperty ? ImageProperty.tl.x : 0
                                                onValueChanged: {
                                                    ImageProperty.tl.x = value

                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("Y:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                decimals: 5
                                                anchors.fill: parent
                                                 showText: ImageProperty ? ImageProperty.tl.y : 0
                                                onValueChanged: {
                                                    ImageProperty.tl.y = value

                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id: tlTitle
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.4
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: tlImage
                                    text: qsTr("TopLeft :")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }
                            }

                            ///////////////////////////////////tr Property///////////////////////////////
                            Rectangle{
                                id:trContainer
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.6
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Column{
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("X:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                decimals: 5
                                                anchors.fill: parent
                                                 showText: ImageProperty ? ImageProperty.tr.x : 0
                                                onValueChanged: {
                                                    ImageProperty.tr.x = value

                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("Y:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                decimals: 5
                                                anchors.fill: parent
                                                 showText: ImageProperty ? ImageProperty.tr.y : 0
                                                onValueChanged: {
                                                    ImageProperty.tr.y = value

                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id: trTitle
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.4
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: trImage
                                    text: qsTr("TopRight :")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }
                            }

                            ///////////////////////////////////br Property///////////////////////////////
                            Rectangle{
                                id:brContainer
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.6
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Column{
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("X:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                decimals: 5
                                                anchors.fill: parent
                                                 showText: ImageProperty ? ImageProperty.br.x : 0
                                                onValueChanged: {
                                                    ImageProperty.br.x = value

                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("Y:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                decimals: 5
                                                anchors.fill: parent
                                                 showText: ImageProperty ? ImageProperty.br.y : 0
                                                onValueChanged: {
                                                    ImageProperty.br.y = value

                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id: brTitle
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.4
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: brImage
                                    text: qsTr("BottomRight:")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
                                    x:7
                                }
                            }


                            ///////////////////////////////////bl Property///////////////////////////////
                            Rectangle{
                                id:blContainer
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.6
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Column{
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("X:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                anchors.fill: parent
                                                decimals: 5
                                                 showText: ImageProperty ? ImageProperty.bl.x : 0
                                                onValueChanged: {
                                                    ImageProperty.bl.x = value

                                                }
                                            }
                                        }
                                    }
                                    Row{
                                        spacing: 8
                                        leftPadding: 5
                                        topPadding: 5
                                        anchors.horizontalCenter: parent.horizontalCenter

                                        Text {
                                            color: "#e5e5e5"
                                            text: qsTr("Y:")
                                            font.pointSize: 10
                                        }
                                        Rectangle{
                                            height: 20
                                            width: 100
                                            color: "transparent"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            FloatSpinBox {
                                                decimals: 5
                                                anchors.fill: parent
                                                 showText: ImageProperty ? ImageProperty.bl.y : 0
                                                onValueChanged: {
                                                    ImageProperty.bl.y = value

                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle{
                                id: blTitle
                                //                                Layout.fillWidth: true
                                width: parent.width * 0.4
                                color: "#404040"
                                height: 60
                                border.color: "#5f5f5f"
                                border.width: 1

                                Text {
                                    id: blImage
                                    text: qsTr("BottomLeft:")
                                    font.pointSize: 10
                                    color: "white"
                                    y: 10
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

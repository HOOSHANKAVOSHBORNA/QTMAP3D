import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import Crystal 1.0

Item {
    id: rootItem
    implicitHeight: parent.height



    property ImageProperties imageProperties
    property string headerTitleSTR: "Image Properties"



    onVisibleChanged: {
        imageProperties.height = heightValue.value
        imageProperties.width = widthValue.value
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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            TextInput {
                                                id: mcenterX
                                                padding: 2
                                                anchors.fill: parent
                                                text: imageProperties.location.x.toFixed(9)
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    imageProperties.location.x = text

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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: mcenterY
                                                padding: 2
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                text: imageProperties.location.y.toFixed(9)
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {

                                                    imageProperties.location.y = text

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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            TextInput {
                                                id: mtlX
                                                padding: 2
                                                anchors.fill: parent
                                                text: imageProperties.tl.x.toFixed(9)
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    imageProperties.tl.x = text

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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: mtlY
                                                padding: 2
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                text: imageProperties.tl.y.toFixed(9)
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {

                                                    imageProperties.tl.y = text

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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            TextInput {
                                                id: mtrX
                                                padding: 2
                                                anchors.fill: parent
                                                text: imageProperties.tr.x.toFixed(9)
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    imageProperties.tr.x = text

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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: mtrY
                                                padding: 2
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                text: imageProperties.tr.y.toFixed(9)
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {

                                                    imageProperties.tr.y = text

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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            TextInput {
                                                id: mbrX
                                                padding: 2
                                                anchors.fill: parent
                                                text: imageProperties.br.x.toFixed(9)
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    imageProperties.br.x = text

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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: mbrY
                                                padding: 2
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                text: imageProperties.br.y.toFixed(9)
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {

                                                    imageProperties.br.y = text

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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5
                                            Layout.fillWidth: true
                                            TextInput {
                                                id: mblX
                                                padding: 2
                                                anchors.fill: parent
                                                text: imageProperties.bl.x.toFixed(9)
                                                font.pointSize: 10
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert x"
                                                }
                                                onTextChanged: {
                                                    imageProperties.bl.x = text

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
                                            color: "#c9c9c9"
                                            clip:  true
                                            radius: 5

                                            TextInput {
                                                id: mblY
                                                padding: 2
                                                anchors.fill: parent
                                                font.pointSize: 10
                                                text: imageProperties.bl.y.toFixed(9)
                                                selectByMouse: true
                                                selectionColor: "dark green"
                                                validator: DoubleValidator {
                                                    decimals: 13;
                                                    notation: DoubleValidator.StandardNotation
                                                    locale: "insert y"
                                                }
                                                onTextChanged: {

                                                    imageProperties.bl.y = text

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

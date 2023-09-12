import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "style"

Rectangle {
    id: rLocationManager

    property alias listModel: lvLocationManger.model

    property int locationColorIconWidth: 10
    property int locationColorIconMargin: 3

    readonly property color backgroundColor: "#DEE3E6"
    readonly property color backgroundColorop25: Qt.rgba(backgroundColor.r, backgroundColor.g, backgroundColor.b, 0.25)
    readonly property color backgroundColorop50: Qt.rgba(backgroundColor.r, backgroundColor.g, backgroundColor.b, 0.5)

    readonly property color foregroundColor: "#003569"
    readonly property color foregroundColorop75: Qt.rgba(foregroundColor.r, foregroundColor.g, foregroundColor.b, 0.75)

    readonly property color endGradiant: "#EDF3F9"
    readonly property color endGradiantop0: Qt.rgba(endGradiant.r, endGradiant.g, endGradiant.b, 0)

    color: "grey"
//    gradient: Gradient {
//        GradientStop { position: 0.0; color: backgroundColor; }
//        GradientStop { position: height; color: endGradiantop0; }
//    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // ----------------------------------------------- search bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            radius: height / 2
            color: backgroundColorop25
            clip: true

            TextInput {
                id: tiSearchedText
                anchors.fill: parent
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                verticalAlignment: Text.AlignVCenter
                font.family: "Roboto"
                font.pixelSize: 17 / Style.monitorRatio
                color: foregroundColorop75

                onTextChanged: {
                    lvLocationManger.model.searchedName = text
                }
            }

            Text {
                anchors.fill: parent
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                verticalAlignment: tiSearchedText.verticalAlignment
                text: "Go to location..."
                font: tiSearchedText.font
                visible: tiSearchedText.text === ""
                color: foregroundColorop75
            }
        }

        // ----------------------------------------------- add place button
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            radius: height / 2
            color: backgroundColorop25

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    console.log(lvLocationManger.model.searchedName)
                }
            }

            RowLayout {
                anchors.fill: parent

                Rectangle {
                    Layout.fillWidth: true
                }

                Image {
                    Layout.preferredWidth: 22 / Style.monitorRatio
                    Layout.preferredHeight: 22 / Style.monitorRatio
                    source: "qrc:/Resources/location-add.png"
                }

                Rectangle {
                    Layout.preferredWidth: 12
                }

                Text {
                    text: "Add Place"
                    font.family: "Roboto"
                    font.pixelSize: 17 / Style.monitorRatio
                    color: foregroundColor
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }
        }

        // ----------------------------------------------- seperate line
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: backgroundColorop50
        }

        // ----------------------------------------------- locatoins list
        ListView {
            id: lvLocationManger

            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 10
            clip: true

            // model set from MainWindow.qml by listModel property in this file

            delegate: Rectangle {
                width: lvLocationManger.width
                height: 232 / Style.monitorRatio
                color: backgroundColorop25
                radius: 15 / Style.monitorRatio

                MouseArea {
                    anchors.fill: parent

                    onDoubleClicked: {
                        // --- for debug
                        console.log("model.name: ", model.name)
                        console.log("model.lon: ", model.lon)
                        console.log("model.lat: ", model.lat)
                        console.log("model.z: ", model.z)
                        console.log("model.heading: ", model.heading)
                        console.log("model.pitch: ", model.pitch)
                        console.log("model.range: ", model.range)
                        // ---

                        lvLocationManger.model.goToLocation(lvLocationManger.model.index(index, 0))
                    }
                }

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 5

                    Image {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        source: model.imageSource
                    }

                    // ----------------------------------------------- Location Name Row
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: locationColorIconMargin
                        Layout.alignment: Qt.AlignCenter

                        Rectangle {
                            width: locationColorIconWidth
                            height: locationColorIconWidth
                            radius: width / 2
                            color: model.color
                        }

                        Text {
                            Layout.fillWidth: true
                            text: model.name
                            font.pixelSize: 20 / Style.monitorRatio
                            font.family: "Roboto"
                            color: foregroundColor

                            // for debug
                            MouseArea {
                                anchors.fill: parent

                                onDoubleClicked: {
                                    lvLocationManger.model.printCurrentLocation();
                                }
                            }
                        }

                        Button {
                            background: Rectangle {
                                color: "transparent"
                            }

                            opacity: 0.75
                            icon.source: "qrc:/Resources/location-edit.png"
                            icon.width: 20
                            icon.height: 20
                        }

                        Button {
                            background: Rectangle {
                                color: "transparent"
                            }

                            opacity: 0.75
                            icon.source: "qrc:/Resources/location-delete.png"
                            icon.width: 20
                            icon.height: 20

                            onClicked: {
                                lvLocationManger.model.myRemoveRow(lvLocationManger.model.index(index, 0))
                            }
                        }
                    }

                    // ----------------------------------------------- Location details
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.leftMargin: locationColorIconWidth + locationColorIconMargin
                        spacing: 0

                        Text {
                            text: model.description
                            font.pixelSize: 17 / Style.monitorRatio
                            font.family: "Roboto"
                            color: foregroundColorop75
                        }

                        Text {
                            text: model.lon + ", " + model.lat
                            font.pixelSize: 17 / Style.monitorRatio
                            font.family: "Roboto"
                            color: foregroundColorop75
                        }
                    }
                }
            }
        }
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "style"

Rectangle {
    id: rLocationManager

    // aliases
    property alias listModel: lvLocationManger.model

    // sizes
    property int locationColorIconWidth: 10
    property int locationColorIconMargin: 3

    // colors
    readonly property color bg20: Qt.rgba(Style.backgroundColor.r, Style.backgroundColor.g, Style.backgroundColor.b, 0.20)
    readonly property color bg25: Qt.rgba(Style.backgroundColor.r, Style.backgroundColor.g, Style.backgroundColor.b, 0.25)
    readonly property color bg50: Qt.rgba(Style.backgroundColor.r, Style.backgroundColor.g, Style.backgroundColor.b, 0.50)

    readonly property color fg80: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.80)
    readonly property color fg75: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.75)
    readonly property color fg50: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.50)

    readonly property color beautifulWhite: "#EDF3F9"

    // components


    color: "silver"

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 40 / 1
        anchors.leftMargin: 20 / Style.monitorRatio
        anchors.rightMargin: 18 / Style.monitorRatio
        spacing: 12 / Style.monitorRatio

        // ----------------------------------------------- search bar & add place button
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 5 / Style.monitorRatio

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 30 / Style.monitorRatio
                radius: height / 2
                color: bg20
                clip: true

                TextInput {
                    id: tiSearchedText

                    anchors.fill: parent
                    anchors.leftMargin: 15 / Style.monitorRatio
                    anchors.rightMargin: 15 / Style.monitorRatio
                    verticalAlignment: Text.AlignVCenter
                    font.family: Style.fontFamily
                    font.pixelSize: 17 / Style.monitorRatio
                    color: fg75

                    onTextChanged: lvLocationManger.model.searchedName = text
                }

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 15 / Style.monitorRatio
                    anchors.rightMargin: 15 / Style.monitorRatio
                    verticalAlignment: tiSearchedText.verticalAlignment
                    text: "Go to location..."
                    font: tiSearchedText.font
                    visible: tiSearchedText.text === ""
                    color: fg75
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 30 / Style.monitorRatio
                radius: height / 2
                color: bg25

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 4 / Style.monitorRatio

                    Image {
                        Layout.preferredWidth: 22 / Style.monitorRatio
                        Layout.preferredHeight: 22 / Style.monitorRatio
                        source: "qrc:/Resources/location-add.png"
                    }

                    Text {
                        text: "Add place"
                        font.family: Style.fontFamily
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: popup.visible = true
                }
            }
        }

        // -------------------------------------- Popup
        Window {
            id: popup

            x: 400
            y: 400
            width: 800 / Style.monitorRatio
            height: 800 / Style.monitorRatio
            color: "transparent"
            visible: false
            flags: Qt.FramelessWindowHint

            // -------------------------------------- rPopup Draggablity
            MouseArea {
                anchors.fill: parent

                smooth: true
                drag.target: rPopup
                drag.axis: Drag.XAndYAxis
                drag.minimumX: 0
                drag.minimumY: 0
                drag.maximumX: 800
                drag.maximumY: 800
            }

            // -------------------------------------- rPopup
            Rectangle {
                id: rPopup

                width: 500 / Style.monitorRatio
                height: 678 / Style.monitorRatio
                color: "silver"
                radius: 20 / Style.monitorRatio

                Text {
                    id: txtAddPlace

                    text: "Add place"
                    color: Style.foregroundColor
                    anchors.horizontalCenter: rPopup.horizontalCenter
                    anchors.top: rPopup.top
                    anchors.topMargin: 25 / Style.monitorRatio
                    font.family: Style.fontFamily
                    font.pixelSize: 30 / Style.monitorRatio
                    font.styleName: Font.Medium
                }

                Button {
                    background: Rectangle {
                        color: "transparent"
                    }

                    anchors.verticalCenter: txtAddPlace.verticalCenter
                    anchors.right: rPopup.right
                    anchors.rightMargin: 24 / Style.monitorRatio
                    icon.source: "qrc:/Resources/add-place-close.png"
                    icon.width: 35 / Style.monitorRatio
                    icon.height: 35 / Style.monitorRatio

                    onClicked: popup.close()
                }

                ColumnLayout {
                    anchors.top: txtAddPlace.bottom
                    anchors.topMargin: 50 / Style.monitorRatio
                    anchors.left: rPopup.left
                    anchors.leftMargin: 24 / Style.monitorRatio
                    width: 452 / Style.monitorRatio
                    spacing: 25 / Style.monitorRatio

                    // ----------------------------------------------- add place location
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 10 / Style.monitorRatio
                        RowLayout {
                            spacing: 2 / Style.monitorRatio

                            Button {
                                id: iconLocation

                                background: Rectangle {
                                    color: "transparent"
                                }

                                icon.source: "qrc:/Resources/add-place-location.png"
                                icon.width: 30 / Style.monitorRatio
                                icon.height: 30 / Style.monitorRatio
                                icon.color: Style.foregroundColor
                            }

                            Text {
                                id: txtPlaceName

                                text: "Iran . Tehran Province . Tehran"
                                font.family: Style.fontFamily
                                font.pixelSize: 20 / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                        }

                        Text {
                            Layout.leftMargin: (30 + 2) / 1 // (wtf?)
                            text: "45.568075, 74.136643"
                            font.family: Style.fontFamily
                            font.pixelSize: 20 / Style.monitorRatio
                            color: Style.foregroundColor
                        }
                    }

                    // ----------------------------------------------- add place input name
                    Rectangle {
                        color: bg50
                        radius: 10 / Style.monitorRatio
                        Layout.fillWidth: true
                        height: 55 / Style.monitorRatio
                        clip: true

                        TextInput {
                            id: tiLocationName

                            anchors.fill: parent
                            anchors.leftMargin: 32 / Style.monitorRatio
                            anchors.rightMargin: 32 / Style.monitorRatio
                            verticalAlignment: Text.AlignVCenter
                            font.family: Style.fontFamily
                            font.pixelSize: 20 / Style.monitorRatio
                            color: fg80
                        }

                        Text {
                            anchors.fill: parent
                            anchors.leftMargin: 32 / Style.monitorRatio
                            anchors.rightMargin: 32 / Style.monitorRatio
                            verticalAlignment: tiLocationName.verticalAlignment
                            text: "Place name"
                            font: tiLocationName.font
                            visible: tiLocationName.text === ""
                            color: fg80
                        }
                    }

                    // ----------------------------------------------- add place take photo
                    Rectangle {
                        Layout.fillWidth: true
                        color: bg50
                        radius: 10 / Style.monitorRatio
                        height: 200 / Style.monitorRatio

                        Column {
                            anchors.centerIn: parent

                            Button {
                                background: Rectangle {
                                    color: "transparent"
                                }

                                anchors.horizontalCenter: txtTakePhoto.horizontalCenter
                                icon.source: "qrc:/Resources/add-place-add-camera.png"
                                icon.width: 40 / Style.monitorRatio
                                icon.height: 40 / Style.monitorRatio
                                icon.color: Style.foregroundColor
                            }

                            Text {
                                id: txtTakePhoto

                                text: "Take photo for place"
                                font.underline: true
                                font.family: Style.fontFamily
                                font.pixelSize: 20 / Style.monitorRatio
                                color: fg80
                            }
                        }
                    }

                    // ----------------------------------------------- place color selection
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 10 / Style.monitorRatio
                        Text {
                            text: "Place Color"
                            font.family: Style.fontFamily
                            font.pixelSize: 20 / Style.monitorRatio
                            color: fg80
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Rectangle {
                                Layout.rightMargin: 5 / Style.monitorRatio
                                color: "red"
                                width: 30 / Style.monitorRatio
                                height: 30 / Style.monitorRatio
                                radius: width / 2
                            }

                            Rectangle {
                                Layout.rightMargin: 5 / Style.monitorRatio
                                color: "blue"
                                width: 30 / Style.monitorRatio
                                height: 30 / Style.monitorRatio
                                radius: width / 2
                            }

                            Rectangle {
                                Layout.rightMargin: 5 / Style.monitorRatio
                                color: "yellow"
                                width: 30 / Style.monitorRatio
                                height: 30 / Style.monitorRatio
                                radius: width / 2
                            }

                            Rectangle {
                                Layout.rightMargin: 5 / Style.monitorRatio
                                color: "green"
                                width: 30 / Style.monitorRatio
                                height: 30 / Style.monitorRatio
                                radius: width / 2
                            }

                            Rectangle {
                                Layout.fillWidth: true
                            }

                            Text {
                                text: "More colors"
                                font.pixelSize: 15 / Style.monitorRatio
                                font.family: Style.fontFamily
                                color: fg50
                            }

                            Button {
                                background: Rectangle {
                                    color: "transparent"
                                }

                                icon.source: "qrc:/Resources/add-place-more-color.png"
                                icon.width: 30 / Style.monitorRatio
                                icon.height: 30 / Style.monitorRatio
                            }
                        }
                    }
                }

                // ---------------------------------------------- cancel & save buttons
                Row {
                    anchors.bottom: rPopup.bottom
                    anchors.bottomMargin: 25 / Style.monitorRatio
                    anchors.right: rPopup.right
                    anchors.rightMargin: 24 / Style.monitorRatio
                    spacing: 5

                    Rectangle {
                        width: 90 / Style.monitorRatio
                        height: 40 / Style.monitorRatio
                        radius: 20 / Style.monitorRatio
                        color: "transparent"
                        border.width: 1
                        border.color: Style.foregroundColor

                        Text {
                            anchors.centerIn: parent
                            text: "Cancel"
                            font.pixelSize: 22 / Style.monitorRatio
                            font.family: Style.fontFamily
                            color: Style.foregroundColor
                        }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: popup.close()
                        }
                    }

                    Rectangle {
                        width: 90 / Style.monitorRatio
                        height: 40 / Style.monitorRatio
                        radius: 20 / Style.monitorRatio
                        color: Style.foregroundColor

                        Text {
                            anchors.centerIn: parent
                            text: "Save"
                            font.pixelSize: 22 / Style.monitorRatio
                            font.family: Style.fontFamily
                            color: beautifulWhite
                        }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                lvLocationManger.model.addNewLocation(tiLocationName.text, "default description", "qrc:/Resources/airplane1.jpg", "red")
                                popup.close()
                            }
                        }
                    }
                }
            }
        }

        // ----------------------------------------------- seperate line
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: bg50
        }

        // ----------------------------------------------- locatoins list
        ListView {
            id: lvLocationManger

            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10 / Style.monitorRatio
            clip: true

            // model set from MainWindow.qml by listModel property in this file

            delegate: Rectangle {
                width: lvLocationManger.width
                height: 232 / Style.monitorRatio
                color: bg25
                radius: 15 / Style.monitorRatio
                clip: true

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

                Column {
                    anchors.fill: parent
                    anchors.margins: 5 / Style.monitorRatio
                    spacing: 0

                    Image {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 138 / Style.monitorRatio
                        source: model.imageSource
                    }

                    // ----------------------------------------------- Location Name Row
                    RowLayout {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        spacing: 0

                        Rectangle {
                            width: locationColorIconWidth
                            height: locationColorIconWidth
                            radius: width / 2
                            color: model.color
                        }

                        Rectangle {
                            width: 3 / Style.monitorRatio
                        }

                        Text {
                            Layout.fillWidth: true
                            text: model.name
                            font.pixelSize: 20 / Style.monitorRatio
                            font.family: Style.fontFamily
                            color: Style.foregroundColor

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
                            font.family: Style.fontFamily
                            color: fg75
                        }

                        Text {
                            text: model.lon + ", " + model.lat
                            font.pixelSize: 17 / Style.monitorRatio
                            font.family: Style.fontFamily
                            color: fg75
                        }
                    }
                }
            }
        }
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "style"

Rectangle {
    id: rLocationManager

    // aliases
    property alias listModel: lvLocationManger.model

    // sizes
    property int locationColorIconWidth: 13
    property int locationColorIconMargin: 3

    // colors
    readonly property color bg40: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.40)
    readonly property color bg20: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.20)
    readonly property color bg25: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.25)
    readonly property color bg50: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.50)

    readonly property color fg80: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.80)
    readonly property color fg75: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.75)
    readonly property color fg50: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.50)

    readonly property color beautifulWhite: "#EDF3F9"

    color: "transparent"

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

                    onClicked: {
                        popup.editIndex = -1
                        popup.myOpen()
                    }
                }
            }
        }

        // -------------------------------------- Popup
        Window {
            id: popup

            property int editIndex: -1

            function myOpen() {
                txtPlaceName.text = listModel.getCurrentXYZ().x.toFixed(
                            6) + ", " + listModel.getCurrentXYZ().y.toFixed(6)

                popup.visible = true
            }

            function myClose() {
                tiLocationName.text = ""
                tiLocationDescription.text = ""
                lvColors.selectedColor = "black"

                popup.close()
            }

            x: 400
            y: 400
            width: 1500 / Style.monitorRatio
            height: 1000 / Style.monitorRatio
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

                width: 400 / Style.monitorRatio
                height: 523 / Style.monitorRatio
                color: "silver"
                radius: 20 / Style.monitorRatio

                Text {
                    id: txtAddPlace

                    text: "Add place"
                    color: Style.foregroundColor
                    anchors.horizontalCenter: rPopup.horizontalCenter
                    anchors.top: rPopup.top
                    anchors.topMargin: 30 / Style.monitorRatio
                    font.family: Style.fontFamily
                    font.pixelSize: 22 / Style.monitorRatio
                }

                Button {
                    topPadding: 0
                    rightPadding: 0
                    bottomPadding: 0
                    leftPadding: 0

                    background: Rectangle {
                        color: "transparent"
                    }

                    anchors.verticalCenter: txtAddPlace.verticalCenter
                    anchors.right: rPopup.right
                    anchors.rightMargin: 20 / Style.monitorRatio
                    icon.source: "qrc:/Resources/add-place-close.png"
                    icon.width: 24 / Style.monitorRatio
                    icon.height: 24 / Style.monitorRatio

                    onClicked: popup.myClose()
                }

                ColumnLayout {
                    anchors.top: txtAddPlace.bottom
                    anchors.topMargin: 25 / Style.monitorRatio
                    anchors.left: rPopup.left
                    anchors.leftMargin: 20 / Style.monitorRatio
                    anchors.right: rPopup.right
                    anchors.rightMargin: 20 / Style.monitorRatio

                    spacing: 8 / Style.monitorRatio

                    // ----------------------------------------------- add place location
                    RowLayout {
                        spacing: 1 / Style.monitorRatio
                        Layout.bottomMargin: 2 / Style.monitorRatio

                        Button {
                            id: iconLocation

                            topPadding: 0
                            rightPadding: 0
                            bottomPadding: 0
                            leftPadding: 0

                            background: Rectangle {
                                color: "transparent"
                            }

                            icon.source: "qrc:/Resources/add-place-location.png"
                            icon.width: 26 / Style.monitorRatio
                            icon.height: 26 / Style.monitorRatio
                            icon.color: Style.foregroundColor
                        }

                        Text {
                            id: txtPlaceName

                            text: "set before opening :)"
                            font.family: Style.fontFamily
                            font.pixelSize: 17 / Style.monitorRatio
                            color: Style.foregroundColor
                        }
                    }

                    // ----------------------------------------------- add place input name
                    Rectangle {
                        color: bg50
                        radius: 10 / Style.monitorRatio
                        Layout.fillWidth: true
                        height: 36 / Style.monitorRatio
                        clip: true

                        TextInput {
                            id: tiLocationName

                            anchors.fill: parent
                            anchors.leftMargin: 30 / Style.monitorRatio
                            anchors.rightMargin: 30 / Style.monitorRatio
                            verticalAlignment: Text.AlignVCenter
                            font.family: Style.fontFamily
                            font.pixelSize: 17 / Style.monitorRatio
                            color: fg80
                        }

                        Text {
                            anchors.fill: parent
                            anchors.leftMargin: 30 / Style.monitorRatio
                            anchors.rightMargin: 30 / Style.monitorRatio
                            verticalAlignment: tiLocationName.verticalAlignment
                            text: "Place name"
                            font: tiLocationName.font
                            visible: tiLocationName.text === ""
                            color: fg50
                        }
                    }

                    // ----------------------------------------------- add place input description
                    Rectangle {
                        color: bg50
                        radius: 10 / Style.monitorRatio
                        Layout.fillWidth: true
                        height: 36 / Style.monitorRatio
                        clip: true

                        TextInput {
                            id: tiLocationDescription

                            anchors.fill: parent
                            anchors.leftMargin: 30 / Style.monitorRatio
                            anchors.rightMargin: 30 / Style.monitorRatio
                            verticalAlignment: Text.AlignVCenter
                            font.family: Style.fontFamily
                            font.pixelSize: 17 / Style.monitorRatio
                            color: fg80
                        }

                        Text {
                            anchors.fill: parent
                            anchors.leftMargin: 30 / Style.monitorRatio
                            anchors.rightMargin: 30 / Style.monitorRatio
                            verticalAlignment: tiLocationDescription.verticalAlignment
                            text: "Description"
                            font: tiLocationDescription.font
                            visible: tiLocationDescription.text === ""
                            color: fg50
                        }
                    }

                    // ----------------------------------------------- add place take photo
                    Rectangle {
                        Layout.fillWidth: true
                        color: bg40
                        radius: 10 / Style.monitorRatio
                        height: 180 / Style.monitorRatio

                        Column {
                            anchors.centerIn: parent
                            spacing: 3 / Style.monitorRatio

                            Button {
                                topPadding: 0
                                rightPadding: 0
                                bottomPadding: 0
                                leftPadding: 0

                                background: Rectangle {
                                    color: "transparent"
                                }

                                opacity: 0.75
                                anchors.horizontalCenter: txtTakePhoto.horizontalCenter
                                icon.source: "qrc:/Resources/add-place-add-camera.png"
                                icon.width: 30 / Style.monitorRatio
                                icon.height: 30 / Style.monitorRatio
                                icon.color: Style.foregroundColor
                            }

                            Text {
                                id: txtTakePhoto

                                text: "Take photo for place"
                                font.underline: true
                                font.family: Style.fontFamily
                                font.pixelSize: 15 / Style.monitorRatio
                                color: fg50
                            }
                        }
                    }

                    // ----------------------------------------------- place color selection
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2 / Style.monitorRatio

                        Text {
                            text: "Place color"
                            font.family: Style.fontFamily
                            font.pixelSize: 17 / Style.monitorRatio
                            color: fg80
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 5 / Style.monitorRatio

                            Row {
                                spacing: 0 / Style.monitorRatio

                                Repeater {
                                    id: lvColors
                                    model: ["red", "blue", "yellow", "green", "white", "black", "orange", "pink", "purple"]

                                    property string selectedColor: "black"

                                    Rectangle {
                                        required property string modelData

                                        width: 26 / Style.monitorRatio
                                        height: 26 / Style.monitorRatio
                                        radius: width / 2
                                        color: "transparent"
                                        border.color: modelData
                                        border.width: lvColors.selectedColor === modelData ? 1 : 0

                                        Rectangle {
                                            anchors.centerIn: parent
                                            color: parent.modelData
                                            width: 21 / Style.monitorRatio
                                            height: 21 / Style.monitorRatio
                                            radius: width / 2
                                        }

                                        Image {
                                            visible: lvColors.selectedColor === modelData
                                            anchors.centerIn: parent
                                            source: "qrc:/Resources/add-place-color-select.png"
                                            width: 22 / Style.monitorRatio
                                            height: 22 / Style.monitorRatio
                                        }

                                        MouseArea {
                                            anchors.fill: parent

                                            onClicked: {
                                                console.log("color: " + parent.modelData)
                                                //                                                console.log(index)
                                                lvColors.selectedColor = parent.modelData
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                            }

                            Text {
                                text: "More colors"
                                font.pixelSize: 14 / Style.monitorRatio
                                font.family: Style.fontFamily
                                color: fg50
                            }

                            Button {
                                topPadding: 0
                                rightPadding: 0
                                bottomPadding: 0
                                leftPadding: 0

                                background: Rectangle {
                                    color: "transparent"
                                }

                                icon.source: "qrc:/Resources/add-place-more-color.png"
                                icon.width: 22 / Style.monitorRatio
                                icon.height: 22 / Style.monitorRatio
                            }
                        }
                    }
                }

                // ---------------------------------------------- cancel & save buttons
                Row {
                    anchors.bottom: rPopup.bottom
                    anchors.bottomMargin: 30 / Style.monitorRatio
                    anchors.right: rPopup.right
                    anchors.rightMargin: 20 / Style.monitorRatio
                    spacing: 4 / Style.monitorRatio

                    Rectangle {
                        width: 77 / Style.monitorRatio
                        height: 28 / Style.monitorRatio
                        radius: 21 / Style.monitorRatio
                        color: "transparent"
                        border.width: 1
                        border.color: Style.foregroundColor

                        Text {
                            anchors.centerIn: parent
                            text: "Cancel"
                            font.pixelSize: 15 / Style.monitorRatio
                            font.family: Style.fontFamily
                            color: Style.foregroundColor
                        }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: popup.myClose()
                        }
                    }

                    Rectangle {
                        width: 77 / Style.monitorRatio
                        height: 28 / Style.monitorRatio
                        radius: 21 / Style.monitorRatio
                        color: Style.foregroundColor

                        Text {
                            anchors.centerIn: parent
                            text: "Save"
                            font.pixelSize: 15 / Style.monitorRatio
                            font.family: Style.fontFamily
                            color: beautifulWhite
                        }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                if (popup.editIndex === -1) {
                                    lvLocationManger.model.addNewLocation(
                                                tiLocationName.text,
                                                tiLocationDescription.text,
                                                "qrc:/Resources/airplane1.jpg",
                                                lvColors.selectedColor)
                                } else {
                                    lvLocationManger.model.editLocation(
                                                lvLocationManger.model.index(
                                                    popup.editIndex, 0),
                                                tiLocationName.text,
                                                tiLocationDescription.text,
                                                "qrc:/Resources/airplane1.jpg",
                                                lvColors.selectedColor)
                                }

                                lvLocationManger.model.sourceModel.writeToFile()
                                popup.myClose()
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
                id: rDelegate

                width: lvLocationManger.width
                height: 232 / Style.monitorRatio
                color: bg25
                radius: 15 / Style.monitorRatio
                clip: true

                MouseArea {
                    id: goToLocationBtn
                    anchors.fill: parent

                    onDoubleClicked: {

                        // DEBUG
                        //                        console.log("model.name: ", model.name)
                        //                        console.log("model.lon: ", model.lon)
                        //                        console.log("model.lat: ", model.lat)
                        //                        console.log("model.z: ", model.z)
                        //                        console.log("model.heading: ", model.heading)
                        //                        console.log("model.pitch: ", model.pitch)
                        //                        console.log("model.range: ", model.range)
                        // ENDDEBUG
                        lvLocationManger.model.goToLocation(
                                    lvLocationManger.model.index(index, 0))
                    }
                }

                Rectangle {
                    id: rDelegateContent
                    anchors.fill: parent
                    anchors.margins: 5 / Style.monitorRatio
                    color: "transparent"

                    Image {
                        id: imgLocation

                        anchors.left: rDelegateContent.left
                        anchors.right: rDelegateContent.right
                        height: 138 / Style.monitorRatio
                        source: model.imageSource
                    }

                    // ----------------------------------------------- Location Name Row
                    RowLayout {
                        anchors.top: imgLocation.bottom
                        anchors.topMargin: 7 / Style.monitorRatio
                        anchors.left: rDelegateContent.left
                        anchors.right: rDelegateContent.right
                        spacing: 0

                        Rectangle {
                            width: locationColorIconWidth / Style.monitorRatio
                            height: locationColorIconWidth / Style.monitorRatio
                            radius: width / 2
                            color: model.color
                        }

                        Rectangle {
                            width: locationColorIconMargin / Style.monitorRatio
                        }

                        Text {
                            id: txtLocationName

                            Layout.fillWidth: true
                            text: model.name
                            font.pixelSize: 20 / Style.monitorRatio
                            font.family: Style.fontFamily
                            color: Style.foregroundColor

                            // for debug
                            MouseArea {
                                anchors.fill: parent

                                onDoubleClicked: {
                                    lvLocationManger.model.printCurrentLocation(
                                                )
                                }
                            }
                        }

                        Button {
                            topPadding: 0
                            rightPadding: 0
                            bottomPadding: 0
                            leftPadding: 0
                            background: Rectangle {
                                color: "transparent"
                            }

                            opacity: 0.75
                            icon.source: "qrc:/Resources/location-edit.png"
                            icon.width: 25 / Style.monitorRatio
                            icon.height: 25 / Style.monitorRatio

                            onClicked: {
                                lvLocationManger.model.goToLocation(
                                            lvLocationManger.model.index(index,
                                                                         0))

                                tiLocationName.text = model.name
                                tiLocationDescription.text = model.description
                                lvColors.selectedColor = model.color
                                popup.editIndex = model.index

                                popup.myOpen()
                            }
                        }

                        Button {
                            topPadding: 0
                            rightPadding: 0
                            bottomPadding: 0
                            leftPadding: 0
                            background: Rectangle {
                                color: "transparent"
                            }

                            opacity: 0.75
                            icon.source: "qrc:/Resources/location-delete.png"
                            icon.width: 25 / Style.monitorRatio
                            icon.height: 25 / Style.monitorRatio

                            onClicked: lvLocationManger.model.myRemoveRow(
                                           lvLocationManger.model.index(index,
                                                                        0))
                        }
                    }

                    // ----------------------------------------------- Location details
                    ColumnLayout {
                        anchors.left: rDelegateContent.left
                        anchors.leftMargin: (locationColorIconWidth
                                             + locationColorIconMargin) / Style.monitorRatio
                        anchors.bottom: rDelegateContent.bottom
                        anchors.bottomMargin: 5 / Style.monitorRatio
                        spacing: 0

                        Text {
                            text: model.description
                            font.pixelSize: 17 / Style.monitorRatio
                            font.family: Style.fontFamily
                            color: fg75
                        }

                        Text {
                            text: model.lon.toFixed(
                                      6) + ", " + model.lat.toFixed(6)
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

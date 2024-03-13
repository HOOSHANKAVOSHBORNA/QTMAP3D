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
    readonly property color bg75: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.75)
    readonly property color bg95: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.75)

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

        // TEST: viewpoint changes connection
        //        Label {
        //            Layout.fillWidth: true
        //            Layout.preferredHeight: 20 / Style.monitorRatio
        //            background: Rectangle {
        //                color: "transparent"
        //            }

        //            Text {
        //                Layout.alignment: Qt.AlignVCenter
        //                color: Style.foregroundColor
        //                text: locationCpp.viewPoint.x.toFixed(
        //                          3) + ", " + locationCpp.viewPoint.y.toFixed(
        //                          3) + ", " + locationCpp.viewPoint.z.toFixed(3)
        //                font.family: Style.fontFamily
        //                font.pixelSize: Style.regularFontSize
        //            }
        //        }

        // ----------------------------------------------- search bar & add place button
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 10 / Style.monitorRatio

            ColumnLayout {
                id: golocationform
                Layout.fillWidth: true
                spacing: 5 / Style.monitorRatio
                property int textWidth: 40
                //                Layout.preferredHeight: 30 / Style.monitorRatio
                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: 'lat:   '
                        color: Style.foregroundColor
                        Layout.preferredWidth: golocationform.textWidth
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30 / Style.monitorRatio
                        radius: height / 2
                        color: bg20
                        clip: true

                        TextInput {
                            id: tiLat

                            anchors.fill: parent
                            anchors.leftMargin: 15 / Style.monitorRatio
                            anchors.rightMargin: 15 / Style.monitorRatio
                            verticalAlignment: Text.AlignVCenter
                            font.family: Style.fontFamily
                            font.pixelSize: Style.regularFontSize
                            color: fg75
                            text: "50"

                            validator: RegularExpressionValidator {
                                regularExpression: /[+-]?([0-9]{1,6}[.])?[0-9]{0,6}/
                            }

                            onAccepted: {
                                listModel.goToLocation(parseFloat(tiLat.text),
                                                       parseFloat(tiLang.text),
                                                       parseFloat(tiAlt.text))
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: 'lon:   '
                        color: Style.foregroundColor
                        Layout.preferredWidth: golocationform.textWidth
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30 / Style.monitorRatio
                        radius: height / 2
                        color: bg20
                        clip: true

                        TextInput {
                            id: tiLang

                            anchors.fill: parent
                            anchors.leftMargin: 15 / Style.monitorRatio
                            anchors.rightMargin: 15 / Style.monitorRatio
                            verticalAlignment: Text.AlignVCenter
                            font.family: Style.fontFamily
                            font.pixelSize: Style.regularFontSize
                            color: fg75
                            text: "50"

                            validator: RegularExpressionValidator {
                                regularExpression: /[+-]?([0-9]{1,6}[.])?[0-9]{0,6}/
                            }

                            onAccepted: {
                                listModel.goToLocation(parseFloat(tiLat.text),
                                                       parseFloat(tiLang.text),
                                                       parseFloat(tiAlt.text))
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: 'range:   '
                        color: Style.foregroundColor
                        Layout.preferredWidth: golocationform.textWidth
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30 / Style.monitorRatio
                        radius: height / 2
                        color: bg20
                        clip: true

                        TextInput {
                            id: tiAlt

                            anchors.fill: parent
                            anchors.leftMargin: 15 / Style.monitorRatio
                            anchors.rightMargin: 15 / Style.monitorRatio
                            verticalAlignment: Text.AlignVCenter
                            font.family: Style.fontFamily
                            font.pixelSize: Style.regularFontSize
                            color: fg75
                            text: "1000"

                            validator: RegularExpressionValidator {
                                regularExpression: /[+-]?([0-9]{1,6}[.])?[0-9]{0,6}/
                            }

                            onAccepted: {
                                listModel.goToLocation(parseFloat(tiLat.text),
                                                       parseFloat(tiLang.text),
                                                       parseFloat(tiAlt.text))
                            }
                        }
                    }
                }
            }

            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 20

                padding: 0

                background: Rectangle {
                    color: 'transparent'
                }

                contentItem: Rectangle {
                    anchors.fill: parent
                    color: fg80
                    radius: 25

                    Text {
                        anchors.centerIn: parent
                        text: 'Go To Location...'
                        color: Style.backgroundColor
                    }
                }

                onClicked: {
                    listModel.goToLocation(parseFloat(tiLat.text),
                                           parseFloat(tiLang.text),
                                           parseFloat(tiAlt.text))
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 2
                color: Style.foregroundColor
                radius: 1
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 30 / Style.monitorRatio
                radius: height / 2
                color: fg80

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 4 / Style.monitorRatio

                    IconImage {
                        Layout.preferredWidth: 22 / Style.monitorRatio
                        Layout.preferredHeight: 22 / Style.monitorRatio
                        source: "qrc:/Resources/location-add.png"
                        color: Style.backgroundColor
                    }

                    Text {
                        text: "Add place"
                        font.family: Style.fontFamily
                        font.pixelSize: Style.regularFontSize
                        color: Style.backgroundColor
                    }
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        rPopup.editIndex = -1
                        rPopup.myOpen()
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 30 / Style.monitorRatio
                radius: height / 2
                color: bg20

                IconImage {
                    id: searchIcon
                    source: "qrc:/Resources/search.png"
                    width: 24 / Style.monitorRatio
                    height: 24 / Style.monitorRatio
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10 / Style.monitorRatio
                    color: Style.foregroundColor
                }

                TextField {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: searchIcon.right
                    anchors.right: parent.right
                    verticalAlignment: Text.AlignVCenter
                    font.family: Style.fontFamily
                    font.pixelSize: Style.regularFontSize

                    background: Rectangle {
                        color: 'transparent'
                    }

                    onAccepted: lvLocationManger.model.searchedName = text
                    onTextChanged: lvLocationManger.model.searchedName = text

                    placeholderText: "Name..."
                    placeholderTextColor: Style.disableColor
                }
            }
        }

        // -------------------------------------- Popup
        Window {
            id: rPopup

            property int editIndex: -1

            function myOpen() {
                rPopup.visible = true

                locationCpp.addPlaceWindowOpened()
            }

            function myClose() {
                tiLocationName.text = ""
                tiLocationDescription.text = ""
                lvColors.selectedColor = "black"

                rPopup.close()
                locationCpp.setImagePath('')
                locationCpp.addPlaceWindowClosed()
            }

            title: 'Add Place'
            flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
            width: 400 / Style.monitorRatio
            height: 523 / Style.monitorRatio
            color: "transparent"

            MouseArea {
                anchors.fill: parent
                onPositionChanged: rPopup.startSystemMove()
            }

            Rectangle {
                anchors.fill: parent
                color: 'silver'
                radius: 20 / Style.monitorRatio

                Text {
                    id: txtAddPlace

                    text: "Add place"
                    color: Style.foregroundColor
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 30 / Style.monitorRatio
                    font.family: Style.fontFamily
                    font.pixelSize: Style.titleFontSize
                }

                Button {
                    padding: 0

                    background: Rectangle {
                        color: "transparent"
                    }

                    anchors.verticalCenter: txtAddPlace.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 20 / Style.monitorRatio
                    icon.source: "qrc:/Resources/add-place-close.png"
                    icon.width: 24 / Style.monitorRatio
                    icon.height: 24 / Style.monitorRatio

                    onClicked: rPopup.myClose()
                }

                ColumnLayout {
                    anchors.top: txtAddPlace.bottom
                    anchors.topMargin: 20 / Style.monitorRatio
                    anchors.left: parent.left
                    anchors.leftMargin: 20 / Style.monitorRatio
                    anchors.rightMargin: 20 / Style.monitorRatio
                    anchors.right: parent.right

                    spacing: 8 / Style.monitorRatio

                    // ----------------------------------------------- add place location
                    RowLayout {
                        spacing: 1 / Style.monitorRatio
                        Layout.bottomMargin: 2 / Style.monitorRatio

                        Button {
                            id: iconLocation

                            padding: 0

                            background: Rectangle {
                                color: "transparent"
                            }

                            icon.source: "qrc:/Resources/add-place-location.png"
                            icon.width: 26 / Style.monitorRatio
                            icon.height: 26 / Style.monitorRatio
                            icon.color: Style.foregroundColor
                        }

                        Label {
                            Layout.preferredWidth: 55 / Style.monitorRatio
                            Layout.preferredHeight: 20 / Style.monitorRatio
                            background: Rectangle {
                                color: "transparent"
                            }

                            Text {
                                Layout.alignment: Qt.AlignVCenter
                                color: Style.foregroundColor
                                text: locationCpp ? locationCpp.viewPoint.x.toFixed(
                                                        3) + ", " + locationCpp.viewPoint.y.toFixed(
                                                        3) + ", " + locationCpp.viewPoint.z.toFixed(
                                                        3) : "locationCpp is undefined"
                                font.family: Style.fontFamily
                                font.pixelSize: Style.regularFontSize
                            }
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
                            font.pixelSize: Style.regularFontSize
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
                            font.pixelSize: Style.regularFontSize
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

                        StackLayout {
                            id: locationImage

                            anchors.fill: parent

                            currentIndex: locationCpp.imagePath === '' ? 0 : 1

                            Rectangle {
                                color: 'transparent'

                                Column {
                                    anchors.centerIn: parent
                                    spacing: 3 / Style.monitorRatio

                                    Button {
                                        padding: 0

                                        background: Rectangle {
                                            color: "transparent"
                                        }

                                        opacity: 0.75
                                        anchors.horizontalCenter: txtTakePhoto.horizontalCenter
                                        icon.source: "qrc:/Resources/add-place-add-camera.png"
                                        icon.width: 30 / Style.monitorRatio
                                        icon.height: 30 / Style.monitorRatio
                                        icon.color: Style.foregroundColor

                                        onClicked: {
                                            locationCpp.capture()
                                        }
                                    }

                                    Text {
                                        id: txtTakePhoto

                                        text: "Take photo for place"
                                        font.underline: true
                                        font.family: Style.fontFamily
                                        font.pixelSize: Style.regularFontSize
                                        color: fg50
                                    }
                                }
                            }

                            Image {
                                source: locationCpp !== undefined
                                        && locationCpp.imagePath
                                        !== '' ? 'file:///'
                                                 + locationCpp.imagePath : 'qrc:/Resources/hand.png'

                                Button {
                                    padding: 0
                                    width: 50 / Style.monitorRatio
                                    height: 50 / Style.monitorRatio
                                    anchors.right: parent.right

                                    background: IconImage {
                                        source: 'qrc:/Resources/close.png'
                                        color: Style.foregroundColor
                                    }

                                    onClicked: {
                                        locationCpp.setImagePath('')
                                    }
                                }
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
                            font.pixelSize: Style.regularFontSize
                            color: fg80
                        }

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
                    }
                }

                // ---------------------------------------------- cancel & save buttons
                Row {
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 30 / Style.monitorRatio
                    anchors.right: parent.right
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
                            font.pixelSize: Style.regularFontSize
                            font.family: Style.fontFamily
                            color: Style.foregroundColor
                        }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: rPopup.myClose()
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
                            font.pixelSize: Style.regularFontSize
                            font.family: Style.fontFamily
                            color: beautifulWhite
                        }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                if (rPopup.editIndex === -1) {
                                    lvLocationManger.model.addNewLocation(
                                                tiLocationName.text,
                                                tiLocationDescription.text,
                                                "qrc:/Resources/airplane1.jpg",
                                                lvColors.selectedColor)
                                } else {
                                    lvLocationManger.model.editLocation(
                                                lvLocationManger.model.index(
                                                    rPopup.editIndex, 0),
                                                tiLocationName.text,
                                                tiLocationDescription.text,
                                                "qrc:/Resources/airplane1.jpg",
                                                lvColors.selectedColor)
                                }

                                lvLocationManger.model.sourceModel.writeToFile()
                                rPopup.myClose()
                            }
                        }
                    }
                }
            }
        }

        // ----------------------------------------------- seperate line
        Rectangle {
            Layout.fillWidth: true
            height: 2
            color: fg80
        }

        // ----------------------------------------------- locatoins list
        ListView {
            id: lvLocationManger

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.bottomMargin: 10 / Style.monitorRatio
            spacing: 10 / Style.monitorRatio
            clip: true

            // model set from MainWindow.qml by listModel property in this file
            delegate: Rectangle {
                id: rDelegate

                width: lvLocationManger.width
                height: 232 / Style.monitorRatio
                color: fg80
                radius: 15 / Style.monitorRatio
                clip: true

                MouseArea {
                    anchors.fill: parent

                    onDoubleClicked: {
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
                            font.pixelSize: Style.titleFontSize
                            font.family: Style.fontFamily
                            color: Style.backgroundColor

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
                            icon.color: Style.backgroundColor

                            onClicked: {
                                lvLocationManger.model.goToLocation(
                                            lvLocationManger.model.index(index,
                                                                         0))

                                tiLocationName.text = model.name
                                tiLocationDescription.text = model.description
                                lvColors.selectedColor = model.color
                                rPopup.editIndex = model.index
                                locationCpp.setImagePath(model.imageSource)

                                rPopup.myOpen()
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
                            icon.color: Style.backgroundColor

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
                            font.pixelSize: Style.regularFontSize
                            font.family: Style.fontFamily
                            color: bg75
                        }

                        Text {
                            text: model.lon.toFixed(
                                      6) + ", " + model.lat.toFixed(6)
                            font.pixelSize: Style.regularFontSize
                            font.family: Style.fontFamily
                            color: bg75
                        }
                    }
                }
            }
        }
    }
}

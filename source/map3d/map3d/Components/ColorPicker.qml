import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../style"

Rectangle {
    id: rootItem

    property color selectedColor: 'transparent'

    color: 'transparent'

    Connections {
        target: rootItem
        function onSelectedColorChanged() {
            let found = -1
            for (var i = 0; i < colorsrowl.mainModel.length; i++) {
                if (selectedColor.toString().toUpperCase(
                            ) === colorsrowl.mainModel[i])
                    found = i
            }

            if (found === -1) {
                mainRepeater.currentIndex = -1
            }
        }
    }

    RowLayout {
        id: colorsrowl

        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 2

        property var mainModel: ["#EF2929", "#FCAF3E", "#FCE94F", "#8AE234", "#729FCF", "#FFFFFF"]

        Repeater {
            id: mainRepeater

            model: colorsrowl.mainModel

            property int currentIndex: -1

            Button {
                required property var model

                padding: 0

                checkable: true
                implicitWidth: 26
                implicitHeight: 26

                background: Rectangle {
                    radius: width
                    color: colorsrowl.mainModel[model.index]
                }

                Image {
                    anchors.fill: parent
                    source: "qrc:/Resources/add-place-color-select.png"
                    visible: mainRepeater.currentIndex === model.index
                }
                onClicked: {
                    mainRepeater.currentIndex = model.index
                    colorBoxRepeater.currentIndex = -1
                    historyRepeater.currentIndex = -1
                    customColorSetting.isCustomSet = false

                    rootItem.selectedColor = colorsrowl.mainModel[model.index]
                    popupItem.visible = false
                    //                    propertySelectIcon.visible = false
                    //                    addIconImage.visible = true
                }
            }
        }

        Rectangle {
            id: popupItemOpener

            implicitWidth: 26
            implicitHeight: 26
            radius: width
            border.width: 1
            border.color: Style.foregroundColor
            color: mainRepeater.currentIndex === -1 ? rootItem.selectedColor : 'transparent'

            IconImage {
                id: addIconImage
                anchors.centerIn: parent
                width: 24
                height: 24
                source: "qrc:/Resources/location-add.png"
            }
            //            IconImage {
            //                id: propertySelectIcon
            //                anchors.centerIn: parent
            //                width: 20
            //                height: 20
            //                source: "qrc:/Resources/add-place-color-select.png"
            //                visible: false
            //            }
            MouseArea {
                anchors.fill: parent
                onClicked: popupItem.visible = true
            }
        }
    }

    Popup {
        id: popupItem

        property int maximumhistoryModelLength: 8
        property color sliderColor: Style.backgroundColor

        property var colorBoxModel: ["#EF2929", "#FCAF3E", "#FCE94F", "#8AE234", "#729FCF", "#AD7FA8", "#E9B96E", "#888A85", "#EEEEEC", "#C00", "#F57900", "#EDD400", "#73D216", "#346594", "#75507B", "#C17D11", "#555753", "#D3D7CF", "#A40000", "#CE5C00", "#C4A000", "#4E9A06", "#204A87", "#5C3566", "#8F5902", "#2E3436", "#BABDB6"]
        property list<color> historyModel: []

        function makeColorFromText(text) {
            return "#" + "0".repeat(6 - text.length) + text
        }

        function intStringToHexString(text) {
            return parseInt(text, 10).toString(16)
        }

        function addTohistoryModel(newColor) {
            let found = -1
            for (var i = 0; i < historyModel.length; i++) {
                //            print(historyModel[i], newColor.toLowerCase())
                if (historyModel[i] == newColor.toLowerCase()) {
                    found = i
                }
            }

            if (found === -1) {
                historyModel.unshift(newColor)
            }

            if (historyModel.length > maximumhistoryModelLength) {
                historyModel.pop()
            }
        }

        // absolute position
        parent: Overlay.overlay
        x: 90
        y: 200

        z: 10

        visible: false

        background: Rectangle {
            color: Style.backgroundColor
            width: 288
            height: 371
            radius: 20
        }

        contentItem: ColumnLayout {
            id: popupColumn
            anchors.fill: parent
            anchors.topMargin: 15
            anchors.leftMargin: 15
            anchors.rightMargin: 15
            spacing: 0

            Text {
                text: "Select Color"
                font.pixelSize: 17
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.bottomMargin: 10
            }

            Flow {
                Layout.fillWidth: true
                Layout.bottomMargin: 15
                spacing: 3

                Repeater {
                    id: colorBoxRepeater

                    property int currentIndex: -1

                    model: popupItem.colorBoxModel

                    Button {
                        implicitWidth: 26
                        implicitHeight: 26
                        checkable: true

                        background: Rectangle {
                            radius: width
                            color: popupItem.colorBoxModel[model.index]
                        }

                        Image {
                            id: checkIcon
                            anchors.centerIn: parent
                            anchors.fill: parent
                            source: "qrc:/Resources/add-place-color-select.png"
                            visible: model.index === colorBoxRepeater.currentIndex
                        }

                        onClicked: {
                            colorBoxRepeater.currentIndex = model.index
                            historyRepeater.currentIndex = -1
                            mainRepeater.currentIndex = -1
                            customColorSetting.isCustomSet = false

                            popupItem.sliderColor = popupItem.colorBoxModel[model.index]
                            popupItem.addTohistoryModel(
                                        popupItem.colorBoxModel[model.index])
                        }
                    }
                }
            }

            Text {
                text: "Custom"
                font.pixelSize: 15
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.bottomMargin: 5
            }

            RowLayout {
                id: customColorSetting

                property bool isCustomSet: false

                Layout.bottomMargin: 15
                spacing: 0

                Rectangle {
                    id: customColorCircle
                    width: 26
                    height: 26
                    radius: 26
                    border.color: Style.foregroundColor
                    border.width: 1
                }

                Text {
                    text: "HEX"
                    font.pixelSize: 12
                    Layout.leftMargin: 9
                    color: Style.foregroundColor
                }

                Rectangle {
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 60
                    Layout.leftMargin: 5
                    border.color: "black"
                    border.width: 1
                    color: Style.backgroundColor
                    radius: 5

                    TextInput {
                        id: hexInput
                        anchors.fill: parent
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        validator: RegularExpressionValidator {
                            regularExpression: /[0-9A-F]{1,6}/i
                        }
                        onAccepted: {
                            customColorCircle.color = popupItem.makeColorFromText(
                                        text)
                            popupItem.sliderColor = customColorCircle.color
                            rInput.text = parseInt(
                                        customColorCircle.color.r * 255)
                            gInput.text = parseInt(
                                        customColorCircle.color.g * 255)
                            bInput.text = parseInt(
                                        customColorCircle.color.b * 255)

                            customColorSetting.isCustomSet = true
                            colorBoxRepeater.currentIndex = -1
                            historyRepeater.currentIndex = -1
                            mainRepeater.currentIndex = -1
                        }
                    }
                }

                Text {
                    text: "R"
                    font.pixelSize: 12
                    Layout.leftMargin: 15
                    color: Style.foregroundColor
                }

                Rectangle {
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 20
                    Layout.leftMargin: 5
                    border.color: "black"
                    border.width: 1
                    color: Style.backgroundColor
                    radius: 5

                    TextInput {
                        id: rInput
                        anchors.fill: parent
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        validator: IntValidator {
                            bottom: 0
                            top: 255
                        }
                        onAccepted: {
                            customColorCircle.color.r = "0".repeat(
                                        2 - intStringToHexString(
                                            text).length) + intStringToHexString(
                                        text)
                            sliderColor = customColorCircle.color
                            selectedColor = customColorCircle.color
                            customColorSetting.isCustomSet = true

                            hexInput.text = customColorCircle.color.toString(
                                        ).replace("#", "")

                            customColorSetting.isCustomSet = true
                            colorBoxRepeater.currentIndex = -1
                            historyRepeater.currentIndex = -1
                            mainRepeater.currentIndex = -1
                        }
                    }
                }

                Text {
                    text: "G"
                    font.pixelSize: 12
                    Layout.leftMargin: 10
                    color: Style.foregroundColor
                }

                Rectangle {
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 20
                    Layout.leftMargin: 5
                    border.color: "black"
                    border.width: 1
                    color: Style.backgroundColor
                    radius: 5

                    TextInput {
                        id: gInput
                        anchors.fill: parent
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        validator: IntValidator {
                            bottom: 0
                            top: 255
                        }
                        onAccepted: {
                            customColorCircle.color.g = "0".repeat(
                                        2 - intStringToHexString(
                                            text).length) + intStringToHexString(
                                        text)

                            sliderColor = customColorCircle.color
                            selectedColor = customColorCircle.color
                            customColorSetting.isCustomSet = true

                            hexInput.text = customColorCircle.color.toString(
                                        ).replace("#", "")

                            customColorSetting.isCustomSet = true
                            colorBoxRepeater.currentIndex = -1
                            historyRepeater.currentIndex = -1
                            mainRepeater.currentIndex = -1
                        }
                    }
                }

                Text {
                    text: "B"
                    font.pixelSize: 12
                    Layout.leftMargin: 10
                    color: Style.foregroundColor
                }

                Rectangle {
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 20
                    Layout.leftMargin: 5
                    border.color: "black"
                    border.width: 1
                    color: Style.backgroundColor
                    radius: 5

                    TextInput {
                        id: bInput
                        anchors.fill: parent
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        validator: IntValidator {
                            bottom: 0
                            top: 255
                        }
                        onAccepted: {
                            customColorCircle.color.b = "0".repeat(
                                        2 - intStringToHexString(
                                            text).length) + intStringToHexString(
                                        text)

                            sliderColor = customColorCircle.color
                            selectedColor = customColorCircle.color
                            customColorSetting.isCustomSet = true

                            hexInput.text = customColorCircle.color.toString(
                                        ).replace("#", "")

                            customColorSetting.isCustomSet = true
                            colorBoxRepeater.currentIndex = -1
                            historyRepeater.currentIndex = -1
                            mainRepeater.currentIndex = -1
                        }
                    }
                }
            }

            Text {
                text: "Opacity"
                font.pixelSize: 15
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.bottomMargin: 3
            }

            RowLayout {
                Layout.bottomMargin: 15
                spacing: 0

                Text {
                    text: Math.trunc(slider.value * 100).toString() + "%"
                    color: Style.foregroundColor
                    font.pixelSize: 12
                    Layout.preferredWidth: 30
                }

                Slider {
                    id: slider
                    value: 1
                    Layout.leftMargin: 5
                    background: Rectangle {
                        x: slider.leftPadding
                        y: slider.topPadding + slider.availableHeight / 2 - height / 2
                        implicitWidth: 225
                        implicitHeight: 10
                        border.width: 1
                        border.color: Style.foregroundColor
                        width: slider.availableWidth
                        height: implicitHeight
                        radius: 10
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop {
                                position: 0.0
                                color: "#00" + popupItem.sliderColor.toString(
                                           ).replace("#", "")
                            }
                            GradientStop {
                                position: 1.0
                                color: "#FF" + popupItem.sliderColor.toString(
                                           ).replace("#", "")
                            }
                        }
                    }
                    handle: Rectangle {
                        x: slider.leftPadding + slider.visualPosition
                           * (slider.availableWidth - width)
                        y: slider.topPadding + slider.availableHeight / 2 - height / 2
                        implicitWidth: 10
                        implicitHeight: 10
                        radius: 10
                        color: Style.backgroundColor
                        border.color: Style.foregroundColor
                    }
                    onMoved: {
                        customColorCircle.color.a = value
                    }
                }
            }

            Text {
                text: "Recent Color"
                font.pixelSize: 15
                color: Style.foregroundColor
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.bottomMargin: 10
            }

            RowLayout {
                Layout.bottomMargin: 15
                Layout.preferredHeight: 26
                spacing: 3

                Repeater {
                    id: historyRepeater
                    property int currentIndex: -1
                    model: popupItem.historyModel

                    Button {
                        implicitWidth: 26
                        implicitHeight: 26
                        checkable: true
                        background: Rectangle {
                            radius: width
                            color: popupItem.historyModel[model.index]
                        }

                        Image {
                            id: historyModelCheckIcon
                            anchors.centerIn: parent
                            anchors.fill: parent
                            source: "qrc:/Resources/add-place-color-select.png"
                            visible: model.index === historyRepeater.currentIndex
                        }

                        onClicked: {
                            historyRepeater.currentIndex = model.index
                            colorBoxRepeater.currentIndex = -1
                            mainRepeater.currentIndex = -1
                            customColorSetting.isCustomSet = false

                            popupItem.sliderColor = popupItem.historyModel[model.index]
                        }
                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 24

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Button {
                    padding: 0
                    Layout.fillHeight: true
                    Layout.preferredWidth: 65
                    contentItem: Text {
                        text: "Cancel"
                        font.pixelSize: 15
                        color: Style.foregroundColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 10
                        color: Style.backgroundColor
                        border.width: 1
                        border.color: Style.foregroundColor
                    }
                    onClicked: {
                        popupItem.visible = false
                    }
                }

                Button {
                    id: doneBtn
                    padding: 0
                    Layout.fillHeight: true
                    Layout.preferredWidth: 65

                    contentItem: Text {
                        text: "Done"
                        font.pixelSize: 15
                        color: Style.backgroundColor
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 10
                        color: Style.foregroundColor
                    }
                    onClicked: {
                        if (customColorSetting.isCustomSet) {
                            selectedColor = Qt.rgba(customColorCircle.color.r,
                                                    customColorCircle.color.g,
                                                    customColorCircle.color.b,
                                                    slider.value)
                        } else {
                            if (historyRepeater.currentIndex !== -1
                                    || colorBoxRepeater.currentIndex !== -1) {
                                selectedColor = historyRepeater.currentIndex
                                        === -1 ? popupItem.colorBoxModel[colorBoxRepeater.currentIndex] : popupItem.historyModel[historyRepeater.currentIndex]

                                selectedColor = Qt.rgba(selectedColor.r,
                                                        selectedColor.g,
                                                        selectedColor.b,
                                                        slider.value)
                            }
                        }

                        popupItem.visible = false
                    }
                }
            }
        }
    }
}

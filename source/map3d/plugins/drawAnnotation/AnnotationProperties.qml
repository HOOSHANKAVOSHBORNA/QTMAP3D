import QtQuick 2.13
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs
import "style"

Item {
    anchors.fill: parent
    id: rootItem
    implicitHeight: parent ? parent.height : 0
    readonly property color foregroundColor: Qt.rgba(Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.20)
    readonly property color backgroundColor: Qt.rgba(Style.backgroundColor.r,
                                                     Style.backgroundColor.g,
                                                     Style.backgroundColor.b,
                                                     0.30)
    property string _headerTitleSTR: "Properties"
    property var cppInterface
    property int swtchWidth: 100
    property int lblWidth: 85
    property int valHeight: 30
    property int previousIndex: 0
    property int nextIndex: 0
    property int strokePreviousIndex: 0
    property int strokeNextIndex: 0
    property int pointColorPreviousIndex: 0
    property int pointColorNextIndex: 0
    //////////////////////////Main Content////////////////////////
    Rectangle {
        id: item
        visible: true
        width: parent.width * 0.9
        height: rootItem.height
        anchors.fill: parent
        color: "transparent"
        anchors.margins: 6 / Style.monitorRatio
        radius: 10 / Style.monitorRatio

        ColorBoxInput {
            id: colorBox
            z: mainLayout.z + 10
            x: nameSec.x
            y: nameSec.y
            width: frame.width
            visible: false
            onColorChosen: {
                colorSelectCircle.color = selectedColor
                rootItem.cppInterface.fillColor = selectedColor
                addIconImage.visible = false
                propertyCheckIcon.visible = true
                colorModel.setProperty(previousIndex, "checkIconVisible", false)
            }
        }

        ColorBoxInput {
            id: colorBoxStroke
            z: mainLayout.z + 10
            x: nameSec.x
            y: nameSec.y
            width: frame.width
            visible: false
            onColorChosen: {
                strokeColorCircle.color = selectedColor
                rootItem.cppInterface.strokeColor = selectedColor
                strokeColorAddIcon.visible = false
                strokeCheckIcon.visible = true
                strokeColorModel.setProperty(strokePreviousIndex,
                                             "checkIconVisible", false)
            }
        }

        ColorBoxInput {
            id: colorBoxPointColor
            z: mainLayout.z + 10
            x: nameSec.x
            y: nameSec.y
            width: frame.width
            visible: false
            Layout.fillWidth: true
            Layout.fillHeight: true
            onColorChosen: {
                pointColorCircle.color = selectedColor
                rootItem.cppInterface.pointsColor = selectedColor
                pointColorAddIcon.visible = false
                pointColorCheckIcon.visible = true
                pointColorModel.setProperty(pointColorPreviousIndex,
                                            "checkIconVisible", false)
            }
        }

        ScrollView {
            id: frame
            clip: true
            contentWidth: parent.width
            anchors.fill: parent
            /////////////////////// components Grid ////////////////////////////
            ColumnLayout {
                id: mainLayout
                z: 0
                anchors.fill: parent
                spacing: 2 / Style.monitorRatio
                ////////////////////////////////// Name ////////////////////////////////////////////////
                RowLayout {
                    id: nameSec
                    Layout.fillWidth: true
                    spacing: 0
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Name"
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    TextField {
                        id: namevalue
                        Layout.preferredWidth: 200 / Style.monitorRatio
                        Layout.minimumWidth: 100 / Style.monitorRatio
                        Layout.fillWidth: true
                        Layout.rightMargin: 15 / Style.monitorRatio
                        height: valHeight
                        text: rootItem.cppInterface ? rootItem.cppInterface.name : ""
                        font.pointSize: 10 / Style.monitorRatio
                        color: "black"
                        background: Rectangle {
                            color: foregroundColor
                            radius: height / 2
                        }
                        onAccepted: {
                            rootItem.cppInterface.name = namevalue.displayText
                        }
                    }
                }

                RowLayout {
                    id: fillcolorSec
                    Layout.fillWidth: true
                    visible: rootItem.cppInterface ? rootItem.cppInterface.fillColorStatus : false
                    spacing: 0
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Color"
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    RowLayout {
                        Layout.rightMargin: 15 / Style.monitorRatio
                        Layout.fillWidth: true
                        spacing: 3 / Style.monitorRatio
                        ListModel {
                            id: colorModel
                            ListElement {
                                checkIconVisible: false
                                propertyColorSelect: "#EF2929"
                            }
                            ListElement {
                                checkIconVisible: false
                                propertyColorSelect: "#FCAF3E"
                            }
                            ListElement {
                                checkIconVisible: false
                                propertyColorSelect: "#FCE94F"
                            }
                            ListElement {
                                checkIconVisible: false
                                propertyColorSelect: "#8AE234"
                            }
                            ListElement {
                                checkIconVisible: false
                                propertyColorSelect: "#729FCF"
                            }
                            ListElement {
                                checkIconVisible: false
                                propertyColorSelect: "#FFFFFF"
                            }
                        }

                        Repeater {
                            id: colorModelRepeater
                            model: colorModel
                            Button {
                                required property color propertyColorSelect
                                required property bool checkIconVisible
                                required property int index

                                checkable: true
                                implicitWidth: 26 / Style.monitorRatio
                                implicitHeight: 26 / Style.monitorRatio
                                background: Rectangle {
                                    radius: width
                                    color: propertyColorSelect
                                }
                                Image {
                                    anchors.fill: parent
                                    source: "qrc:/Resources/add-place-color-select.png"
                                    visible: checkIconVisible
                                }
                                onClicked: {
                                    addIconImage.visible = true
                                    propertyCheckIcon.visible = false
                                    colorSelectCircle.color = Style.backgroundColor

                                    colorModel.setProperty(previousIndex,
                                                           "checkIconVisible",
                                                           false)
                                    nextIndex = index
                                    if (previousIndex !== nextIndex) {
                                        colorModelRepeater.itemAt(
                                                    previousIndex).checked = false
                                    }
                                    colorModel.setProperty(nextIndex,
                                                           "checkIconVisible",
                                                           checked)
                                    if (checked) {
                                        previousIndex = index
                                        rootItem.cppInterface.fillColor = propertyColorSelect
                                    } else {
                                        rootItem.cppInterface.fillColor = "#FFFFFF"
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id: colorSelectCircle
                            implicitWidth: 26 / Style.monitorRatio
                            implicitHeight: 26 / Style.monitorRatio
                            radius: width
                            border.width: 1 / Style.monitorRatio
                            border.color: Style.foregroundColor
                            IconImage {
                                id: addIconImage
                                anchors.centerIn: parent
                                width: 20 / Style.monitorRatio
                                height: 20 / Style.monitorRatio
                                source: "qrc:/Resources/location-add.png"
                            }
                            IconImage {
                                id: propertyCheckIcon
                                anchors.centerIn: parent
                                width: 20 / Style.monitorRatio
                                height: 20 / Style.monitorRatio
                                source: "qrc:/Resources/add-place-color-select.png"
                                visible: false
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: colorBox.visible = true
                            }
                        }
                    }
                }
                RowLayout {
                    spacing: 0
                    Text {
                        text: "Stroke"
                        color: Style.foregroundColor
                        font.pixelSize: 17 / Style.monitorRatio
                        Layout.alignment: Qt.AlignTop
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        visible: rootItem.cppInterface ? rootItem.cppInterface.strokeStatus : false

                    }
                    GroupBox {
                        id: strokeSec
                        visible: rootItem.cppInterface ? rootItem.cppInterface.strokeStatus : false
                        Layout.rightMargin: 15 / Style.monitorRatio
                        padding: 0
                        Layout.fillWidth: true
                        Layout.margins: 0

                        background: Rectangle {
                            color: foregroundColor
                            radius: 10 / Style.monitorRatio
                            border.color: "transparent"
                        }

                        ColumnLayout {
                            anchors.fill: parent

                            RowLayout {
                                spacing: 3 / Style.monitorRatio
                                Layout.leftMargin: 7 / Style.monitorRatio
                                Layout.topMargin: 5 / Style.monitorRatio
                                ListModel {
                                    id: strokeColorModel
                                    ListElement {
                                        checkIconVisible: false
                                        propertyColorSelect: "#EF2929"
                                    }
                                    ListElement {
                                        checkIconVisible: false
                                        propertyColorSelect: "#FCAF3E"
                                    }
                                    ListElement {
                                        checkIconVisible: false
                                        propertyColorSelect: "#FCE94F"
                                    }
                                    ListElement {
                                        checkIconVisible: false
                                        propertyColorSelect: "#8AE234"
                                    }
                                    ListElement {
                                        checkIconVisible: false
                                        propertyColorSelect: "#729FCF"
                                    }
                                    ListElement {
                                        checkIconVisible: false
                                        propertyColorSelect: "#FFFFFF"
                                    }
                                }
                                Repeater {
                                    id: strokeColorModelRepeater
                                    model: strokeColorModel
                                    Button {
                                        required property color propertyColorSelect
                                        required property bool checkIconVisible
                                        required property int index
                                        checkable: true
                                        implicitWidth: 26 / Style.monitorRatio
                                        implicitHeight: 26 / Style.monitorRatio
                                        background: Rectangle {
                                            radius: width
                                            color: propertyColorSelect
                                        }
                                        Image {
                                            anchors.centerIn: parent
                                            anchors.fill: parent
                                            source: "qrc:/Resources/add-place-color-select.png"
                                            visible: checkIconVisible
                                        }
                                        onClicked: {
                                            strokeColorAddIcon.visible = true
                                            strokeCheckIcon.visible = false
                                            strokeColorCircle.color = Style.backgroundColor

                                            strokeColorModel.setProperty(
                                                        strokePreviousIndex,
                                                        "checkIconVisible",
                                                        false)
                                            strokeNextIndex = index
                                            if (strokePreviousIndex !== strokeNextIndex) {
                                                strokeColorModelRepeater.itemAt(
                                                            strokePreviousIndex).checked = false
                                            }
                                            strokeColorModel.setProperty(
                                                        strokeNextIndex,
                                                        "checkIconVisible",
                                                        checked)
                                            if (checked) {
                                                rootItem.cppInterface.strokeColor = propertyColorSelect
                                                strokePreviousIndex = index
                                            } else {
                                                rootItem.cppInterface.strokeColor = "#FFFFFF"
                                            }
                                        }
                                    }
                                }

                                Rectangle {
                                    id: strokeColorCircle
                                    implicitWidth: 26 / Style.monitorRatio
                                    implicitHeight: 26 / Style.monitorRatio
                                    radius: width
                                    border.width: 1 / Style.monitorRatio
                                    border.color: Style.foregroundColor
                                    IconImage {
                                        id: strokeColorAddIcon
                                        anchors.centerIn: parent
                                        width: 20 / Style.monitorRatio
                                        height: 20 / Style.monitorRatio
                                        source: "qrc:/Resources/location-add.png"
                                    }
                                    IconImage {
                                        id: strokeCheckIcon
                                        anchors.fill: parent
                                        source: "qrc:/Resources/add-place-color-select.png"
                                        visible: false
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: colorBoxStroke.visible = true
                                    }
                                }
                            }

                            FloatSpinbox {
                                id: strokeWidthValue
                                editable: true
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignCenter
                                Layout.topMargin: 10 / Style.monitorRatio
                                Layout.bottomMargin: 5 / Style.monitorRatio
                                Layout.rightMargin: 5 / Style.monitorRatio
                                Layout.leftMargin: 5 / Style.monitorRatio
                                height: 20 / Style.monitorRatio
                                from: 0
                                to: 20000000
                                stepSize: 1
                                onValueChanged: {
                                    if (rootItem.cppInterface)
                                        rootItem.cppInterface.strokeWidth = value
                                }
                            }
                            Binding {
                                target: strokeWidthValue
                                property: "value"
                                value: rootItem.cppInterface ? rootItem.cppInterface.strokeWidth : 0
                                delayed: true
                            }
                        }
                    }
                }

                ////////------------------------- height------------------//////////////
                RowLayout {
                    id: heightSec
                    visible: rootItem.cppInterface ? rootItem.cppInterface.heightStatus : false
                    spacing: 0
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Height "
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                        Layout.alignment: Qt.AlignVCenter
                    }
                    Rectangle {
                        color: foregroundColor
                        radius: 10 / Style.monitorRatio
                        border.color: "transparent"
                        Layout.rightMargin: 15 / Style.monitorRatio
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredHeight: valHeight / Style.monitorRatio
                        Layout.preferredWidth: 200 / Style.monitorRatio
                        StepSpinBox {
                            id: heightValue
                            editable: true
                            anchors.leftMargin: 5 / Style.monitorRatio
                            anchors.rightMargin: 5 / Style.monitorRatio
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 30 / Style.monitorRatio
                            decimals: 2
                            from: 0
                            to: 20000000
                            onValueChanged: {
                                rootItem.cppInterface.heighT = value
                            }
                        }
                        Binding {
                            target: heightValue
                            property: "value"
                            value: rootItem.cppInterface ? rootItem.cppInterface.heighT : 0
                            delayed: true
                        }
                    }
                }


                // --------------------------------- clamp -----------------------////////////
                RowLayout {
                    id: clampSec
                    spacing: 0
                    visible: rootItem.cppInterface ? rootItem.cppInterface.clampStatus : false
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Clamp "
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }

                    Rectangle {
                        color: foregroundColor
                        radius: 10 / Style.monitorRatio
                        border.color: "transparent"
                        Layout.fillWidth: true
                        Layout.rightMargin: 15 / Style.monitorRatio
                        Layout.preferredHeight: valHeight / Style.monitorRatio

                        ComboBox {
                            id: clampCombo
                            model: ["None", "Terrain", "Relative", "Absolute"]
                            width: (parent.width - 5) / Style.monitorRatio
                            anchors.centerIn: parent
                            height: 25 / Style.monitorRatio

                            onCurrentIndexChanged: {
                                if (rootItem.cppInterface)
                                    rootItem.cppInterface.clamp = currentIndex
                            }

                            delegate: ItemDelegate {
                                width: clampCombo.width
                                contentItem: Text {
                                    text: clampCombo.textRole ? (Array.isArray(
                                                                     clampCombo.model) ? modelData[clampCombo.textRole] : model[clampCombo.textRole]) : modelData
                                    color: "#5f5f5f"
                                    font: clampCombo.font
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                }
                                highlighted: clampCombo.highlightedIndex === index
                            }

                            indicator: Canvas {
                                id: canvas
                                x: (clampCombo.width - width
                                    - clampCombo.rightPadding) / Style.monitorRatio
                                y: (clampCombo.topPadding + (clampCombo.availableHeight - height)
                                    / 2) / Style.monitorRatio
                                width: 12 / Style.monitorRatio
                                height: 8 / Style.monitorRatio
                                contextType: "2d"

                                Connections {
                                    target: clampCombo
                                    function onPressedChanged() {
                                        canvas.requestPaint()
                                    }
                                }
                            }

                            contentItem: Text {
                                leftPadding: 5 / Style.monitorRatio
                                rightPadding: clampCombo.indicator.width + clampCombo.spacing

                                text: clampCombo.displayText
                                font: clampCombo.font
                                color: clampCombo.pressed ? "#5f5f5f" : "#404040"
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                implicitWidth: 100 / Style.monitorRatio
                                implicitHeight: 22 / Style.monitorRatio
                                border.color: clampCombo.pressed ? "#5f5f5f" : "#404040"
                                border.width: clampCombo.visualFocus ? 2 : 1
                                radius: 5
                            }

                            popup: Popup {
                                y: (clampCombo.height - 1) / Style.monitorRatio
                                width: clampCombo.width
                                implicitHeight: contentItem.implicitHeight
                                padding: 1 / Style.monitorRatio

                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight / Style.monitorRatio
                                    model: clampCombo.popup.visible ? clampCombo.delegateModel : null
                                    currentIndex: clampCombo.highlightedIndex

                                    ScrollIndicator.vertical: ScrollIndicator {}
                                }

                                background: Rectangle {
                                    border.color: "#404040"
                                    radius: 5 / Style.monitorRatio
                                }
                            }
                        }
                    }
                }

                ////////-------------------------/tesselation/------------------//////////////
                RowLayout {
                    id: tesselationSec
                    visible: rootItem.cppInterface ? rootItem.cppInterface.tesselationStatus : false
                    spacing: 0
                    Text {
                        Layout.preferredWidth: lblWidth / Style.monitorRatio
                        text: "Tesselation "
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }

                    Rectangle {
                        color: foregroundColor
                        radius: 10 / Style.monitorRatio
                        border.color: "transparent"
                        Layout.fillWidth: true
                        Layout.preferredHeight: valHeight / Style.monitorRatio
                        Layout.rightMargin: 15 / Style.monitorRatio

                        FloatSpinbox {
                            id: tesselationValue
                            editable: true
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.leftMargin: 5 / Style.monitorRatio
                            anchors.rightMargin: 5 / Style.monitorRatio
                            anchors.verticalCenter: parent.verticalCenter
                            stepSize: 1
                            from: 0
                            to: 20000000
                            height: 20 / Style.monitorRatio
                            onValueChanged: {
                                rootItem.cppInterface.tesselation = value
                            }
                        }
                        Binding {
                            target: tesselationValue
                            property: "value"
                            value: rootItem.cppInterface ? rootItem.cppInterface.tesselation : 0
                            delayed: true
                        }
                    }
                }

                ////------------------------ Show lenght -------------------- ///////////////
                RowLayout {
                    id: showLenghtSec
                    visible: rootItem.cppInterface ? rootItem.cppInterface.showLenStatus : false
                    spacing: 0

                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Length "
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    Switch {
                        id: showLenghtValue
                        padding: 0
                        width: 100
                        height: valHeight / Style.monitorRatio
                        checked: false
                        onToggled: function () {
                            rootItem.cppInterface.showLen = showLenghtValue.checked
                        }
                    }
                    Binding {
                        target: showLenghtValue
                        property: "checked"
                        value: rootItem.cppInterface ? rootItem.cppInterface.showLen : 0
                        delayed: true
                    }
                }

                ////------------------------Show Bearing -------------------- ///////////////
                RowLayout {
                    id: bearingSec
                    visible: rootItem.cppInterface ? rootItem.cppInterface.bearingStatus : false
                    spacing: 0
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Bearing "
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    Switch {
                        id: bearingValue
                        padding: 0 / Style.monitorRatio
                        width: 100 / Style.monitorRatio
                        height: valHeight / Style.monitorRatio
                        checked: false
                        onToggled: function () {
                            rootItem.cppInterface.showBearing = bearingValue.checked
                        }
                    }
                    Binding {
                        target: bearingValue
                        property: "checked"
                        value: rootItem.cppInterface ? rootItem.cppInterface.showBearing : 0
                        delayed: true
                    }
                }

                ////------------------------  showAltitude -------------------- ///////////////
                RowLayout {
                    id: showAltitudeSec
                    visible: rootItem.cppInterface ? rootItem.cppInterface.altitudeStatus : false
                    spacing: 0
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Altitude "
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    Switch {
                        id: showAltitudeValue
                        padding: 0
                        width: 100 / Style.monitorRatio
                        height: valHeight / Style.monitorRatio
                        checked: false
                        onToggled: function () {
                            rootItem.cppInterface.showAltitude = showAltitudeValue.checked
                        }
                    }
                    Binding {
                        target: showAltitudeValue
                        property: "checked"
                        value: rootItem.cppInterface ? rootItem.cppInterface.showAltitude : 0
                        delayed: true
                    }
                }

                ////------------------------ Show Slope -------------------- ///////////////
                RowLayout {
                    id: slopeSec
                    visible: rootItem.cppInterface ? rootItem.cppInterface.showSlopStatus : false
                    spacing: 0
                    Text {
                        Layout.preferredWidth: swtchWidth / Style.monitorRatio
                        text: "Show Slope "
                        font.pixelSize: 17 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                    Switch {
                        id: slopeValue
                        padding: 0
                        width: 100 / Style.monitorRatio
                        height: valHeight / Style.monitorRatio
                        checked: false
                        onToggled: function () {
                            rootItem.cppInterface.showSlop = slopeValue.checked
                        }
                    }
                    Binding {
                        target: slopeValue
                        property: "checked"
                        value: rootItem.cppInterface ? rootItem.cppInterface.showSlop : 0
                        delayed: true
                    }
                }

                ////------------------------ Points  -------------------- ///////////////
                GroupBox {
                    id: pointSec
                    visible: rootItem.cppInterface ? rootItem.cppInterface.pointsStatus : false
                    padding: 0
                    Layout.fillWidth: true

                    background: Rectangle {
                        color: "transparent"
                        border.color: "transparent"
                    }

                    ColumnLayout {
                        anchors.fill: parent
                        RowLayout {
                            spacing: 0
                            Text {
                                Layout.preferredWidth: swtchWidth / Style.monitorRatio
                                text: "Points "
                                font.pixelSize: 17 / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            Switch {
                                id: pointVisible
                                padding: 0
                                width: 100
                                height: valHeight
                                checked: false
                                onToggled: function () {
                                    rootItem.cppInterface.pointsVisible = pointVisible.checked
                                }
                            }
                            Binding {
                                target: pointVisible
                                property: "checked"
                                value: rootItem.cppInterface ? rootItem.cppInterface.pointsVisible : 0
                                delayed: true
                            }
                        }
                        RowLayout {
                            spacing: 0
                            Text {
                                text: "Point Color"
                                color: Style.foregroundColor
                                font.pixelSize: 17 / Style.monitorRatio
                                Layout.alignment: Qt.AlignTop
                                Layout.preferredWidth: lblWidth / Style.monitorRatio
                                visible: rootItem.cppInterface ? rootItem.cppInterface.strokeStatus : false
                            }
                            GroupBox {
                                padding: 0
                                Layout.fillWidth: true
                                Layout.margins: 0
                                Layout.rightMargin: 15 / Style.monitorRatio
                                enabled: pointVisible.checked
                                background: Rectangle {
                                    color: foregroundColor
                                    radius: 10 / Style.monitorRatio
                                    border.color: "transparent"
                                }

                                ColumnLayout {
                                    anchors.fill: parent

                                    RowLayout {
                                        spacing: 3 / Style.monitorRatio
                                        Layout.leftMargin: 7 / Style.monitorRatio
                                        Layout.topMargin: 5 / Style.monitorRatio

                                        ListModel {
                                            id: pointColorModel
                                            ListElement {
                                                checkIconVisible: false
                                                propertyColorSelect: "#EF2929"
                                            }
                                            ListElement {
                                                checkIconVisible: false
                                                propertyColorSelect: "#FCAF3E"
                                            }
                                            ListElement {
                                                checkIconVisible: false
                                                propertyColorSelect: "#FCE94F"
                                            }
                                            ListElement {
                                                checkIconVisible: false
                                                propertyColorSelect: "#8AE234"
                                            }
                                            ListElement {
                                                checkIconVisible: false
                                                propertyColorSelect: "#729FCF"
                                            }
                                            ListElement {
                                                checkIconVisible: false
                                                propertyColorSelect: "#FFFFFF"
                                            }
                                        }
                                        Repeater {
                                            id: pointColorRepeater
                                            model: pointColorModel
                                            Button {
                                                required property color propertyColorSelect
                                                required property bool checkIconVisible
                                                required property int index
                                                checkable: true
                                                implicitWidth: 26 / Style.monitorRatio
                                                implicitHeight: 26 / Style.monitorRatio
                                                background: Rectangle {
                                                    radius: width
                                                    color: propertyColorSelect
                                                }
                                                Image {
                                                    anchors.fill: parent
                                                    source: "qrc:/Resources/add-place-color-select.png"
                                                    visible: checkIconVisible
                                                }

                                                onClicked: {
                                                    pointColorAddIcon.visible = true
                                                    pointColorCheckIcon.visible = false
                                                    pointColorCircle.color = Style.backgroundColor
                                                    pointColorModel.setProperty(
                                                                pointColorPreviousIndex,
                                                                "checkIconVisible",
                                                                false)
                                                    pointColorNextIndex = index
                                                    if (pointColorPreviousIndex
                                                            !== pointColorNextIndex) {
                                                        pointColorRepeater.itemAt(
                                                                    pointColorPreviousIndex).checked
                                                                = false
                                                    }
                                                    pointColorModel.setProperty(
                                                                pointColorNextIndex,
                                                                "checkIconVisible",
                                                                checked)
                                                    if (checked) {
                                                        pointColorPreviousIndex = index
                                                        rootItem.cppInterface.pointsColor
                                                                = propertyColorSelect
                                                    } else {
                                                        rootItem.cppInterface.pointsColor = "#FFFFFF"
                                                    }
                                                }
                                            }
                                        }


                                        Rectangle {
                                            id: pointColorCircle
                                            implicitWidth: 26 / Style.monitorRatio
                                            implicitHeight: 26 / Style.monitorRatio
                                            radius: width
                                            border.width: 1 / Style.monitorRatio
                                            border.color: Style.foregroundColor
                                            IconImage {
                                                id: pointColorAddIcon
                                                anchors.centerIn: parent
                                                width: 20 / Style.monitorRatio
                                                height: 20 / Style.monitorRatio
                                                source: "qrc:/Resources/location-add.png"
                                            }
                                            IconImage {
                                                id: pointColorCheckIcon
                                                anchors.fill: parent
                                                source: "qrc:/Resources/add-place-color-select.png"
                                                visible: false
                                            }
                                            MouseArea {
                                                anchors.fill: parent
                                                onClicked: colorBoxPointColor.visible = true
                                            }
                                        }
                                    }
                                    FloatSpinbox {
                                        id: pointOpacityValue
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 20 / Style.monitorRatio
                                        Layout.bottomMargin: 5 / Style.monitorRatio
                                        Layout.topMargin: 10 / Style.monitorRatio
                                        Layout.rightMargin: 5 / Style.monitorRatio
                                        Layout.leftMargin: 5 / Style.monitorRatio
                                        editable: true
                                        stepSize: 1
                                        height: 20 / Style.monitorRatio
                                        from: 0
                                        to: 100
                                        onValueChanged: {
                                            rootItem.cppInterface.pointsColor.a = value / 100
                                        }
                                    }
                                    Binding {
                                        target: pointOpacityValue
                                        property: "value"
                                        value: rootItem.cppInterface ? rootItem.cppInterface.pointsColor.a
                                                                * 100 : 0
                                        delayed: true
                                    }
                                }
                            }
                        }

                        RowLayout {
                            spacing: 0
                            Text {
                                text: "Point Width "
                                font.pixelSize: 17 / Style.monitorRatio
                                Layout.preferredWidth: lblWidth / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            Rectangle {
                                color: foregroundColor
                                radius: 10 / Style.monitorRatio
                                border.color: "transparent"
                                Layout.rightMargin: 15 / Style.monitorRatio
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignVCenter
                                Layout.preferredHeight: valHeight / Style.monitorRatio
                                Layout.preferredWidth: 200 / Style.monitorRatio

                                FloatSpinbox {
                                    id: pointWidthValue
                                    editable: true
                                    enabled: pointVisible.checked
                                    anchors.leftMargin: 5 / Style.monitorRatio
                                    anchors.rightMargin: 5 / Style.monitorRatio
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    height: 20 / Style.monitorRatio
                                    stepSize: 1
                                    from: 0
                                    to: 20000000
                                    onValueChanged: {
                                        rootItem.cppInterface.pointsWidth = value
                                    }
                                }
                                Binding {
                                    target: pointWidthValue
                                    property: "value"
                                    value: rootItem.cppInterface ? rootItem.cppInterface.pointsWidth : 0
                                    delayed: true
                                }
                            }
                        }
                        RowLayout {
                            spacing: 0
                            Text {
                                Layout.preferredWidth: swtchWidth / Style.monitorRatio
                                text: "Point Smooth "
                                font.pixelSize: 17 / Style.monitorRatio
                                color: Style.foregroundColor
                            }
                            Switch {
                                id: pointSmoothValue
                                enabled: pointVisible.checked
                                padding: 0
                                width: 100 / Style.monitorRatio
                                height: valHeight
                                checked: false
                                onToggled: function () {
                                    rootItem.cppInterface.pointsSmooth = pointSmoothValue.checked
                                }
                            }
                            Binding {
                                target: pointSmoothValue
                                property: "checked"
                                value: rootItem.cppInterface ? rootItem.cppInterface.pointsSmooth : 0
                                delayed: true
                            }
                        }
                    }
                }
            }
        }
    }
}

import QtQuick.Layouts
import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Crystal 1.0
import "style"


Rectangle {
    id: rootObj
    property var filterManager

    property real innerHeight : mainColumn.height

    readonly property color fg30: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.3)


    function isNumeric(s) {
        return !isNaN(s - parseFloat(s))
    }
    Rectangle{
        width: parent.width - 3/ Style.monitorRatio
        height: 20
        anchors.top: parent.top
        color: Qt.rgba(Style.backgroundColor.r,Style.backgroundColor.g,Style.backgroundColor.b,.2)
        z:2
        anchors.topMargin: mainColumn.height - 4 / Style.monitorRatio
        radius: 20
        Rectangle{
            width: parent.width
            height: 10/Style.monitorRatio
            color: Qt.rgba(Style.foregroundColor.r,Style.foregroundColor.g,Style.foregroundColor.b,0.03)
        }
    }

    color: Qt.rgba(Style.backgroundColor.r,Style.backgroundColor.g,Style.backgroundColor.b,.5)
    width: parent.width
    radius: 15


    ColumnLayout {
        id:mainColumn
        width: parent.width

        Flow {
            id: filterFields
            width: parent.width
            spacing: 15
            Layout.topMargin: 10 / Style.monitorRatio
            Layout.leftMargin: 15 / Style.monitorRatio
            RowLayout {
                width: parent.width
                RowLayout {
                    Label {
                        width: 36 / Style.monitorRatio
                        height: 18 / Style.monitorRatio
                        text: "Logical Operator :"
                        font.pixelSize: 15 / Style.monitorRatio
                        font.family: Style.fontFamily
                        color: Style.foregroundColor
                    }

                    RadioButton {
                        id: andCheck
                        checked: true
                    }

                    Label {
                        width: 36 / Style.monitorRatio
                        height: 18 / Style.monitorRatio
                        text: "And"
                        font.pixelSize: 15 / Style.monitorRatio
                        font.family: Style.fontFamily
                        color: Style.foregroundColor

                    }

                    RadioButton {
                        id: orCheck
                    }

                    Label {
                        width: 36 / Style.monitorRatio
                        height: 18 / Style.monitorRatio
                        text: "Or"
                        font.pixelSize: 15 / Style.monitorRatio
                        font.family: Style.fontFamily
                        color: Style.foregroundColor

                    }
                }
            }

            RowLayout {
                Label {
                    width: 36 / Style.monitorRatio
                    height: 18 / Style.monitorRatio
                    text: "Color"
                    font.pixelSize: 15 / Style.monitorRatio
                    font.family: Style.fontFamily
                    color: Style.foregroundColor
                }
                IconImage {
                    id: colorLeftIcon
                    source: "qrc:/Resources/down.png"
                    Layout.preferredHeight: 18 / Style.monitorRatio
                    Layout.preferredWidth: 18 / Style.monitorRatio
                    rotation: 90
                    color: colorRepeater.currentIndex < 1 ? Style.backgroundColor : "transparent"
                    visible: colorRepeater.count > 4
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorRepeater.decrementCurrentIndex();
                            colorRepeater.positionViewAtIndex(colorRepeater.currentIndex, ListView.Beginning)


                        }
                    }
                }
                Item {
                    width: 115 / Style.monitorRatio
                    height: 24 / Style.monitorRatio
                    clip: true
                    ListView {
                        id: colorRepeater
                        width: parent.width
                        height: 24 / Style.monitorRatio
                        orientation: ListView.Horizontal
                        layoutDirection: Qt.LeftToRight
                        model: rootObj.filterManager ? filterManager.colorFilterFields: 0
                        delegate: Rectangle {
                            width: 24 / Style.monitorRatio
                            height: 24 / Style.monitorRatio
                            radius: height / 2
                            color: model.display
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    filterManager.addFilterTag("Color",
                                                               model.display, Tag.Equal, andCheck.checked ? Tag.And : Tag.Or)
                                }
                            }
                        }
                        flickableDirection: Flickable.VerticalFlick
                        boundsBehavior: Flickable.StopAtBounds
                    }
                }
                IconImage {
                    id: colorRightIcon
                    source: "qrc:/Resources/down.png"
                    Layout.preferredHeight: 18 / Style.monitorRatio
                    Layout.preferredWidth: 18 / Style.monitorRatio
                    rotation: -90
                    color: colorRepeater.currentIndex > colorRepeater.count - 5 ? Style.backgroundColor : "transparent"
                    visible:colorRepeater.count > 4

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (colorRepeater.currentIndex < colorRepeater.count - 4)
                                colorRepeater.incrementCurrentIndex();
                            colorRepeater.positionViewAtIndex(colorRepeater.currentIndex, ListView.Beginning)
                        }
                    }
                }
            }

            Rectangle {
                id: stringFilter
                width: 204 / Style.monitorRatio
                height: 28 / Style.monitorRatio
                Layout.leftMargin: 15 / Style.monitorRatio
                radius: 10
                property color s: "black"
                color: Qt.rgba(s.r, s.g, s.b, .04)
                Rectangle {
                    width: stringFilter.width - 3
                    height: 28 / Style.monitorRatio - 3
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    color: Qt.rgba(Style.backgroundColor.r,Style.backgroundColor.g,Style.backgroundColor.b,.6)

                    radius: 8
                    RowLayout {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.leftMargin: 5 / Style.monitorRatio
                        // anchors.right: parent.right
                        spacing: 5 / Style.monitorRatio
                        ComboBox {
                            id: control
                            model: rootObj.filterManager ? filterManager.stringFilterFields: 0

                            textRole: "display"
                            valueRole: "display"
                            background: Rectangle {
                                color: "transparent"
                            }
                            popup: Popup {
                                y: control.height - 5
                                width: 100
                                implicitHeight: contentItem.implicitHeight
                                padding: 1
                                leftPadding: 5
                                rightPadding: 5


                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight
                                    model: control.delegateModel
                                    currentIndex: control.highlightedIndex

                                    ScrollIndicator.vertical: ScrollIndicator { }
                                }

                                background: Rectangle {
                                    border.color: Style.foregroundColor
                                    radius: 8 * Style.monitorRatio
                                }

                            }

                            delegate: ItemDelegate {
                                hoverEnabled: true
                                width: 100
                                background: Rectangle {
                                    height: parent.height
                                    color: "white"
                                    radius: 20
                                    border.width: .3
                                    border.color: "black"
                                }

                                contentItem: Label {
                                    text: model.display
                                    color: Style.foregroundColor
                                    font.family: Style.fontFamily
                                    font.pixelSize: 14 / Style.monitorRatio
                                    background: Rectangle {
                                        color: "transparent"
                                    }
                                }
                            }
                            onFocusChanged: {
                                if (focus)
                                    control.popup.open()
                            }
                            onCurrentIndexChanged: {
                                txtContentItem.text = control.textAt(control.highlightedIndex)
                                control.currentText = txtContentItem.text
                            }

                            indicator: Rectangle {}
                            contentItem: TextField {
                                id: txtContentItem
                                implicitWidth: 60 / Style.monitorRatio
                                Layout.fillHeight: true
                                placeholderText: qsTr("subject")
                                placeholderTextColor: rootObj.fg30
                                color: rootObj.fg30
                                font.family: Style.fontFamily
                                font.pixelSize: 15 / Style.monitorRatio
                                selectedTextColor: Style.backgroundColor
                                selectionColor: Style.foregroundColor
                                background: Rectangle {
                                    id: redBackG
                                    color: "transparent"
                                }
                                validator: DoubleValidator {}
                                PropertyAnimation {
                                    id: rbg
                                    target: redBackG
                                    properties: "opacity"
                                    to: 0
                                    from: 1
                                    duration: 2500
                                    easing.type: Easing.OutQuint

                                }

                                onTextChanged: {
                                    control.popup.open()
                                    var searchFilter = txtContentItem.text
                                    control.model.setFilterFixedString(searchFilter)
                                }
                            }
                        }

                        Label {
                            text: ":"
                            font.pixelSize: 15 / Style.monitorRatio
                            font.family: Style.fontFamily
                            color: rootObj.fg30
                        }
                        TextField {
                            id: descriptionField
                            implicitWidth: 95 / Style.monitorRatio
                            placeholderText: qsTr("Description")
                            color: Style.foregroundColor
                            font.family: Style.fontFamily
                            font.pixelSize: 15 / Style.monitorRatio
                            selectedTextColor: Style.backgroundColor
                            selectionColor: Style.foregroundColor
                            placeholderTextColor: rootObj.fg30
                            background: Rectangle {
                                color: "transparent"
                            }

                            onAccepted: {
                                if (control.indexOfValue(txtContentItem.text) === -1) {
                                    redBackG.color = "red"
                                    rbg.running = true
                                    return
                                }
                                filterManager.addFilterTag(txtContentItem.text,
                                                           descriptionField.text,
                                                           Tag.Equal,
                                                           andCheck.checked ? Tag.And : Tag.Or)
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: filterCompare
                width: 204 / Style.monitorRatio
                height: 28 / Style.monitorRatio
                Layout.leftMargin: 15 / Style.monitorRatio
                radius: 15
                property color s: "black"
                color: Qt.rgba(s.r, s.g, s.b, .04)
                Rectangle {
                    width: filterCompare.width  - 3
                    height: 28 / Style.monitorRatio - 3
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    color: Qt.rgba(Style.backgroundColor.r,Style.backgroundColor.g,Style.backgroundColor.b,.6)
                    radius: 8
                    RowLayout {
                        anchors.left: parent.left
                        anchors.leftMargin: 5 / Style.monitorRatio
                        anchors.top: parent.top
                        anchors.topMargin: -3
                        spacing: 5 / Style.monitorRatio
                        ComboBox {
                            id: control3
                            textRole: "display"
                            valueRole: "display"
                            model: rootObj.filterManager?filterManager.numFilterFields :0
                            background: Rectangle {
                                color: "transparent"
                            }
                            popup: Popup {
                                y: control3.height - 5
                                width: 100
                                implicitHeight: contentItem.implicitHeight
                                padding: 1
                                leftPadding: 5
                                rightPadding: 5


                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight
                                    model: control3.delegateModel
                                    currentIndex: control3.highlightedIndex

                                    ScrollIndicator.vertical: ScrollIndicator { }
                                }

                                background: Rectangle {
                                    border.color: Style.foregroundColor
                                    radius: 8 * Style.monitorRatio
                                }

                            }

                            delegate: ItemDelegate {
                                hoverEnabled: true
                                width: 100
                                // height: 20
                                background: Rectangle {
                                    height: parent.height
                                    color: "white"
                                    radius: 20
                                    border.width: .3
                                    border.color: "black"
                                }

                                contentItem: Label {
                                    text: model.display
                                    color: Style.foregroundColor
                                    font.family: Style.fontFamily
                                    font.pixelSize: 14 / Style.monitorRatio
                                    background: Rectangle {
                                        color: "transparent"
                                    }
                                }
                                onClicked: {
                                    control3.currentIndex = index
                                    txtContentItem3.text = model.display
                                }
                            }
                            onFocusChanged: {
                                if (focus)
                                    control3.popup.open()

                            }

                            indicator: Rectangle {}
                            contentItem: TextField {
                                id: txtContentItem3
                                implicitWidth:80 / Style.monitorRatio
                                Layout.fillHeight: true
                                placeholderText: qsTr("subject")
                                placeholderTextColor: rootObj.fg30
                                color: rootObj.fg30
                                font.family: Style.fontFamily
                                font.pixelSize: 15 / Style.monitorRatio
                                selectedTextColor: Style.backgroundColor
                                selectionColor: Style.foregroundColor

                                background: Rectangle {
                                    id: redBackG3
                                    color: "transparent"
                                }
                                validator: DoubleValidator {}
                                PropertyAnimation {
                                    id: rbg3
                                    target: redBackG3
                                    properties: "opacity"
                                    to: 0
                                    from: 1
                                    duration: 2500
                                    easing.type: Easing.OutQuint

                                }

                                onTextChanged: {
                                    control3.popup.open()
                                    var searchFilter = txtContentItem3.text
                                    control3.model.setFilterFixedString(searchFilter)
                                }
                            }
                        }
                        // -----------------------------------------------
                        Item {
                            width: 26 / Style.monitorRatio
                            height: 26 / Style.monitorRatio
                            Rectangle {
                                id: comparison
                                anchors.fill: parent
                                radius: width / 2
                                color: Style.backgroundColor
                                anchors.verticalCenter: parent.verticalCenter
                                Label {
                                    id: lblComparision
                                    anchors.centerIn: parent
                                    text: "="
                                    font.pixelSize: 16 / Style.monitorRatio
                                    font.family: Style.fontFamily
                                    color: Style.foregroundColor
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            comparisonMenu.popup()
                                        }
                                    }
                                    Menu {
                                        id: comparisonMenu
                                        width: 40
                                        background: Rectangle {
                                            width: 40
                                            color: Style.backgroundColor
                                            border.width: .3
                                            border.color: Style.backgroundColor
                                            radius: 8*Style.monitorRatio
                                        }
                                        Repeater {
                                            id: repeaterMenu

                                            model: ["!=", "<", "<=", "=", ">=", ">"]
                                            MenuItem {
                                                text: modelData

                                                background: Rectangle {
                                                    width: 40
                                                    color: Style.backgroundColor
                                                    border.width: .3
                                                    border.color: Style.backgroundColor
                                                    radius: 8*Style.monitorRatio
                                                }
                                                contentItem: Text {
                                                    text: modelData
                                                    color: "#003569"
                                                    font.family: "Roboto"
                                                    font.pixelSize: 14 / Style.monitorRatio
                                                }
                                                onClicked: {
                                                    lblComparision.text = modelData
                                                }
                                            }
                                        }

                                        enter: Transition {
                                            NumberAnimation {
                                                property: "opacity"
                                                from: 0.0
                                                to: 1.0
                                            }
                                        }

                                        exit: Transition {
                                            NumberAnimation {
                                                property: "opacity"
                                                from: 1.0
                                                to: 0.0
                                            }
                                        }
                                    }
                                }
                            }
                            MultiEffect {
                                source: comparison
                                enabled: true
                                anchors.fill: comparison
                                shadowColor: "black"
                                shadowEnabled: true
                                shadowBlur: 1
                                shadowHorizontalOffset: 0.5
                                shadowVerticalOffset: 0
                                shadowOpacity: 1
                                shadowScale: 0.6
                            }
                        }

                        TextField {
                            id: numbfield3
                            implicitWidth: 60 / Style.monitorRatio
                            Layout.fillHeight: true
                            placeholderText: qsTr("Numb")
                            color: Style.foregroundColor
                            font.family: Style.fontFamily
                            font.pixelSize: 15 / Style.monitorRatio
                            selectedTextColor: Style.backgroundColor
                            selectionColor: Style.foregroundColor
                            placeholderTextColor: rootObj.fg30
                            background: Rectangle {
                                color: "transparent"
                            }
                            onAccepted: {
                                if (control3.indexOfValue(txtContentItem3.text) === -1) {
                                    redBackG3.color = "red"
                                    rbg3.running = true
                                    return
                                }

                                else {
                                    filterManager.addFilterTag(
                                                txtContentItem3.text,
                                                parseFloat(numbfield3.text),
                                                rootObj.comparetor(lblComparision.text),
                                                andCheck.checked ? Tag.And : Tag.Or)
                                }
                            }
                        }
                    }
                }
            }

        }

        ScrollView {
            id: mainScroll
            Layout .topMargin: 10 / Style.monitorRatio
            Layout.preferredHeight: 50 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                width: rootObj.width
                visible: true
                clip: true

                Flow {
                    spacing: 1
                    Layout.fillWidth: true
                    Layout.leftMargin: 3 / Style.monitorRatio

                    Repeater {
                        id: tagsRep
                        model:rootObj.filterManager ? rootObj.filterManager.filterTags : 0
                        delegate: Rectangle {
                            property color colorHandler: modelData.isEnabled ? "#01AED6" : Style.foregroundColor
                            id: typesHolder
                            implicitHeight: 22 / Style.monitorRatio
                            implicitWidth: showDetails.implicitWidth + (index ? andOrLabel.implicitWidth + 5 : 0)
                            color: "transparent"
                            radius: 20
                            Rectangle{
                                id: borderRect
                                implicitHeight: 22 / Style.monitorRatio
                                implicitWidth: showDetails.implicitWidth
                                anchors.right: parent.right
                                color: mouseArea.containsMouse ? Qt.rgba(Style.foregroundColor.r,
                                                                         Style.foregroundColor.g,
                                                                         Style.foregroundColor.b, 0.1): "transparent"
                                border {
                                    color: colorHandler
                                    width: 1
                                }
                                radius: 20
                            }
                            Label {
                                id:andOrLabel
                                anchors.right: borderRect.left
                                anchors.rightMargin: 5 / Style.monitorRatio
                                anchors.verticalCenter: parent.verticalCenter
                                text: modelData.logicalOperator === Tag.And ? "&&" : "||"
                                font.pixelSize: 15 / Style.monitorRatio
                                font.family: "Roboto"
                                color: typesHolder.colorHandler
                                visible: index ? true : false
                            }

                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    modelData.toggleIsEnabled()
                                }
                            }

                            RowLayout {
                                id: showDetails
                                anchors.right:  parent.right
                                Text {
                                    Layout.alignment: Qt.AlignLeft
                                    text: modelData.field ? modelData.field : 0
                                    font.family: "Roboto"
                                    font.pixelSize: 15 / Style.monitorRatio
                                    color: typesHolder.colorHandler /*typesHolder.checked ? Style.foregroundColor : rootObj.hoverColor*/
                                    Layout.leftMargin: 15 / Style.monitorRatio
                                    Layout.topMargin: 2 / Style.monitorRatio
                                    Layout.bottomMargin: 2 / Style.monitorRatio
                                }
                                Label {
                                    text: modelData.comparisionToString()
                                    font.pixelSize: 15 / Style.monitorRatio
                                    font.family: "Roboto"
                                    color: typesHolder.colorHandler
                                }

                                Text {
                                    id: itemValue
                                    Layout.alignment: Qt.AlignLeft
                                    text: modelData.value ? modelData.value : 0
                                    font.family: "Roboto"
                                    font.pixelSize: 15 / Style.monitorRatio
                                    color: typesHolder.colorHandler /*typesHolder.checked ? Style.foregroundColor : rootObj.hoverColor*/
                                    visible: modelData.value ? modelData.value : 0
                                }



                                IconImage {
                                    id: closeIcon
                                    source: "qrc:/Resources/close-icon.png"
                                    Layout.preferredHeight: 18 / Style.monitorRatio
                                    Layout.preferredWidth: 18 / Style.monitorRatio
                                    Layout.leftMargin: 15 / Style.monitorRatio
                                    Layout.rightMargin: 15 / Style.monitorRatio
                                    color: typesHolder.colorHandler

                                    MouseArea {

                                        anchors.fill: parent
                                        onClicked: {
                                            rootObj.filterManager.removeFilterTag(index)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


    }
    function comparetor(txt){
        if  (txt === "!=")
            return Tag.NotEqual
        else if (txt === ">")
            return Tag.Greater
        else if (txt === "<")
            return Tag.Less
        else if (txt === ">=")
            return Tag.GreaterEqual
        else if (txt === "<=")
            return Tag.LessEqual
        else
            return Tag.Equal
    }
    PropertyAnimation {
        id: openMotion
        target: rootObj
        properties: "height"
        from: 68 / Style.monitorRatio
        to: 177 / Style.monitorRatio
        duration: 350
        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: closeMotion
        target: rootObj
        properties: "height"
        to: 68 / Style.monitorRatio
        from: 177 / Style.monitorRatio
        duration: 350
        easing.type: Easing.OutQuint
    }
}

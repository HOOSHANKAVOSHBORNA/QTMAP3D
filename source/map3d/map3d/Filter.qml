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
    readonly property color backgroundColor: "#DEE3E6"
    readonly property color foregroundColor: "#003569"
    readonly property color fg20: Qt.rgba(rootObj.foregroundColor.r,
                                          rootObj.foregroundColor.g,
                                          rootObj.foregroundColor.b, 0.2)
    readonly property color fg30: Qt.rgba(rootObj.foregroundColor.r,
                                          rootObj.foregroundColor.g,
                                          rootObj.foregroundColor.b, 0.3)
    readonly property color fg50: Qt.rgba(rootObj.foregroundColor.r,
                                          rootObj.foregroundColor.g,
                                          rootObj.foregroundColor.b, 0.5)
    readonly property color fg75: Qt.rgba(rootObj.foregroundColor.r,
                                          rootObj.foregroundColor.g,
                                          rootObj.foregroundColor.b, 0.75)
    readonly property color bg20: Qt.rgba(rootObj.backgroundColor.r,
                                          rootObj.backgroundColor.g,
                                          rootObj.backgroundColor.b, 0.2)
    readonly property color bg75: Qt.rgba(rootObj.backgroundColor.r,
                                          rootObj.backgroundColor.g,
                                          rootObj.backgroundColor.b, 0.75)

    readonly property color selectColor: "#B6C0CA"
    readonly property string fontFamily: "Roboto"
    readonly property double fontPointSize: 11 / Style.monitorRatio

    function isNumeric(s) {
        return !isNaN(s - parseFloat(s))
    }
    Rectangle{
        width: parent.width - 3/ Style.monitorRatio
        height: 20
        anchors.top: parent.top
        color: Qt.rgba(Style.backgroundColor.r,Style.backgroundColor.g,Style.backgroundColor.b,.5)
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
                        font.family: rootObj.fontFamily
                        color: rootObj.foregroundColor
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
                        font.family: rootObj.fontFamily
                        color: rootObj.foregroundColor

                    }

                    RadioButton {
                        id: orCheck
                    }

                    Label {
                        width: 36 / Style.monitorRatio
                        height: 18 / Style.monitorRatio
                        text: "Or"
                        font.pixelSize: 15 / Style.monitorRatio
                        font.family: rootObj.fontFamily
                        color: rootObj.foregroundColor

                    }
                }
            }

            RowLayout {

                Label {

                    width: 36 / Style.monitorRatio
                    height: 18 / Style.monitorRatio
                    text: "Color"
                    font.pixelSize: 15 / Style.monitorRatio
                    font.family: rootObj.fontFamily
                    color: rootObj.foregroundColor
                }
                IconImage {
                    id: colorLeftIcon
                    source: "qrc:/Resources/down.png"
                    Layout.preferredHeight: 18 / Style.monitorRatio
                    Layout.preferredWidth: 18 / Style.monitorRatio
                    rotation: 90
                    color: colorScroll.position < .01 ? rootObj.backgroundColor : "transparent"
                    visible:colorRepeater.count > 4
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                            if (colorScroll.position > 0)
                                colorScroll.position -= 0.24

                        }
                    }
                }
                Item {
                    width: 115 / Style.monitorRatio
                    height: 24 / Style.monitorRatio
                    clip: true

                    RowLayout {
                        spacing: 3
                        x: -colorScroll.position * parent.width

                        Repeater {
                            id: colorRepeater
                            model: rootObj.filterManager ? filterManager.colorFilterFields: 0
                            delegate: Rectangle {
                                required property var modelData
                                width: 24 / Style.monitorRatio
                                height: 24 / Style.monitorRatio
                                radius: height / 2
                                color: modelData
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        filterManager.addFilterTag("Color",
                                                                   modelData, Tag.Equal, andCheck.checked ? Tag.And : Tag.Or)
                                    }
                                }
                            }
                        }
                    }
                    ScrollBar {
                        id: colorScroll
                        hoverEnabled: true
                        active: hovered || pressed
                        orientation: Qt.Horizontal
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        visible: false
                    }
                }
                IconImage {
                    id: colorRightIcon
                    source: "qrc:/Resources/down.png"
                    Layout.preferredHeight: 18 / Style.monitorRatio
                    Layout.preferredWidth: 18 / Style.monitorRatio
                    rotation: -90
                    color: colorScroll.position > .95 ? rootObj.backgroundColor : "transparent"
                    visible:colorRepeater.count > 4

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (colorScroll.position < 0.9) {
                                colorScroll.position += 0.24

                            }
                        }
                    }
                }
            }

            Rectangle {
                id: stringFilter
                width: 204 / Style.monitorRatio
                height: 28 / Style.monitorRatio
                Layout.leftMargin: 20 / Style.monitorRatio
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
                        anchors.right: parent.right
                        spacing: 5
                        ComboBox {
                            id: control
                            property real txtWidth: 0
                            function filterModel(searchFilterText,searchFilterModel) {
                                var filteredModel = [];
                                for (var i = 0; i < searchFilterModel.length; ++i) {

                                    var itemName = searchFilterModel[i].toLowerCase();
                                    if (itemName.startsWith(searchFilterText)) {
                                        filteredModel.push(searchFilterModel[i]);
                                    }
                                }
                                return filteredModel;
                            }
                            model: rootObj.filterManager ? filterManager.stringFilterFields: 0

                            delegate: ItemDelegate {
                                id: itemDelegate
                                implicitWidth: control.txtWidth
                                background: Rectangle {
                                    width: control.txtWidth
                                    color: Style.backgroundColor
                                    border.width: .3
                                    border.color: "black"
                                }

                                contentItem: Text {
                                    text: control.textRole ? (Array.isArray(
                                                                  control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
                                    color: Style.foregroundColor
                                    font.family: Style.fontFamily
                                    font.pixelSize: 14 / Style.monitorRatio
                                }
                            }
                            indicator: Rectangle {}
                            contentItem: TextField {
                                id: txtContentItem1
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
                                    color: "transparent"
                                }

                                onTextChanged: {
                                    control.popup.open()
                                    var searchFilter = txtContentItem1.text.toLowerCase();
                                    control.model = control.filterModel(searchFilter,filterManager.stringFilterFields);
                                }
                            }

                            background: Rectangle {
                                color: "transparent"
                            }
                            popup: Popup {
                                id: popupCombo1
                                y: control.height - 1
                                width: 100
                                implicitHeight: contentItem.implicitHeight
                                padding: 1
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

                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight
                                    model: control.delegateModel
                                    currentIndex: control.highlightedIndex
                                    ScrollIndicator.vertical: ScrollIndicator {}
                                }

                                background: Rectangle {
                                    border.color: Style.foregroundColor
                                    radius: 2
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        txtContentItem1.text = control.textAt(
                                                    control.highlightedIndex)

                                        popupCombo1.close()
                                    }
                                }
                            }
                        }

                        Label {
                            text: ":"
                            font.pixelSize: 15 / Style.monitorRatio
                            font.family: rootObj.fontFamily
                            color: rootObj.fg30
                        }
                        TextField {
                            id: descriptionField
                            implicitWidth: 95 / Style.monitorRatio
                            placeholderText: qsTr("Description")
                            color: rootObj.foregroundColor
                            font.family: rootObj.fontFamily
                            font.pixelSize: 15 / Style.monitorRatio
                            selectedTextColor: rootObj.backgroundColor
                            selectionColor: rootObj.foregroundColor
                            placeholderTextColor: rootObj.fg30
                            background: Rectangle {
                                id: redBackG2
                                color: "transparent"
                                radius: 8
                            }

                            PropertyAnimation {
                                id: rbg2
                                target: redBackG2
                                properties: "opacity"
                                to: 0
                                from: 1
                                duration: 2500
                                easing.type: Easing.OutQuint

                            }
                            onAccepted: {
                                filterManager.addFilterTag(control.currentText,
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
                        anchors.leftMargin: 25 / Style.monitorRatio
                        anchors.top: parent.top
                        anchors.topMargin: -3
                        spacing: 5
                        ComboBox {
                            id: control3
                            property real txtWidth: 0
                            function filterModel(searchFilterText,searchFilterModel) {
                                var filteredModel = [];
                                for (var i = 0; i < searchFilterModel.length; ++i) {

                                    var itemName = searchFilterModel[i].toLowerCase();
                                    if (itemName.startsWith(searchFilterText)) {
                                        filteredModel.push(searchFilterModel[i]);
                                    }
                                }
                                return filteredModel;
                            }
                            model: rootObj.filterManager?filterManager.stringFilterFields :0

                            delegate: ItemDelegate {
                                id: itemDelegate3
                                implicitWidth: control.txtWidth
                                background: Rectangle {
                                    width: control.txtWidth
                                    color: Style.backgroundColor
                                    border.width: .3
                                    border.color: "black"
                                }

                                contentItem: Text {
                                    text: control3.textRole ? (Array.isArray(
                                                                   control3.model) ? modelData[control3.textRole] : model[control3.textRole]) : modelData
                                    color: Style.foregroundColor
                                    font.family: Style.fontFamily
                                    font.pixelSize: 14 / Style.monitorRatio
                                }
                            }
                            indicator: Rectangle {}
                            contentItem: TextField {
                                id: txtContentItem3
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
                                    color: "transparent"
                                }

                                onTextChanged: {
                                    control3.popup.open()
                                    var searchFilter = txtContentItem3.text.toLowerCase();
                                    control3.model = control3.filterModel(searchFilter,filterManager.numFilterFields);
                                }
                            }

                            background: Rectangle {
                                color: "transparent"
                            }
                            popup: Popup {
                                id: popupCombo3
                                y: control.height - 1
                                width: 100
                                implicitHeight: contentItem.implicitHeight
                                padding: 1
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

                                contentItem: ListView {
                                    clip: true
                                    implicitHeight: contentHeight
                                    model: control3.delegateModel
                                    currentIndex: control3.highlightedIndex
                                    //visible: false
                                    ScrollIndicator.vertical: ScrollIndicator {}
                                }

                                background: Rectangle {
                                    border.color: Style.foregroundColor
                                    radius: 2
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        txtContentItem3.text = control3.textAt(
                                                    control3.highlightedIndex)
                                        popupCombo3.close()
                                    }
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
                                color: rootObj.backgroundColor
                                anchors.verticalCenter: parent.verticalCenter
                                Label {
                                    id: lblComparision
                                    anchors.centerIn: parent
                                    text: "="
                                    font.pixelSize: 16 / Style.monitorRatio
                                    font.family: rootObj.fontFamily
                                    color: rootObj.foregroundColor
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            comparisonMenu.popup()
                                        }
                                    }
                                    Menu {
                                        id: comparisonMenu
                                        width: 30

                                        Repeater {
                                            id: repeaterMenu
                                            model: ["!=", "<", "<=", "=", ">=", ">"]
                                            MenuItem {
                                                text: modelData

                                                background: Rectangle {
                                                    width: 30
                                                    color: rootObj.backgroundColor
                                                    border.width: .3
                                                    border.color: "black"
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
                            color: rootObj.foregroundColor
                            font.family: rootObj.fontFamily
                            font.pixelSize: 15 / Style.monitorRatio
                            selectedTextColor: rootObj.backgroundColor
                            selectionColor: rootObj.foregroundColor
                            placeholderTextColor: rootObj.fg30
                            background: Rectangle {
                                id: redBackG3
                                color: "transparent"
                                radius: 8
                            }
                            PropertyAnimation {
                                id: rbg3
                                target: redBackG3
                                properties: "opacity"
                                to: 0
                                from: 1
                                duration: 2500
                                easing.type: Easing.OutQuint

                            }

                            onAccepted: {
                                if (rootObj.isNumeric(numbfield3.text)) {
                                    filterManager.addFilterTag(
                                                control3.currentText,
                                                parseFloat(numbfield3.text),
                                                rootObj.comparetor(lblComparision.text),
                                                andCheck.checked ? Tag.And : Tag.Or)

                                } else {
                                    redBackG3.color = "red"
                                    rbg3.running = true
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
                            property color colorHandler: modelData.isEnabled ? "#01AED6" : rootObj.foregroundColor
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
                                color: mouseArea.containsMouse ? Qt.rgba(rootObj.foregroundColor.r,
                                                                         rootObj.foregroundColor.g,
                                                                         rootObj.foregroundColor.b, 0.1): "transparent"
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
                                    color: typesHolder.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
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
                                    color: typesHolder.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
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

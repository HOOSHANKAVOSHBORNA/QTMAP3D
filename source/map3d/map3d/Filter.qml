import QtQuick.Layouts
import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Crystal 1.0
import "style"


Rectangle {
    id: rootObj
    property var filterManager

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

    color: "#DEE3E6"
    width: parent.width
    height: filterFields.height + searchRow.height + mainScroll.height + 52 / Style.monitorRatio
    radius: 15
    ColumnLayout {
        RowLayout {
            id: searchRow
            Layout.topMargin: 36 / Style.monitorRatio
            Layout.leftMargin: 15 / Style.monitorRatio
            Rectangle {
                id: searchBar
                Layout.minimumWidth: 300 / Style.monitorRatio
                width: parent.width / Style.monitorRatio
                height: 32 / Style.monitorRatio
                radius: 15
                color: rootObj.fg20
                IconImage {
                    id: searchIconImage
                    anchors.left: parent.left
                    anchors.leftMargin: 10 / Style.monitorRatio
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Resources/search-icon.png"
                    width: 24 / Style.monitorRatio
                    height: 24 / Style.monitorRatio
                    color: rootObj.fg75
                }
                visible: false

                TextField {
                    id: searchTextField
                    anchors.fill: parent
                    anchors.leftMargin: searchIconImage.width + searchIconImage.x
                    implicitWidth: parent.width / 3
                    placeholderText: qsTr("Search ...")
                    color: rootObj.foregroundColor
                    font.family: rootObj.fontFamily
                    font.pointSize: 13 / Style.monitorRatio
                    selectedTextColor: rootObj.backgroundColor
                    selectionColor: rootObj.foregroundColor
                    placeholderTextColor: rootObj.fg50
                    wrapMode: Text.WrapAnywhere
                    background: Rectangle {
                        color: "transparent"
                        radius: 15
                    }
                }
            }
        }

        Flow {
            id: filterFields
            width: parent.width
            spacing: 15
            Layout.topMargin: 10 / Style.monitorRatio
            Layout.leftMargin: 15 / Style.monitorRatio
            RowLayout {
                Label {
                    width: 36 / Style.monitorRatio
                    height: 18 / Style.monitorRatio
                    text: "Logical Operator :"
                    //                Layout.leftMargin: 15 / Style.monitorRatio
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
                    color: colorLeftIcon.position < .01 ? rootObj.backgroundColor : "transparent"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (colorLeftIcon.position > 0)
                                colorLeftIcon.position -= 0.24

                        }
                    }
                }
                Item {
                    width: 115 / Style.monitorRatio
                    height: 24 / Style.monitorRatio
                    clip: true

                    RowLayout {
                        spacing: 3
                        x: -colorLeftIcon.position * parent.width

                        Repeater {
                            id: colorRepeater
                            model: filterManager.colorFilterFields
                            delegate: Rectangle {
                                required property var modelData
                                width: 24 / Style.monitorRatio
                                height: 24 / Style.monitorRatio
                                radius: height / 2
                                color: modelData
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        filterManager.addFilterTag("color",
                                                                   modelData, TagComparision.Equal, andCheck.checked ? TagLogicalOperator.And:TagLogicalOperator.Or)
                                        tagsModel.append({
                                                             "name": "color",
                                                             "color": modelData,
                                                             "value1": "",
                                                             "value2": "",
                                                             "value3": "",
                                                             "value4": "",
                                                             "compVal": "",
                                                             "logical": andCheck.checked ? true : false
                                                         })
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
                    color: colorLeftIcon.position > .95 ? rootObj.backgroundColor : "transparent"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (colorLeftIcon.position < 0.9) {
                                colorLeftIcon.position += 0.24

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
                    color: rootObj.backgroundColor
                    radius: 8
                    RowLayout {
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        spacing: 5
                        ComboBox {
                            id: control
                            property real txtWidth: 0
                            Layout.minimumWidth: 50
                            Layout.maximumWidth: 50
                            model: filterManager.stringFilterFields


                            delegate: ItemDelegate {
                                id: itemDelegate
                                implicitWidth: control.txtWidth
                                background: Rectangle {
                                    width: control.txtWidth
                                    color: rootObj.backgroundColor
                                    border.width: .3
                                    border.color: "black"
                                }

                                contentItem: Text {
                                    text: control.textRole ? (Array.isArray(
                                                                  control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
                                    color: rootObj.foregroundColor
                                    font.family: rootObj.fontFamily
                                    font.pixelSize: 14 / Style.monitorRatio
                                }
                            }
                            indicator: Rectangle {}
                            contentItem: Text {
                                id:txtContentItem1
                                text: control.displayText
                                font.family: rootObj.fontFamily
                                font.pixelSize: 14 / Style.monitorRatio
                                color: rootObj.fg30
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
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
                                    border.color: rootObj.foregroundColor
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
                            font.pointSize: 13 / Style.monitorRatio
                            selectedTextColor: rootObj.backgroundColor
                            selectionColor: rootObj.foregroundColor
                            placeholderTextColor: rootObj.fg30
                            background: Rectangle {
                                color: "transparent"
                                radius: 15
                            }
                            onAccepted: {
                                filterManager.addFilterTag(control.currentText, descriptionField.text, TagComparision.Equal, andCheck.checked ? TagLogicalOperator.And : TagLogicalOperator.Or)
                                tagsModel.append({
                                                     "name": control.currentText,
                                                     "value1": descriptionField.text,
                                                     "value2": "",
                                                     "value3": "",
                                                     "value4": "",
                                                     "compVal": "",
                                                     "logical": andCheck.checked ? true : false
                                                 })
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
                    width: filterCompare.width - 3
                    height: 28 / Style.monitorRatio - 3
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    color: rootObj.backgroundColor
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
                            Layout.minimumWidth: 50
                            Layout.maximumWidth: 50
                            model: filterManager.numFilterFields ///* compareModel*/ ["test","test2"]

                            delegate: ItemDelegate {
                                implicitWidth: control3.txtWidth
                                background: Rectangle {
                                    width: control3.txtWidth
                                    color: rootObj.backgroundColor
                                    border.width: .3
                                    border.color: "black"
                                }

                                contentItem: Text {
                                    text: control3.textRole ? (Array.isArray(
                                                                   control3.model) ? modelData[control3.textRole] : model[control3.textRole]) : modelData
                                    color: rootObj.foregroundColor
                                    font.family: rootObj.fontFamily
                                    font.pixelSize: 14 / Style.monitorRatio
                                }
                            }
                            indicator: Rectangle {}
                            contentItem: Text {
                                id:txtContentItem3
                                text: control3.displayText
                                font.family: rootObj.fontFamily
                                font.pixelSize: 14 / Style.monitorRatio
                                color: rootObj.fg30
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }
                            background: Rectangle {
                                color: "transparent"
                            }
                            popup: Popup {
                                id: popupCombo3
                                y: control3.height - 1
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
                                    border.color: rootObj.foregroundColor
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
                            //                            anchors.centerIn: filterString11
                            //Layout.verticalCenter: filterString11.verticalCenter
                            Rectangle {
                                id: comparison
                                anchors.fill: parent
                                radius: width / 2
                                color: rootObj.backgroundColor
                                Label {
                                    id: lblComparision
                                    anchors.centerIn: parent
                                    text: "="
                                    font.pixelSize: 20 / Style.monitorRatio
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
                                            model: ["<", "<=", "=", ">=", ">"]
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
                                                    //width: 50
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
                                duration: 3500
                                easing.type: Easing.OutQuint

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
                            onAccepted: {


                                if (rootObj.isNumeric(numbfield3.text)) {
                                    filterManager.addFilterTag(
                                                control3.currentText,
                                                parseFloat(numbfield3.text),numbfield3.comparetor(lblComparision.text),
                                                andCheck.checked ? Tag.And : Tag.Or)
                                    tagsModel.append({
                                                         "name": control3.currentText,
                                                         "value4": numbfield3.text,
                                                         "compVal": lblComparision.text,
                                                         "logical": andCheck.checked ? true : false
                                                     })

                                    tagsModel.sync()

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
            // anchors.top: filterFields.bottom
            height: 60 / Style.monitorRatio
            // anchors.topMargin: 18 / Style.monitorRatio
            RowLayout {
                width: rootObj.width
                visible: true
                clip: true

                Flow {
                    spacing: 5 / Style.monitorRatio
                    height: 40 / Style.monitorRatio
                    Layout.bottomMargin: 20
                    Layout.fillWidth: true
                    Layout.leftMargin: 5

                    Repeater {

                        model: ListModel {
                            id: tagsModel
                        }

                        delegate: Rectangle {
                            property bool visiblitySet: true
                            property bool checked: true
                            property bool selected: false
                            id: typesHolder
                            implicitHeight: 26 / Style.monitorRatio
                            implicitWidth: showDetails.implicitWidth + (index ? andOrLabel.implicitWidth + 5 : 0)
                            color: "transparent"
                            radius: 20
                            Rectangle{
                                id:borderRect
                                implicitHeight: 26 / Style.monitorRatio
                                implicitWidth: showDetails.implicitWidth
                                anchors.right: parent.right
                                color: checked ? "transparent" : Qt.rgba(
                                                     rootObj.foregroundColor.r,
                                                     rootObj.foregroundColor.g,
                                                     rootObj.foregroundColor.b, 0.1)
                                border {

                                    color: "#01AED6" /*rootObj.disableColor*/
                                    width: 1
                                }
                                radius: 20
                            }
                            Label {
                                id:andOrLabel
                                anchors.right: borderRect.left
                                anchors.rightMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                                text: model.logical ? "&&" : "||"
                                font.pixelSize: 17 / Style.monitorRatio
                                font.family: "Roboto"
                                color: showDetails.colorHandler
                                visible: index ? true : false
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: {
                                    print(index)
                                    if (!typesHolder.selected == true) {
                                        borderRect.border.color = rootObj.foregroundColor
                                        showDetails.colorHandler = rootObj.foregroundColor

                                    } else {
                                        borderRect.border.color = rootObj.foregroundColor
                                        showDetails.colorHandler = rootObj.foregroundColor
                                    }
                                }
                                onExited: {
                                    if (typesHolder.selected == true) {
                                        borderRect.border.color = rootObj.fg30
                                        showDetails.colorHandler = rootObj.fg30
                                    } else {
                                        borderRect.border.color = "#01AED6"
                                        showDetails.colorHandler = "#01AED6"
                                    }
                                }

                                onClicked: {

                                    if (!typesHolder.selected) {
                                        typesHolder.selected = true
                                        typesHolder.border.color = "#01AED6"
                                        showDetails.colorHandler = "#01AED6"
                                    } else {
                                        typesHolder.selected = false
                                        typesHolder.border.color = rootObj.fg30
                                        showDetails.colorHandler = rootObj.fg30
                                    }
                                }
                            }

                            RowLayout {
                                id: showDetails
                                anchors.right:  parent.right
                                property color colorHandler: "#01AED6"
                                Text {
                                    Layout.alignment: Qt.AlignLeft
                                    text: model.name ? model.name : 0
                                    font.family: "Roboto"
                                    font.pixelSize: 17 / Style.monitorRatio
                                    color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                    Layout.leftMargin: 15 / Style.monitorRatio
                                    Layout.topMargin: 2 / Style.monitorRatio
                                    Layout.bottomMargin: 2 / Style.monitorRatio
                                }
                                Label {
                                    text: ":"
                                    font.pixelSize: 17 / Style.monitorRatio
                                    font.family: "Roboto"
                                    color: showDetails.colorHandler
                                    visible: model.compVal ? false : true
                                }

                                Text {
                                    id: itemValue
                                    Layout.alignment: Qt.AlignLeft
                                    text: model.value1 ? model.value1 : 0
                                    font.family: "Roboto"
                                    font.pixelSize: 17 / Style.monitorRatio
                                    color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                    visible: model.value2 || model.value4
                                             || model.color ? false : true
                                }
                                Text {
                                    id: itemValue2
                                    Layout.alignment: Qt.AlignLeft
                                    text: model.value2 ? model.value2 : 0
                                    font.family: "Roboto"
                                    font.pixelSize: 17 / Style.monitorRatio
                                    color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                    visible: model.value3
                                             && model.value2 ? true : false
                                }
                                Label {
                                    text: "To"
                                    font.pixelSize: 17 / Style.monitorRatio
                                    font.family: "Roboto"
                                    color: showDetails.colorHandler
                                    visible: model.value2 ? true : false
                                }
                                Text {
                                    id: itemValue3
                                    Layout.alignment: Qt.AlignLeft
                                    text: model.value3 ? model.value3 : 0
                                    font.family: "Roboto"
                                    font.pixelSize: 17 / Style.monitorRatio
                                    color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                    visible: model.value3
                                             && model.value2 ? true : false
                                }
                                Text {
                                    id: comparisonIcon
                                    Layout.alignment: Qt.AlignLeft
                                    text: model.compVal ? model.compVal : 0
                                    font.family: "Roboto"
                                    font.pixelSize: 17 / Style.monitorRatio
                                    color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                    visible: model.compVal ? true : false
                                }
                                Text {
                                    id: itemValue4
                                    Layout.alignment: Qt.AlignLeft
                                    text: model.value4 ? model.value4 : 0
                                    font.family: "Roboto"
                                    font.pixelSize: 17 / Style.monitorRatio
                                    color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                    visible: model.value4 ? true : false
                                }
                                Text {
                                    id: itemColor
                                    Layout.alignment: Qt.AlignLeft
                                    text: model.color ? model.color : 0
                                    font.family: "Roboto"
                                    font.pixelSize: 17 / Style.monitorRatio
                                    color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                    visible: model.color ? true : false
                                }
                                IconImage {
                                    id: closeIcon
                                    source: "qrc:/Resources/close.png"
                                    Layout.preferredHeight: 20 / Style.monitorRatio
                                    Layout.preferredWidth: 20 / Style.monitorRatio
                                    color: showDetails.colorHandler
                                    Layout.leftMargin: 15 / Style.monitorRatio
                                    Layout.rightMargin: 15 / Style.monitorRatio
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: tagsModel.remove(index)

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
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

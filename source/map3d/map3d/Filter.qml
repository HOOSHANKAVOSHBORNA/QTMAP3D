import QtQuick.Layouts
import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Crystal 1.0

Rectangle {
    id: rootObj    
    property var filterManager
    //    required property var colorModel
    //    required property var columnModel
    //    required property var rangeModel
    //    required property var compareModel
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

    readonly property color hoverColor: "#01AED6"
    readonly property color selectColor: "#B6C0CA"
    readonly property real monitorRatio: /*Style.monitorRatio*/ 1.3
    readonly property string fontFamily: "Roboto"
    readonly property double fontPointSize: 11 / monitorRatio

    function isNumeric(s) {
        return !isNaN(s - parseFloat(s))
    }

    color: "#DEE3E6"
    width: parent.width
    height: mainRow.height + searchRow.height + mainScroll.height + 52 / /*Style.monitorRatio*/ 1.3
    radius: 15
    RowLayout {
        id: searchRow
        anchors.top: parent.top
        anchors.topMargin: 36 / /*Style.monitorRatio*/ 1.3
        anchors.left: parent.left
        anchors.leftMargin: 15 / /*Style.monitorRatio*/ 1.3
        Rectangle {
            id: searchBar
            Layout.minimumWidth: 300 / /*Style.monitorRatio*/ 1.3
            width: parent.width / /*Style.monitorRatio*/ 1.3
            height: 32 / /*Style.monitorRatio*/ 1.3
            radius: 15
            color: rootObj.fg20
            IconImage {
                id: searchIconImage
                anchors.left: parent.left
                anchors.leftMargin: 10 / /*Style.monitorRatio*/ 1.3
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/Resources/search-icon.png"
                width: 24 / /*Style.monitorRatio*/ 1.3
                height: 24 / /*Style.monitorRatio*/ 1.3
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
                font.pointSize: 13 / /*Style.monitorRatio*/ 1.3
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
        id: mainRow
        width: parent.width
        spacing: 15
        anchors.top: searchRow.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 15 / rootObj.monitorRatio
        RowLayout {

            Label {
                width: 36 / rootObj.monitorRatio
                height: 18 / rootObj.monitorRatio
                text: "Color"
//                Layout.leftMargin: 15 / rootObj.monitorRatio
                font.pixelSize: 15 / rootObj.monitorRatio
                font.family: rootObj.fontFamily
                color: rootObj.foregroundColor
            }
            IconImage {
                id: leftIcon
                source: "qrc:/Resources/down.png"
                Layout.preferredHeight: 18 / rootObj.monitorRatio
                Layout.preferredWidth: 18 / rootObj.monitorRatio
                rotation: 90
                color: hbar.position < .01 ? rootObj.backgroundColor : "transparent"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (hbar.position > 0)
                            hbar.position -= 0.24
                    }
                }
            }
            Item {
                width: 115 / /*Style.monitorRatio*/ 1.3
                height: 24 / /*Style.monitorRatio*/ 1.3
                clip: true

                RowLayout {
                    spacing: 3
                    x: -hbar.position * parent.width

                    Repeater {
                        id: colorRepeater
                        model: filterManager.colorFilterFields ///*colorModel*/ ["#EF2929", "#FCAF3E", "#FCE94F", "#8AE234", "#FCAF3E", "#FCE94F", "#8AE234", "#EF2929", "#FCAF3E", "#FCE94F", "#8AE234", "#729FCF", "#AD7FA8", "#E9B96E", "#8AE234", "#729FCF", "#AD7FA8", "#E9B96E"]
                        delegate: Rectangle {
                            required property var modelData
                            width: 24 / rootObj.monitorRatio
                            height: 24 / rootObj.monitorRatio
                            radius: height / 2
                            color: modelData
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    filterManager.addFilterTag("color",
                                                               modelData, TagComparision.Equal, TagLogicalOperator.And)
                                    tagsModel.append({
                                                         "name": "color",
                                                         "color": modelData,
                                                         "value1": "",
                                                         "value2": "",
                                                         "value3": "",
                                                         "value4": "",
                                                         "compVal": ""
                                                     })
                                }
                            }
                        }
                    }
                }
                ScrollBar {
                    id: hbar
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
                id: rightIcon
                source: "qrc:/Resources/down.png"
                Layout.preferredHeight: 18 / rootObj.monitorRatio
                Layout.preferredWidth: 18 / rootObj.monitorRatio
                rotation: -90
                color: hbar.position > .95 ? rootObj.backgroundColor : "transparent"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (hbar.position < 0.9) {
                            hbar.position += 0.24
                        }
                    }
                }
            }
        }

        Rectangle {
            id: filterColumn
            width: 204 / rootObj.monitorRatio
            height: 28 / rootObj.monitorRatio
            Layout.leftMargin: 20 / rootObj.monitorRatio
            radius: 15
            property color s: "black"
            color: Qt.rgba(s.r, s.g, s.b, .04)
            Rectangle {
                width: filterColumn.width - 3
                height: 28 / rootObj.monitorRatio - 3
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
                        model: filterManager.stringFilterFields // /* columnModel*/ ["test","test2"]


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
                                //Layout.leftMargin: 20
                                text: control.textRole ? (Array.isArray(
                                                              control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
                                color: rootObj.foregroundColor
                                font.family: rootObj.fontFamily
                                font.pixelSize: 14 / rootObj.monitorRatio
                            }

                            //                                highlighted: control.highlightedIndex === index
                        }
                        indicator: Rectangle {}
                        contentItem: Text {
                            id:txtContentItem1
                            text: control.displayText
                            font.family: rootObj.fontFamily
                            font.pixelSize: 14 / rootObj.monitorRatio
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
                                model: control.delegateModel //comboFilter1.popup.visible ? comboFilter1.delegateModel : null
                                currentIndex: control.highlightedIndex
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
                                    txtContentItem1.text = control.textAt(
                                                control.highlightedIndex)
                                    popupCombo1.close()
                                }
                            }
                        }
                    }
                    Label {
                        text: ":"
                        font.pixelSize: 15 / rootObj.monitorRatio
                        font.family: rootObj.fontFamily
                        color: rootObj.fg30
                    }
                    TextField {
                        id: descriptionField
                        implicitWidth: 95 / rootObj.monitorRatio
                        placeholderText: qsTr("Description")
                        color: rootObj.foregroundColor
                        font.family: rootObj.fontFamily
                        font.pointSize: 13 / rootObj.monitorRatio
                        selectedTextColor: rootObj.backgroundColor
                        selectionColor: rootObj.foregroundColor
                        placeholderTextColor: rootObj.fg30
                        background: Rectangle {
                            color: "transparent"
                            radius: 15
                        }
                        onAccepted: {
                            filterManager.addFilterTag(control.currentText, descriptionField.text, TagComparision.Equal, TagLogicalOperator.And)
                            tagsModel.append({
                                                 "name": control.currentText,
                                                 "value1": descriptionField.text,
                                                 "value2": "",
                                                 "value3": "",
                                                 "value4": "",
                                                 "compVal": ""
                                             })
                        }
                    }
                }
            }
        }

        Rectangle {
            id: filterCompare
            width: 204 / rootObj.monitorRatio
            height: 28 / rootObj.monitorRatio
            Layout.leftMargin: 15 / rootObj.monitorRatio
            radius: 15
            property color s: "black"
            color: Qt.rgba(s.r, s.g, s.b, .04)
            Rectangle {
                width: filterCompare.width - 3
                height: 28 / rootObj.monitorRatio - 3
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                color: rootObj.backgroundColor
                radius: 8
                RowLayout {
                    anchors.left: parent.left
                    anchors.leftMargin: 25 / /*Style.monitorRatio*/ 1.3
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
                                font.pixelSize: 14 / rootObj.monitorRatio
                            }
                        }
                        indicator: Rectangle {}
                        contentItem: Text {
                            id:txtContentItem3
                            text: control3.displayText
                            font.family: rootObj.fontFamily
                            font.pixelSize: 14 / rootObj.monitorRatio
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
                                model: control3.delegateModel //comboFilter1.popup.visible ? comboFilter1.delegateModel : null
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
                        width: 26 / rootObj.monitorRatio
                        height: 26 / rootObj.monitorRatio
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
                                font.pixelSize: 20 / rootObj.monitorRatio
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
                                                font.pixelSize: 14 / rootObj.monitorRatio
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

                    //---------------------------------------------------
//                    Item {
//                        width: 26 / rootObj.monitorRatio
//                        height: 26 / rootObj.monitorRatio
//                        Rectangle {
//                            id: comparisonHold
//                            anchors.fill: parent
//                            radius: width / 2
//                            color: rootObj.backgroundColor

//                            ComboBox {
//                                id: comparison
//                                anchors.centerIn: parent
//                                anchors.fill: parent
//                                model: /* myProxyModel.comboItem*/ ["=", "<=", ">=", "<", ">"]

//                                function findLongestString(stringList) {
//                                    var longest = ""

//                                    for (var i = 0; i < stringList.length; i++) {
//                                        if (stringList[i].length > longest.length) {
//                                            longest = stringList[i]
//                                        }
//                                    }
//                                    return longest
//                                }
//                                delegate: ItemDelegate {
//                                    implicitWidth: 26 / /*Style.monitorRatio*/ 1.3
//                                    background: Rectangle {
//                                        width: 26 / /*Style.monitorRatio*/ 1.3
//                                        color: rootObj.backgroundColor
//                                        border.width: .3
//                                        border.color: "black"
//                                    }

//                                    contentItem: Text {
//                                        text: comparison.textRole ? (Array.isArray(
//                                                                         comparison.model) ? modelData[comparison.textRole] : model[comparison.textRole]) : modelData
//                                        color: rootObj.foregroundColor
//                                        font.family: rootObj.fontFamily
//                                        font.pixelSize: 14 / rootObj.monitorRatio
//                                    }
//                                }
//                                indicator: Rectangle {}
//                                contentItem: Text {
//                                    text: comparison.displayText
//                                    font.family: rootObj.fontFamily
//                                    font.pixelSize: 14 / rootObj.monitorRatio
//                                    color: rootObj.fg30
//                                    verticalAlignment: Text.AlignVCenter
//                                    horizontalAlignment: Text.AlignHCenter
//                                    elide: Text.ElideRight
//                                }
//                                background: Rectangle {
//                                    color: "transparent"
//                                }
//                            }
//                        }
//                        MultiEffect {
//                            source: comparisonHold
//                            enabled: true
//                            anchors.fill: comparisonHold
//                            shadowColor: "black"
//                            shadowEnabled: true
//                            shadowBlur: 1
//                            shadowHorizontalOffset: 0.5
//                            shadowVerticalOffset: 0
//                            shadowOpacity: 1
//                            shadowScale: 0.6
//                        }
//                    }
                    TextField {
                        id: numbfield3
                        implicitWidth: 60 / rootObj.monitorRatio
                        Layout.fillHeight: true
                        placeholderText: qsTr("Numb")
                        color: rootObj.foregroundColor
                        font.family: rootObj.fontFamily
                        font.pixelSize: 15 / rootObj.monitorRatio
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
                        onAccepted: {
                            if (rootObj.isNumeric(numbfield3.text)) {
                                filterManager.addFilterTag(
                                            control3.currentText,
                                            parseFloat(numbfield3.text),TagComparision.Equal, TagLogicalOperator.And,
                                            comparison.currentText)
                                tagsModel.append({
                                                     "name": control3.currentText,
                                                     "value4": numbfield3.text,
                                                     "compVal": comparison.currentText
                                                 })
                            } else {
                                redBackG3.color = "red"
                                rbg3.running = true
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            id: filterRange
            width: 268 / rootObj.monitorRatio
            height: 28 / rootObj.monitorRatio
            Layout.leftMargin: 15 / rootObj.monitorRatio
            radius: 15
            property color s: "black"
            color: Qt.rgba(s.r, s.g, s.b, .04)
            Rectangle {
                width: filterRange.width - 3
                height: 28 / rootObj.monitorRatio - 3
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                color: rootObj.backgroundColor
                radius: 8
                RowLayout {
                    anchors.left: parent.left
                    anchors.leftMargin: 25 / /*Style.monitorRatio*/ 1.3
                    anchors.centerIn: parent
                    spacing: 5
                    ComboBox {
                        id: control2
                        property real txtWidth: 0
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: 50
                        model: filterManager.numFilterFields /* rangeModel*/
                        /*["test","test2"]*/

                        delegate: ItemDelegate {
                            implicitWidth: control2.txtWidth
                            background: Rectangle {
                                width: control2.txtWidth
                                color: rootObj.backgroundColor
                                border.width: .3
                                border.color: "black"
                            }

                            contentItem: Text {
                                text: control2.textRole ? (Array.isArray(
                                                               control2.model) ? modelData[control2.textRole] : model[control2.textRole]) : modelData
                                color: rootObj.foregroundColor
                                font.family: rootObj.fontFamily
                                font.pixelSize: 14 / rootObj.monitorRatio
                            }
                        }
                        indicator: Rectangle {}
                        contentItem: Text {
                            id:txtContentItem2
                            text: control2.displayText
                            font.family: rootObj.fontFamily
                            font.pixelSize: 14 / rootObj.monitorRatio
                            color: rootObj.fg30
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                        background: Rectangle {
                            color: "transparent"
                        }
                        popup: Popup {
                            id: popupCombo2
                            y: control2.height - 1
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
                                model: control2.delegateModel //comboFilter1.popup.visible ? comboFilter1.delegateModel : null
                                currentIndex: control2.highlightedIndex
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
                                    txtContentItem2.text = control2.textAt(
                                                control2.highlightedIndex)
                                    popupCombo2.close()
                                }
                            }
                        }
                    }
                    Label {
                        text: ":"
                        font.pixelSize: 15 / rootObj.monitorRatio
                        font.family: rootObj.fontFamily
                        color: rootObj.fg30
                    }
                    TextField {
                        id: numField
                        implicitWidth: 60 / rootObj.monitorRatio
                        Layout.fillHeight: true
                        placeholderText: qsTr("Numb")
                        color: rootObj.foregroundColor
                        font.family: rootObj.fontFamily
                        font.pixelSize: 15 / rootObj.monitorRatio
                        selectedTextColor: rootObj.backgroundColor
                        selectionColor: rootObj.foregroundColor
                        placeholderTextColor: rootObj.fg30
                        background: Rectangle {
                            id: redBackG1
                            color: "transparent"
                            radius: 8
                        }
                        PropertyAnimation {
                            id: rbg1
                            target: redBackG1
                            properties: "opacity"
                            to: 0
                            from: 1
                            duration: 3500
                            easing.type: Easing.OutQuint
                        }
                        onAccepted: {
                            if (numField2.text !== ""
                                    && numField2.text !== "ERROR"
                                    && numField.text !== ""
                                    && rootObj.isNumeric(numField.text)) {

                                filterManager.addFilterTag(
                                            control2.currentText,
                                            parseFloat(numField.text),
                                            parseFloat(numField2.text), "<=>")
                                tagsModel.append({
                                                     "name": control2.currentText,
                                                     "value2": numField.text,
                                                     "value3": numField2.text
                                                 })
                            } else {
                                redBackG1.color = "red"
                                rbg1.running = true
                            }
                        }
                    }
                    Label {

                        text: "To"
                        font.pixelSize: 17 / rootObj.monitorRatio
                        font.family: rootObj.fontFamily
                        color: rootObj.foregroundColor
                    }
                    TextField {
                        id: numField2
                        implicitWidth: 60 / rootObj.monitorRatio
                        Layout.fillHeight: true
                        placeholderText: qsTr("Numb")
                        color: rootObj.foregroundColor
                        font.family: rootObj.fontFamily
                        font.pixelSize: 15 / rootObj.monitorRatio
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
                            duration: 3500
                            easing.type: Easing.OutQuint
                        }

                        onAccepted: {
                            if ((numField.text !== ""
                                 && numField.text !== "ERROR")
                                    && numField2.text !== ""
                                    && rootObj.isNumeric(numField2.text)) {
                                filterManager.addFilterTag(
                                            control2.currentText,
                                            parseFloat(numField.text),
                                            parseFloat(numField2.text), "<=>")

                                tagsModel.append({
                                                     "name": control2.currentText,
                                                     "value2": numField.text,
                                                     "value3": numField2.text
                                                 })
                            } else {
                                redBackG2.color = "red"
                                rbg2.running = true
                            }
                        }
                    }
                }
            }
        }
    }
    ScrollView {
        id: mainScroll
        anchors.top: mainRow.bottom
        height: 60 / /*Style.monitorRatio*/ 1.3
        anchors.topMargin: 18 / /*Style.monitorRatio*/ 1.3
        RowLayout {
            width: rootObj.width
            id: flowRow
            visible: true
            clip: true

            Flow {
                id: mytypes
                spacing: 5 / rootObj.monitorRatio
                height: 40 / /*Style.monitorRatio*/ 1.3
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
                        implicitHeight: 26 / /*Style.monitorRatio*/ 1.3
                        implicitWidth: showDetails.implicitWidth
                        color: checked ? "transparent" : Qt.rgba(
                                             rootObj.foregroundColor.r,
                                             rootObj.foregroundColor.g,
                                             rootObj.foregroundColor.b, 0.1)
                        radius: 20
                        border {

                            color: "#01AED6" /*rootObj.disableColor*/
                            width: 1
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: {
                                if (!typesHolder.selected == true) {
                                    typesHolder.border.color = rootObj.foregroundColor
                                    showDetails.colorHandler = rootObj.foregroundColor
                                } else {
                                    typesHolder.border.color = rootObj.foregroundColor
                                    showDetails.colorHandler = rootObj.foregroundColor
                                }
                            }
                            onExited: {
                                if (typesHolder.selected == true) {
                                    typesHolder.border.color = rootObj.fg30
                                    showDetails.colorHandler = rootObj.fg30
                                } else {
                                    typesHolder.border.color = "#01AED6"
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
                            anchors.fill: parent
                            property color colorHandler: "#01AED6"
                            Text {
                                Layout.alignment: Qt.AlignLeft
                                text: model.name ? model.name : 0
                                font.family: "Roboto"
                                font.pixelSize: 17 / /*Style.monitorRatio*/ 1.3
                                color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                Layout.leftMargin: 15 / /*Style.monitorRatio*/ 1.3
                                Layout.topMargin: 2 / /*Style.monitorRatio*/ 1.3
                                Layout.bottomMargin: 2 / /*Style.monitorRatio*/ 1.3
                            }
                            Label {
                                text: ":"
                                font.pixelSize: 17 / /*Style.monitorRatio*/ 1.3
                                font.family: "Roboto"
                                color: showDetails.colorHandler
                                visible: model.compVal ? false : true
                            }

                            Text {
                                id: itemValue
                                Layout.alignment: Qt.AlignLeft
                                text: model.value1 ? model.value1 : 0
                                font.family: "Roboto"
                                font.pixelSize: 17 / /*Style.monitorRatio*/ 1.3
                                color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                visible: model.value2 || model.value4
                                         || model.color ? false : true
                            }
                            Text {
                                id: itemValue2
                                Layout.alignment: Qt.AlignLeft
                                text: model.value2 ? model.value2 : 0
                                font.family: "Roboto"
                                font.pixelSize: 17 / /*Style.monitorRatio*/ 1.3
                                color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                visible: model.value3
                                         && model.value2 ? true : false
                            }
                            Label {
                                text: "To"
                                font.pixelSize: 17 / /*Style.monitorRatio*/ 1.3
                                font.family: "Roboto"
                                color: showDetails.colorHandler
                                visible: model.value2 ? true : false
                            }
                            Text {
                                id: itemValue3
                                Layout.alignment: Qt.AlignLeft
                                text: model.value3 ? model.value3 : 0
                                font.family: "Roboto"
                                font.pixelSize: 17 / /*Style.monitorRatio*/ 1.3
                                color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                visible: model.value3
                                         && model.value2 ? true : false
                            }
                            Text {
                                id: comparisonIcon
                                Layout.alignment: Qt.AlignLeft
                                text: model.compVal ? model.compVal : 0
                                font.family: "Roboto"
                                font.pixelSize: 17 / /*Style.monitorRatio*/ 1.3
                                color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                visible: model.compVal ? true : false
                            }
                            Text {
                                id: itemValue4
                                Layout.alignment: Qt.AlignLeft
                                text: model.value4 ? model.value4 : 0
                                font.family: "Roboto"
                                font.pixelSize: 17 / /*Style.monitorRatio*/ 1.3
                                color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                visible: model.value4 ? true : false
                            }
                            Text {
                                id: itemColor
                                Layout.alignment: Qt.AlignLeft
                                text: model.color ? model.color : 0
                                font.family: "Roboto"
                                font.pixelSize: 17 / /*Style.monitorRatio*/ 1.3
                                color: showDetails.colorHandler /*typesHolder.checked ? rootObj.foregroundColor : rootObj.hoverColor*/
                                visible: model.color ? true : false
                            }
                            IconImage {
                                id: closeIcon
                                source: "qrc:/Resources/close.png"
                                Layout.preferredHeight: 20 / rootObj.monitorRatio
                                Layout.preferredWidth: 20 / rootObj.monitorRatio
                                color: showDetails.colorHandler
                                Layout.leftMargin: 15 / /*Style.monitorRatio*/ 1.3
                                Layout.rightMargin: 15 / /*Style.monitorRatio*/ 1.3
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

    PropertyAnimation {
        id: openMotion
        target: rootObj
        properties: "height"
        from: 68 / rootObj.monitorRatio
        to: 177 / rootObj.monitorRatio
        duration: 350
        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: closeMotion
        target: rootObj
        properties: "height"
        to: 68 / rootObj.monitorRatio
        from: 177 / rootObj.monitorRatio
        duration: 350
        easing.type: Easing.OutQuint
    }
}

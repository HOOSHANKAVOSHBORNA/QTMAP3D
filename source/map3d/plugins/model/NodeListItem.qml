import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import Qt.labs.qmlmodels

import "Components"
import "style"

Item {
    id: rootItem

    readonly property int widthStyle: 950
    readonly property int heightStyle: 68
    readonly property color disableColor: Qt.rgba(Style.foregroundColor.r,
                                                  Style.foregroundColor.g,
                                                  Style.foregroundColor.b, 0.5)
    readonly property color fg20: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.2)
    readonly property color fg30: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.3)
    readonly property color fg50: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.5)
    readonly property color fg75: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.75)
    readonly property color bg20: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.2)
    readonly property color bg75: Qt.rgba(Style.backgroundColor.r,
                                          Style.backgroundColor.g,
                                          Style.backgroundColor.b, 0.75)

    function isNumeric(s) {
        return !isNaN(s - parseFloat(s))
    }

    //    DebugInAnchor {
    //        text: 'I am added bro'
    //    }

    // TODO: remove this
    anchors.fill: parent

    property var tableModel

    //property var attackModel: undefined
    Component.onCompleted: {

        //tableModel.filterStringColumn("")
    }

    //    Rectangle {
    //        id: rectMainSearch

    //        color: Style.backgroundColor
    //        //border.color: "black"
    //        anchors.top: rootItem.bottom
    //        anchors.topMargin: 20
    //        width: parent.width
    //        height: parent.heightStyle
    //        anchors.left: parent.left
    //        anchors.leftMargin: 20
    //        anchors.right: parent.right
    //        anchors.rightMargin: 20
    //        radius: 15
    //        Rectangle {
    //            id: dropShadowRect
    //            property real offset: Math.min(parent.width * 0.03,
    //                                           parent.height * 0.03)
    //            color: "black"
    //            width: parent.width
    //            height: parent.height
    //            z: -1
    //            opacity: 0.06
    //            radius: rectMainSearch.radius + 2
    //            anchors.left: parent.left
    //            anchors.leftMargin: -offset
    //            anchors.top: parent.top
    //            anchors.topMargin: offset
    //        }

    //        Rectangle {
    //            id: searchRect
    //            //color: Style.backgroundColor
    //            //Layout.fillWidth: true
    //            //Layout.fillHeight: true
    //            //Layout.bottomMargin: 1
    //            anchors.top: parent.top
    //            anchors.topMargin: 20 / Style.monitorRatio
    //            anchors.left: parent.left
    //            anchors.leftMargin: 15 / Style.monitorRatio
    //            width: parent.width / 3
    //            height: 28 / Style.monitorRatio
    //            radius: 15
    //            color: rootItem.fg20

    //            IconImage {
    //                id: searchIcon
    //                anchors.left: parent.left
    //                anchors.leftMargin: 10 / Style.monitorRatio
    //                anchors.verticalCenter: parent.verticalCenter
    //                source: "qrc:/Resources/search-icon.jpg"
    //                width: 24 / Style.monitorRatio
    //                height: 24 / Style.monitorRatio
    //                color: rootItem.fg75
    //            }

    //            TextField {
    //                id: txtSearch
    //                anchors.fill: parent
    //                anchors.leftMargin: searchIcon.width + searchIcon.x
    //                implicitWidth: parent.width / 3
    //                placeholderText: qsTr("Search ...")
    //                color: Style.foregroundColor
    //                font.family: Style.fontFamily
    //                font.pointSize: 13 / Style.monitorRatio
    //                selectedTextColor: Style.backgroundColor
    //                selectionColor: Style.foregroundColor
    //                placeholderTextColor: rootItem.fg50
    //                wrapMode: Text.WrapAnywhere

    //                background: Rectangle {
    //                    color: "transparent"
    //                    radius: 15
    //                }

    //                onTextChanged: {
    //                    //                        console.log(txtSearchType.text)
    //                    //                        tableModel.setFilterColor(txtSearchColor.text)
    //                    tableModel.filterString("filterAllTable", txtSearch.text)
    //                }
    //            }
    //        }

    //        Rectangle {
    //            id: openViewBtn
    //            width: parent.width / 7.9
    //            height: 28 / Style.monitorRatio
    //            color: Style.foregroundColor
    //            anchors.horizontalCenter: parent.horizontalCenter
    //            anchors.verticalCenter: parent.bottom //***********************
    //            radius: 15
    //            z: 3

    //            RowLayout {
    //                anchors.centerIn: parent
    //                spacing: 15 / Style.monitorRatio

    //                Label {
    //                    id: objectLabel
    //                    text: "Filter"
    //                    font.pixelSize: 17 / Style.monitorRatio
    //                    font.family: Style.fontFamily
    //                    color: "white"
    //                }

    //                IconImage {
    //                    id: downIcon
    //                    source: "qrc:/Resources/down-icon.jpg"
    //                    Layout.preferredHeight: 18 / Style.monitorRatio
    //                    Layout.preferredWidth: 18 / Style.monitorRatio
    //                    color: "white"
    //                }
    //            }
    //            MouseArea {
    //                anchors.fill: parent
    //                onClicked: {
    //                    if ((downIcon.rotation / 180) % 2 == 0) {
    //                        openMotion.running = true
    //                        mainRow.visible = true
    //                        flowRow.visible = true
    //                    } else {
    //                        closeMotion.running = true
    //                        mainRow.visible = false
    //                        flowRow.visible = false
    //                    }
    //                    downIcon.rotation += 180
    //                }
    //            }
    //        }
    //        PropertyAnimation {
    //            id: openMotion
    //            target: rectMainSearch
    //            properties: "height"
    //            from: 68
    //            to: 174
    //            duration: 350
    //            easing.type: Easing.OutQuint
    //        }
    //        PropertyAnimation {
    //            id: closeMotion
    //            target: rectMainSearch
    //            properties: "height"
    //            to: 68
    //            from: 174
    //            duration: 350
    //            easing.type: Easing.OutQuint
    //        }

    //        Rectangle {
    //            id: rectFilterMain
    //            width: parent.width
    //            height: 0
    //            color: Style.backgroundColor //"green"
    //            radius: 15
    //            anchors.top: searchRect.bottom
    //            anchors.topMargin: 10

    //            RowLayout {
    //                id: mainRow
    //                width: parent.width
    //                //            anchors.top: searchRect.bottom
    //                //            anchors.topMargin: 15 / Style.monitorRatio
    //                //Layout.fillWidth: true
    //                visible: false

    //                spacing: 0
    //                RowLayout {
    //                    spacing: 2

    //                    Label {
    //                        width: 36 / Style.monitorRatio
    //                        height: 18 / Style.monitorRatio
    //                        text: "Color"
    //                        Layout.leftMargin: 15 / Style.monitorRatio
    //                        font.pixelSize: 15 / Style.monitorRatio
    //                        font.family: Style.fontFamily
    //                        color: Style.foregroundColor
    //                    }
    //                    IconImage {
    //                        id: leftIcon
    //                        source: "qrc:/Resources/left-icon.jpg"
    //                        Layout.preferredHeight: 18 / Style.monitorRatio
    //                        Layout.preferredWidth: 18 / Style.monitorRatio
    //                        //rotation: 90
    //                        //                visible: hbar.position < .01 ? false : true
    //                        color: hbar.position < .01 ? Style.backgroundColor : "transparent"
    //                        MouseArea {
    //                            anchors.fill: parent
    //                            onClicked: {
    //                                if (hbar.position > 0)
    //                                    hbar.position -= 0.24
    //                                print(hbar.position)
    //                            }
    //                        }
    //                    }
    //                    Item {
    //                        width: 115 / Style.monitorRatio
    //                        Layout.fillHeight: true
    //                        clip: true

    //                        RowLayout {
    //                            spacing: 3
    //                            x: -hbar.position * parent.width

    //                            Repeater {
    //                                id: colorRepeater
    //                                model: tableModel ? tableModel.getColorFilter(
    //                                                        ) : undefined //["#EF2929","#FCAF3E","#FCE94F","#8AE234","#EF2929","#FCAF3E","#FCE94F","#8AE234","#729FCF","#AD7FA8","#E9B96E","#8AE234","#729FCF","#AD7FA8","#E9B96E"]
    //                                delegate: Rectangle {
    //                                    required property var modelData
    //                                    width: 24 / Style.monitorRatio
    //                                    height: 24 / Style.monitorRatio
    //                                    radius: height / 2
    //                                    color: modelData
    //                                    MouseArea {
    //                                        anchors.fill: parent
    //                                        cursorShape: Qt.PointingHandCursor
    //                                        onClicked: {
    //                                            tableModel.addTagColor("color",
    //                                                                   modelData)
    //                                            //tagModel.append({ name: "color", signLogical: " : " , value1: modelData, isTo:'', value2:'', filter:"colorFilter" })
    //                                            tagModel.append({
    //                                                                "name": "color",
    //                                                                "color": modelData,
    //                                                                "value1": "",
    //                                                                "value2": "",
    //                                                                "value3": "",
    //                                                                "value4": "",
    //                                                                "compVal": "",
    //                                                                "filter": "colorFilter"
    //                                                            })
    //                                            tableModel.getColorFilter()
    //                                        }
    //                                    }
    //                                }
    //                            }
    //                        }
    //                        ScrollBar {
    //                            id: hbar
    //                            hoverEnabled: true
    //                            active: hovered || pressed
    //                            orientation: Qt.Horizontal
    //                            anchors.left: parent.left
    //                            anchors.right: parent.right
    //                            anchors.bottom: parent.bottom
    //                            visible: false
    //                        }
    //                    }

    //                    IconImage {
    //                        id: rightIcon
    //                        source: "qrc:/Resources/right-icon.jpg"
    //                        Layout.preferredHeight: 18 / Style.monitorRatio
    //                        Layout.preferredWidth: 18 / Style.monitorRatio
    //                        //rotation: -90
    //                        //                visible: hbar.position > .95 ? false : true
    //                        color: hbar.position > .7 ? Style.backgroundColor : "transparent"
    //                        MouseArea {
    //                            anchors.fill: parent
    //                            onClicked: {
    //                                //console.log(hbar.position)
    //                                if (hbar.position < 0.7) {
    //                                    hbar.position += 0.24
    //                                }
    //                            }
    //                        }
    //                    }
    //                }
    //                Rectangle {
    //                    id: filterString
    //                    width: rootItem.widthStyle / 4.65
    //                    height: 28 / Style.monitorRatio
    //                    //Layout.leftMargin: 20 / Style.monitorRatio
    //                    radius: 15
    //                    property color s: "black"
    //                    color: Qt.rgba(s.r, s.g, s.b, .04)
    //                    Rectangle {
    //                        width: rootItem.widthStyle / 4.65 - 3
    //                        height: 28 / Style.monitorRatio - 3
    //                        anchors.bottom: parent.bottom
    //                        anchors.right: parent.right
    //                        color: Style.backgroundColor
    //                        radius: 8
    //                        RowLayout {
    //                            anchors.left: parent.left
    //                            anchors.centerIn: parent
    //                            spacing: 5
    //                            ComboBox {
    //                                id: control
    //                                property real txtWidth: 0
    //                                //Layout.minimumWidth: 50
    //                                //Layout.maximumWidth: 50
    //                                model: tableModel ? tableModel.getDataComboBox(
    //                                                        ) : undefined

    //                                Text {
    //                                    id: maximumText
    //                                    text: "Longest String: "
    //                                    anchors.centerIn: parent
    //                                    Component.onCompleted: {
    //                                        control.txtWidth = maximumText.width
    //                                    }
    //                                    visible: false
    //                                }
    //                                delegate: ItemDelegate {
    //                                    id: itemDelegate
    //                                    implicitWidth: control.txtWidth
    //                                    background: Rectangle {
    //                                        width: control.txtWidth
    //                                        color: Style.backgroundColor
    //                                        border.width: .3
    //                                        border.color: "black"
    //                                        //                                    radius:5
    //                                    }

    //                                    contentItem: Text {
    //                                        //Layout.leftMargin: 20
    //                                        text: control.textRole ? (Array.isArray(
    //                                                                      control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
    //                                        color: Style.foregroundColor
    //                                        font.family: Style.fontFamily
    //                                        font.pixelSize: 14 / Style.monitorRatio
    //                                    }

    //                                    //                                highlighted: control.highlightedIndex === index
    //                                }
    //                                indicator: Rectangle {}

    //                                //                                text: control.displayText
    //                                //                                font.family: Style.fontFamily
    //                                //                                font.pixelSize: 14/Style.monitorRatio
    //                                //                                color:rootItem.fg30
    //                                //                                verticalAlignment: Text.AlignVCenter
    //                                //elide: Text.ElideRight
    //                                contentItem: TextField {
    //                                    id: txtContentItem1
    //                                    implicitWidth: 60 / Style.monitorRatio
    //                                    Layout.fillHeight: true
    //                                    placeholderText: qsTr("subject")
    //                                    placeholderTextColor: rootItem.fg30
    //                                    color: rootItem.fg30
    //                                    font.family: Style.fontFamily
    //                                    font.pixelSize: 15 / Style.monitorRatio
    //                                    selectedTextColor: Style.backgroundColor
    //                                    selectionColor: Style.foregroundColor
    //                                    background: Rectangle {
    //                                        color: "transparent"
    //                                    }

    //                                    //                                text: control2.displayText
    //                                    //                                font.family: Style.fontFamily
    //                                    //                                font.pixelSize: 14/Style.monitorRatio
    //                                    //                                color:rootItem.fg30
    //                                    //                                verticalAlignment: Text.AlignVCenter
    //                                    //placeholderText: "subject"
    //                                    //elide: Text.ElideRight
    //                                    onTextChanged: {
    //                                        if (txtContentItem1.text !== "") {
    //                                            //console.log(text)
    //                                            control.model = tableModel.filterCombo(
    //                                                        text, "String")

    //                                            popupCombo1.open()
    //                                        }
    //                                        if (txtContentItem1.text === "") {
    //                                            //console.log("close combo")
    //                                            popupCombo1.close()
    //                                        }
    //                                    }
    //                                }

    //                                background: Rectangle {
    //                                    color: "transparent"
    //                                }
    //                                popup: Popup {
    //                                    id: popupCombo1
    //                                    y: control.height - 1
    //                                    width: 100
    //                                    implicitHeight: contentItem.implicitHeight
    //                                    padding: 1
    //                                    enter: Transition {
    //                                        NumberAnimation {
    //                                            property: "opacity"
    //                                            from: 0.0
    //                                            to: 1.0
    //                                        }
    //                                    }

    //                                    exit: Transition {
    //                                        NumberAnimation {
    //                                            property: "opacity"
    //                                            from: 1.0
    //                                            to: 0.0
    //                                        }
    //                                    }

    //                                    contentItem: ListView {
    //                                        clip: true
    //                                        implicitHeight: contentHeight
    //                                        model: control.delegateModel //comboFilter1.popup.visible ? comboFilter1.delegateModel : null
    //                                        currentIndex: control.highlightedIndex
    //                                        //visible: false
    //                                        ScrollIndicator.vertical: ScrollIndicator {}
    //                                    }

    //                                    background: Rectangle {
    //                                        border.color: Style.foregroundColor
    //                                        radius: 2
    //                                    }
    //                                    MouseArea {
    //                                        anchors.fill: parent
    //                                        onClicked: {
    //                                            txtContentItem1.text = control.textAt(
    //                                                        control.highlightedIndex)
    //                                            popupCombo1.close()
    //                                        }
    //                                    }
    //                                }
    //                            }
    //                            Label {
    //                                text: ":"
    //                                font.pixelSize: 15 / Style.monitorRatio
    //                                font.family: Style.fontFamily
    //                                color: rootItem.fg30
    //                            }
    //                            TextField {
    //                                id: descriptionField
    //                                //implicitWidth:95 / Style.monitorRatio
    //                                Layout.fillWidth: true
    //                                placeholderText: qsTr("Description")
    //                                color: rootItem.fg30
    //                                font.family: Style.fontFamily
    //                                font.pointSize: 13 / Style.monitorRatio
    //                                selectedTextColor: Style.backgroundColor
    //                                selectionColor: Style.foregroundColor
    //                                placeholderTextColor: rootItem.fg30
    //                                //                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
    //                                background: Rectangle {
    //                                    color: "transparent"
    //                                    radius: 15
    //                                }
    //                                onAccepted: {
    //                                    tagModel.append({
    //                                                        "name": control.currentText,
    //                                                        "color": "",
    //                                                        "value1": descriptionField.text,
    //                                                        "value2": "",
    //                                                        "value3": "",
    //                                                        "value4": "",
    //                                                        "compVal": "",
    //                                                        "filter": "filter1"
    //                                                    })
    //                                    //console.log(control.currentText, descriptionField.text)
    //                                    tableModel.addTag1(control.currentText,
    //                                                       descriptionField.text)
    //                                }
    //                            }
    //                        }
    //                    }
    //                }
    //                Rectangle {
    //                    id: filterRange
    //                    width: rootItem.widthStyle / 3.55
    //                    height: 28 / Style.monitorRatio
    //                    Layout.leftMargin: 15 / Style.monitorRatio
    //                    radius: 15
    //                    property color s: "black"
    //                    color: Qt.rgba(s.r, s.g, s.b, .04)
    //                    Rectangle {
    //                        width: rootItem.widthStyle / 3.55 - 3
    //                        height: 28 / Style.monitorRatio - 3
    //                        anchors.bottom: parent.bottom
    //                        anchors.right: parent.right
    //                        color: Style.backgroundColor
    //                        radius: 8
    //                        RowLayout {
    //                            anchors.left: parent.left
    //                            anchors.leftMargin: 25 / Style.monitorRatio
    //                            anchors.centerIn: parent
    //                            spacing: 5
    //                            ComboBox {
    //                                id: control2
    //                                property real txtWidth: 0
    //                                //Layout.minimumWidth: 50
    //                                //Layout.maximumWidth: 50
    //                                model: tableModel ? tableModel.getDataComboBoxInt(
    //                                                        ) : undefined

    //                                Text {
    //                                    id: maximumText1
    //                                    text: "Longest String: "
    //                                    anchors.centerIn: parent
    //                                    Component.onCompleted: {
    //                                        control2.txtWidth = maximumText1.width
    //                                    }
    //                                    visible: false
    //                                }
    //                                delegate: ItemDelegate {
    //                                    implicitWidth: control2.txtWidth
    //                                    background: Rectangle {
    //                                        width: control2.txtWidth
    //                                        color: Style.backgroundColor
    //                                        border.width: .3
    //                                        border.color: "black"
    //                                        //                                    radius:5
    //                                    }

    //                                    contentItem: Text {
    //                                        //Layout.leftMargin: 20
    //                                        text: control2.textRole ? (Array.isArray(
    //                                                                       control2.model) ? modelData[control2.textRole] : model[control2.textRole]) : modelData
    //                                        color: Style.foregroundColor
    //                                        font.family: Style.fontFamily
    //                                        font.pixelSize: 14 / Style.monitorRatio
    //                                    }

    //                                    //                                highlighted: control.highlightedIndex === index
    //                                }
    //                                indicator: Rectangle {}
    //                                contentItem: TextField {
    //                                    id: txtContentItem
    //                                    implicitWidth: 60 / Style.monitorRatio
    //                                    Layout.fillHeight: true
    //                                    placeholderText: qsTr("subject")
    //                                    placeholderTextColor: rootItem.fg30
    //                                    color: rootItem.fg30
    //                                    font.family: Style.fontFamily
    //                                    font.pixelSize: 15 / Style.monitorRatio
    //                                    selectedTextColor: Style.backgroundColor
    //                                    selectionColor: Style.foregroundColor
    //                                    background: Rectangle {
    //                                        color: "transparent"
    //                                    }
    //                                    //                                text: control2.displayText
    //                                    //                                font.family: Style.fontFamily
    //                                    //                                font.pixelSize: 14/Style.monitorRatio
    //                                    //                                color:rootItem.fg30
    //                                    //                                verticalAlignment: Text.AlignVCenter
    //                                    //placeholderText: "subject"
    //                                    //elide: Text.ElideRight
    //                                    onTextChanged: {
    //                                        if (txtContentItem.text !== "") {
    //                                            //console.log(text)
    //                                            control2.model = tableModel.filterCombo(
    //                                                        text, "Int")

    //                                            popupCombo2.open()
    //                                        }
    //                                        if (txtContentItem.text === "") {
    //                                            //console.log("close combo")
    //                                            popupCombo2.close()
    //                                        }
    //                                    }
    //                                }
    //                                background: Rectangle {
    //                                    color: "transparent"
    //                                }
    //                                popup: Popup {
    //                                    id: popupCombo2
    //                                    y: control2.height - 1
    //                                    width: 100
    //                                    implicitHeight: contentItem.implicitHeight
    //                                    padding: 1
    //                                    enter: Transition {
    //                                        NumberAnimation {
    //                                            property: "opacity"
    //                                            from: 0.0
    //                                            to: 1.0
    //                                        }
    //                                    }

    //                                    exit: Transition {
    //                                        NumberAnimation {
    //                                            property: "opacity"
    //                                            from: 1.0
    //                                            to: 0.0
    //                                        }
    //                                    }

    //                                    contentItem: ListView {
    //                                        clip: true
    //                                        implicitHeight: contentHeight
    //                                        model: control2.delegateModel //comboFilter1.popup.visible ? comboFilter1.delegateModel : null
    //                                        currentIndex: control2.highlightedIndex
    //                                        //visible: false
    //                                        ScrollIndicator.vertical: ScrollIndicator {}
    //                                    }

    //                                    background: Rectangle {
    //                                        border.color: Style.foregroundColor
    //                                        radius: 2
    //                                    }
    //                                    MouseArea {
    //                                        anchors.fill: parent
    //                                        onClicked: {
    //                                            txtContentItem.text = control2.textAt(
    //                                                        control2.highlightedIndex)
    //                                            popupCombo2.close()
    //                                        }
    //                                    }
    //                                }
    //                            }
    //                            Label {
    //                                text: ":"
    //                                font.pixelSize: 15 / Style.monitorRatio
    //                                font.family: Style.fontFamily
    //                                color: rootItem.fg30
    //                            }
    //                            TextField {
    //                                id: txtFromFilter1
    //                                Layout.alignment: Qt.AlignVCenter
    //                                //Layout.alignment: Qt.AlignHCenter
    //                                //Layout.fillWidth: true
    //                                implicitWidth: 60 / Style.monitorRatio
    //                                //Layout.fillHeight: true
    //                                placeholderText: qsTr("Numb")
    //                                color: rootItem.fg30
    //                                font.family: Style.fontFamily
    //                                font.pixelSize: 15 / Style.monitorRatio
    //                                selectedTextColor: Style.backgroundColor
    //                                selectionColor: Style.foregroundColor
    //                                placeholderTextColor: rootItem.fg30
    //                                background: Rectangle {
    //                                    id: redBackG1
    //                                    color: "transparent"
    //                                    radius: 10
    //                                }
    //                                PropertyAnimation {
    //                                    id: rbg1
    //                                    target: redBackG1
    //                                    properties: "opacity"
    //                                    to: 0
    //                                    from: 1
    //                                    duration: 3500
    //                                    easing.type: Easing.OutQuint
    //                                }
    //                                onAccepted: {
    //                                    if (txtFromFilter1.text !== ""
    //                                            && txtToFilter1.text !== "ERROR"
    //                                            && txtToFilter1.text !== ""
    //                                            && rootItem.isNumeric(
    //                                                txtFromFilter1.text)) {

    //                                        tagModel.append({
    //                                                            "name": control2.currentText,
    //                                                            "color": "",
    //                                                            "value1": "",
    //                                                            "value2": txtFromFilter1.text,
    //                                                            "value3": txtToFilter1.text,
    //                                                            "value4": "",
    //                                                            "compVal": "",
    //                                                            "filter": "filter2"
    //                                                        })
    //                                        tableModel.addTag2(
    //                                                    control2.currentText,
    //                                                    txtFromFilter1.text,
    //                                                    txtToFilter1.text)
    //                                    } else {
    //                                        redBackG1.color = "red"
    //                                        rbg1.running = true
    //                                    }
    //                                }
    //                            }
    //                            Label {

    //                                text: "To"
    //                                font.pixelSize: 17 / Style.monitorRatio
    //                                font.family: Style.fontFamily
    //                                color: Style.foregroundColor
    //                            }
    //                            TextField {
    //                                id: txtToFilter1
    //                                //Layout.alignment: Qt.AlignVCenter
    //                                //implicitWidth:60 / Style.monitorRatio
    //                                //Layout.fillHeight: true
    //                                Layout.fillWidth: true
    //                                placeholderText: qsTr("Numb")
    //                                color: rootItem.fg30
    //                                font.family: Style.fontFamily
    //                                font.pixelSize: 15 / Style.monitorRatio
    //                                selectedTextColor: Style.backgroundColor
    //                                selectionColor: Style.foregroundColor
    //                                placeholderTextColor: rootItem.fg30
    //                                background: Rectangle {
    //                                    id: redBackG2
    //                                    color: "transparent"
    //                                    radius: 8
    //                                }
    //                                PropertyAnimation {
    //                                    id: rbg2
    //                                    target: redBackG2
    //                                    properties: "opacity"
    //                                    to: 0
    //                                    from: 1
    //                                    duration: 3500
    //                                    easing.type: Easing.OutQuint
    //                                }
    //                                onAccepted: {

    //                                    //tagModel.append({name: control2.currentText,value2: numbfield1.text, value3 : numbfield2.text})
    //                                    //tableModel.addTag2(control2.currentText, txtFromFilter1.text, txtToFilter1.text)
    //                                    if (txtToFilter1.text !== ""
    //                                            && txtToFilter1.text !== "ERROR"
    //                                            && txtFromFilter1.text !== ""
    //                                            && rootItem.isNumeric(
    //                                                txtFromFilter1.text)) {
    //                                        //console.log(txtFromFilter1.text, txtToFilter1.text)
    //                                        tagModel.append({
    //                                                            "name": control2.currentText,
    //                                                            "color": "",
    //                                                            "value1": "",
    //                                                            "value2": txtFromFilter1.text,
    //                                                            "value3": txtToFilter1.text,
    //                                                            "value4": "",
    //                                                            "compVal": "",
    //                                                            "filter": "filter2"
    //                                                        })
    //                                        tableModel.addTag2(
    //                                                    control2.currentText,
    //                                                    txtFromFilter1.text,
    //                                                    txtToFilter1.text)
    //                                    } else {
    //                                        redBackG2.color = "red"
    //                                        rbg2.running = true
    //                                    }
    //                                }
    //                            }
    //                        }
    //                    }
    //                }

    //                Rectangle {
    //                    id: filterString11
    //                    width: rootItem.widthStyle / 4.65
    //                    height: 28 / Style.monitorRatio
    //                    Layout.leftMargin: 15 / Style.monitorRatio
    //                    radius: 15
    //                    property color s: "black"
    //                    color: Qt.rgba(s.r, s.g, s.b, .04)
    //                    Rectangle {
    //                        width: rootItem.widthStyle / 4.65 - 3
    //                        height: 28 / Style.monitorRatio - 3
    //                        anchors.bottom: parent.bottom
    //                        anchors.right: parent.right
    //                        color: Style.backgroundColor
    //                        radius: 8
    //                        RowLayout {
    //                            //anchors.left: parent.left
    //                            //anchors.leftMargin: 25/ Style.monitorRatio
    //                            anchors.centerIn: parent

    //                            spacing: 5
    //                            ComboBox {
    //                                id: control3
    //                                property real txtWidth: 0
    //                                //Layout.minimumWidth: 50
    //                                //Layout.maximumWidth: 50
    //                                model: tableModel ? tableModel.getDataComboBoxInt(
    //                                                        ) : undefined

    //                                Text {
    //                                    id: maximumText3
    //                                    text: "Longest String: "
    //                                    anchors.centerIn: parent
    //                                    Component.onCompleted: {
    //                                        control3.txtWidth = maximumText3.width
    //                                    }
    //                                    visible: false
    //                                }
    //                                delegate: ItemDelegate {
    //                                    implicitWidth: control3.txtWidth
    //                                    background: Rectangle {
    //                                        width: control3.txtWidth
    //                                        color: Style.backgroundColor
    //                                        border.width: .3
    //                                        border.color: "black"
    //                                        //radius:5
    //                                    }

    //                                    contentItem: Text {
    //                                        //Layout.leftMargin: 20
    //                                        text: control3.textRole ? (Array.isArray(
    //                                                                       control3.model) ? modelData[control3.textRole] : model[control3.textRole]) : modelData
    //                                        color: "#003569"
    //                                        font.family: "Roboto"
    //                                        font.pixelSize: 14 / Style.monitorRatio
    //                                    }

    //                                    //                                highlighted: control.highlightedIndex === index
    //                                }
    //                                indicator: Rectangle {}

    //                                contentItem: TextField {
    //                                    id: txtContentItem3
    //                                    implicitWidth: 60 / Style.monitorRatio
    //                                    Layout.fillHeight: true
    //                                    //Layout.fillWidth: true
    //                                    placeholderText: qsTr("subject")
    //                                    placeholderTextColor: rootItem.fg30
    //                                    color: rootItem.fg30
    //                                    font.family: Style.fontFamily
    //                                    font.pixelSize: 15 / Style.monitorRatio
    //                                    selectedTextColor: Style.backgroundColor
    //                                    selectionColor: Style.foregroundColor
    //                                    background: Rectangle {
    //                                        color: "transparent"
    //                                    }
    //                                    //                                text: control2.displayText
    //                                    //                                font.family: Style.fontFamily
    //                                    //                                font.pixelSize: 14/Style.monitorRatio
    //                                    //                                color:rootItem.fg30
    //                                    //                                verticalAlignment: Text.AlignVCenter
    //                                    //placeholderText: "subject"
    //                                    //elide: Text.ElideRight
    //                                    onTextChanged: {
    //                                        if (txtContentItem3.text !== "") {
    //                                            //console.log(text)
    //                                            control3.model = tableModel.filterCombo(
    //                                                        text, "Int")

    //                                            popupCombo3.open()
    //                                        }
    //                                        if (txtContentItem3.text === "") {
    //                                            //console.log("close combo")
    //                                            popupCombo3.close()
    //                                        }
    //                                    }
    //                                }
    //                                background: Rectangle {
    //                                    color: "transparent"
    //                                    radius: 8
    //                                }
    //                                popup: Popup {
    //                                    id: popupCombo3
    //                                    y: control3.height - 1
    //                                    width: 100
    //                                    implicitHeight: contentItem.implicitHeight
    //                                    padding: 1
    //                                    enter: Transition {
    //                                        NumberAnimation {
    //                                            property: "opacity"
    //                                            from: 0.0
    //                                            to: 1.0
    //                                        }
    //                                    }

    //                                    exit: Transition {
    //                                        NumberAnimation {
    //                                            property: "opacity"
    //                                            from: 1.0
    //                                            to: 0.0
    //                                        }
    //                                    }

    //                                    contentItem: ListView {
    //                                        clip: true
    //                                        implicitHeight: contentHeight
    //                                        model: control3.delegateModel //comboFilter1.popup.visible ? comboFilter1.delegateModel : null
    //                                        currentIndex: control3.highlightedIndex
    //                                        //visible: false
    //                                        ScrollIndicator.vertical: ScrollIndicator {}
    //                                    }

    //                                    background: Rectangle {
    //                                        border.color: Style.foregroundColor
    //                                        radius: 2
    //                                    }
    //                                    MouseArea {
    //                                        anchors.fill: parent
    //                                        onClicked: {
    //                                            txtContentItem3.text = control3.textAt(
    //                                                        control3.highlightedIndex)
    //                                            popupCombo3.close()
    //                                        }
    //                                    }
    //                                }
    //                            }
    //                            Item {
    //                                width: 26 / Style.monitorRatio
    //                                height: 26 / Style.monitorRatio
    //                                //                            anchors.centerIn: filterString11
    //                                //Layout.verticalCenter: filterString11.verticalCenter
    //                                Rectangle {
    //                                    id: comparison
    //                                    anchors.fill: parent
    //                                    radius: width / 2
    //                                    color: Style.backgroundColor
    //                                    Label {
    //                                        id: lblComparision
    //                                        anchors.centerIn: parent
    //                                        text: "="
    //                                        font.pixelSize: 20 / Style.monitorRatio
    //                                        font.family: Style.fontFamily
    //                                        color: Style.foregroundColor
    //                                        MouseArea {
    //                                            anchors.fill: parent
    //                                            onClicked: {
    //                                                comparisonMenu.popup()
    //                                            }
    //                                        }
    //                                        Menu {
    //                                            id: comparisonMenu
    //                                            width: 30

    //                                            Repeater {
    //                                                id: repeaterMenu
    //                                                model: ["<", "<=", "=", ">=", ">"]
    //                                                MenuItem {
    //                                                    text: modelData

    //                                                    background: Rectangle {
    //                                                        width: 30
    //                                                        color: Style.backgroundColor
    //                                                        border.width: .3
    //                                                        border.color: "black"
    //                                                    }
    //                                                    contentItem: Text {
    //                                                        text: modelData
    //                                                        //width: 50
    //                                                        color: "#003569"
    //                                                        font.family: "Roboto"
    //                                                        font.pixelSize: 14 / Style.monitorRatio
    //                                                    }
    //                                                    onClicked: {
    //                                                        lblComparision.text = modelData
    //                                                    }
    //                                                }
    //                                            }

    //                                            enter: Transition {
    //                                                NumberAnimation {
    //                                                    property: "opacity"
    //                                                    from: 0.0
    //                                                    to: 1.0
    //                                                }
    //                                            }

    //                                            exit: Transition {
    //                                                NumberAnimation {
    //                                                    property: "opacity"
    //                                                    from: 1.0
    //                                                    to: 0.0
    //                                                }
    //                                            }
    //                                        }
    //                                    }
    //                                }
    //                                MultiEffect {
    //                                    source: comparison
    //                                    enabled: true
    //                                    anchors.fill: comparison
    //                                    shadowColor: "black"
    //                                    shadowEnabled: true
    //                                    shadowBlur: 1
    //                                    shadowHorizontalOffset: 0.5
    //                                    shadowVerticalOffset: 0
    //                                    shadowOpacity: 1
    //                                    shadowScale: 0.6
    //                                }
    //                            }
    //                            TextField {
    //                                id: txtFilter3
    //                                //implicitWidth:60 / Style.monitorRatio
    //                                Layout.fillWidth: true
    //                                //Layout.fillHeight: true
    //                                placeholderText: qsTr("Numb")
    //                                color: rootItem.fg30
    //                                font.family: Style.fontFamily
    //                                font.pixelSize: 15 / Style.monitorRatio
    //                                selectedTextColor: Style.backgroundColor
    //                                selectionColor: Style.foregroundColor
    //                                placeholderTextColor: rootItem.fg30
    //                                background: Rectangle {
    //                                    id: redBackG3
    //                                    color: "transparent"
    //                                    radius: 8
    //                                }
    //                                PropertyAnimation {
    //                                    id: rbg3
    //                                    target: redBackG3
    //                                    properties: "opacity"
    //                                    to: 0
    //                                    from: 1
    //                                    duration: 3500
    //                                    easing.type: Easing.OutQuint
    //                                }
    //                                onAccepted: {
    //                                    if (rootItem.isNumeric(txtFilter3.text)) {
    //                                        tagModel.append({
    //                                                            "name": control3.currentText,
    //                                                            "color": "",
    //                                                            "value1": "",
    //                                                            "value2": "",
    //                                                            "value3": "",
    //                                                            "value4": txtFilter3.text,
    //                                                            "compVal": lblComparision.text,
    //                                                            "filter": "filter3"
    //                                                        })
    //                                        tableModel.addTag3(
    //                                                    control3.currentText,
    //                                                    txtFilter3.text,
    //                                                    lblComparision.text)
    //                                    } else {
    //                                        redBackG3.color = "red"
    //                                        rbg3.running = true
    //                                    }
    //                                }
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //            RowLayout {
    //                width: rootItem.widthStyle
    //                id: flowRow
    //                height: 50 / Style.monitorRatio
    //                visible: false
    //                anchors.top: mainRow.bottom
    //                anchors.topMargin: 20
    //                //anchors.bottom: parent.bottom
    //                Flow {
    //                    id: felo
    //                    spacing: 5 / Style.monitorRatio
    //                    height: 40 / Style.monitorRatio
    //                    Layout.bottomMargin: 20
    //                    Layout.fillWidth: true
    //                    clip: true
    //                    Repeater {
    //                        id: typesRepeater
    //                        model: ListModel {
    //                            id: tagModel
    //                        }
    //                        delegate: Rectangle {
    //                            property bool visiblitySet: true
    //                            property bool checked: true
    //                            property bool selected: false
    //                            id: typeHolder
    //                            implicitHeight: 26 / Style.monitorRatio
    //                            implicitWidth: shortCut.implicitWidth
    //                            color: checked ? "transparent" : Qt.rgba(
    //                                                 Style.foregroundColor.r,
    //                                                 Style.foregroundColor.g,
    //                                                 Style.foregroundColor.b, 0.1)
    //                            radius: 20
    //                            border {

    //                                color: "#01AED6" /*rootItem.disableColor*/
    //                                width: 1
    //                            }

    //                            MouseArea {
    //                                anchors.fill: parent
    //                                hoverEnabled: true
    //                                onEntered: {
    //                                    if (!typeHolder.selected == true) {
    //                                        typeHolder.border.color = Style.foregroundColor
    //                                        shortCut.colorHandler = Style.foregroundColor
    //                                    } else {
    //                                        typeHolder.border.color = Style.foregroundColor
    //                                        shortCut.colorHandler = Style.foregroundColor
    //                                    }
    //                                }
    //                                onExited: {
    //                                    if (typeHolder.selected == true) {
    //                                        typeHolder.border.color = rootItem.fg30
    //                                        shortCut.colorHandler = rootItem.fg30
    //                                    } else {
    //                                        typeHolder.border.color = "#01AED6"
    //                                        shortCut.colorHandler = "#01AED6"
    //                                    }
    //                                }
    //                                onClicked: {
    //                                    if (!typeHolder.selected) {
    //                                        typeHolder.selected = true
    //                                        typeHolder.border.color = "#01AED6"
    //                                        shortCut.colorHandler = "#01AED6"
    //                                        console.log("disable Tag:", filter)
    //                                        //tableModel.removeTag(filter, name, value1)
    //                                        if (model.filter === "colorFilter") {
    //                                            tableModel.removeTag(model.filter,
    //                                                                 model.name,
    //                                                                 model.color)
    //                                        } else if (model.filter === "filter1") {
    //                                            tableModel.removeTag(model.filter,
    //                                                                 model.name,
    //                                                                 value1)
    //                                        } else if (model.filter === "filter2") {
    //                                            tableModel.removeTag(model.filter,
    //                                                                 model.name,
    //                                                                 model.value2)
    //                                        } else if (model.filter === "filter3") {
    //                                            tableModel.removeTag(model.filter,
    //                                                                 model.name,
    //                                                                 model.value4)
    //                                        }
    //                                    } else {
    //                                        typeHolder.selected = false
    //                                        typeHolder.border.color = rootItem.fg30
    //                                        shortCut.colorHandler = rootItem.fg30
    //                                        console.log("enable Tag:", filter)
    //                                        if (filter === "colorFilter")
    //                                            tableModel.addTagColor(name,
    //                                                                   model.color)
    //                                        else if (filter === "filter1")
    //                                            tableModel.addTag1(name, value1)
    //                                        else if (filter === "filter2")
    //                                            tableModel.addTag2(name,
    //                                                               value2, value3)
    //                                        else if (filter === "filter3")
    //                                            tableModel.addTag3(name,
    //                                                               value4, compVal)
    //                                    }
    //                                }
    //                            }

    //                            //            Component.onCompleted: {
    //                            //                if (typesRepeater.model.rowCount()){
    //                            //                    objectLabel.visible=true
    //                            //                }
    //                            //            }
    //                            //            visible: flag
    //                            RowLayout {
    //                                id: shortCut
    //                                anchors.fill: parent
    //                                property color colorHandler: "#01AED6"
    //                                property string filterRemove: ''

    //                                Text {
    //                                    id: itemText
    //                                    Layout.alignment: Qt.AlignLeft
    //                                    text: model.name ? model.name : 0
    //                                    font.family: "Roboto"
    //                                    font.pixelSize: 17 / Style.monitorRatio
    //                                    color: shortCut.colorHandler /*typeHolder.checked ? Style.foregroundColor : Style.hoverColor*/
    //                                    Layout.leftMargin: 15 / Style.monitorRatio
    //                                    Layout.topMargin: 2 / Style.monitorRatio
    //                                    Layout.bottomMargin: 2 / Style.monitorRatio
    //                                }
    //                                Label {
    //                                    text: ":"
    //                                    font.pixelSize: 17 / Style.monitorRatio
    //                                    font.family: "Roboto"
    //                                    color: shortCut.colorHandler
    //                                    visible: model.compVal ? false : true
    //                                }

    //                                Text {
    //                                    id: itemValue
    //                                    Layout.alignment: Qt.AlignLeft
    //                                    text: model.value1 ? model.value1 : 0
    //                                    font.family: "Roboto"
    //                                    font.pixelSize: 17 / Style.monitorRatio
    //                                    color: shortCut.colorHandler /*typeHolder.checked ? Style.foregroundColor : Style.hoverColor*/
    //                                    visible: model.value2 || model.value4
    //                                             || model.color ? false : true
    //                                }
    //                                Text {
    //                                    id: itemValue2
    //                                    Layout.alignment: Qt.AlignLeft
    //                                    text: model.value2 ? model.value2 : 0
    //                                    font.family: "Roboto"
    //                                    font.pixelSize: 17 / Style.monitorRatio
    //                                    color: shortCut.colorHandler /*typeHolder.checked ? Style.foregroundColor : Style.hoverColor*/
    //                                    visible: model.value3
    //                                             && model.value2 ? true : false
    //                                }
    //                                Label {
    //                                    text: "To"
    //                                    font.pixelSize: 17 / Style.monitorRatio
    //                                    font.family: "Roboto"
    //                                    color: shortCut.colorHandler
    //                                    visible: model.value2 ? true : false
    //                                }
    //                                Text {
    //                                    id: itemValue3
    //                                    Layout.alignment: Qt.AlignLeft
    //                                    text: model.value3 ? model.value3 : 0
    //                                    font.family: "Roboto"
    //                                    font.pixelSize: 17 / Style.monitorRatio
    //                                    color: shortCut.colorHandler /*typeHolder.checked ? Style.foregroundColor : Style.hoverColor*/
    //                                    visible: model.value3
    //                                             && model.value2 ? true : false
    //                                }
    //                                Text {
    //                                    id: comparisonIcon
    //                                    Layout.alignment: Qt.AlignLeft
    //                                    text: model.compVal ? model.compVal : 0
    //                                    font.family: "Roboto"
    //                                    font.pixelSize: 17 / Style.monitorRatio
    //                                    color: shortCut.colorHandler /*typeHolder.checked ? Style.foregroundColor : Style.hoverColor*/
    //                                    visible: model.compVal ? true : false
    //                                }
    //                                Text {
    //                                    id: itemValue4
    //                                    Layout.alignment: Qt.AlignLeft
    //                                    text: model.value4 ? model.value4 : 0
    //                                    font.family: "Roboto"
    //                                    font.pixelSize: 17 / Style.monitorRatio
    //                                    color: shortCut.colorHandler /*typeHolder.checked ? Style.foregroundColor : Style.hoverColor*/
    //                                    visible: model.value4 ? true : false
    //                                }
    //                                Text {
    //                                    id: itemColor
    //                                    Layout.alignment: Qt.AlignLeft
    //                                    text: model.color ? model.color : 0
    //                                    font.family: "Roboto"
    //                                    font.pixelSize: 17 / Style.monitorRatio
    //                                    color: shortCut.colorHandler /*typeHolder.checked ? Style.foregroundColor : Style.hoverColor*/
    //                                    visible: model.color ? true : false
    //                                }
    //                                IconImage {
    //                                    id: closeIcon
    //                                    source: "qrc:/Resources/close-icon.jpg"
    //                                    Layout.preferredHeight: 20 / Style.monitorRatio
    //                                    Layout.preferredWidth: 20 / Style.monitorRatio
    //                                    color: shortCut.colorHandler
    //                                    Layout.leftMargin: 15 / Style.monitorRatio
    //                                    Layout.rightMargin: 15 / Style.monitorRatio
    //                                    MouseArea {
    //                                        anchors.fill: parent
    //                                        cursorShape: Qt.PointingHandCursor
    //                                        onClicked: {

    //                                            if (model.filter === "colorFilter") {
    //                                                console.log(filter)
    //                                                console.log(model.name,
    //                                                            model.color)
    //                                                tableModel.removeTag(
    //                                                            model.filter,
    //                                                            model.name,
    //                                                            model.color)
    //                                            } else if (model.filter === "filter1") {
    //                                                console.log(filter)
    //                                                console.log(model.name,
    //                                                            model.value1)
    //                                                tableModel.removeTag(
    //                                                            model.filter,
    //                                                            model.name, value1)
    //                                            } else if (model.filter === "filter2") {
    //                                                console.log(filter)
    //                                                console.log(model.name,
    //                                                            model.value2,
    //                                                            model.value3)
    //                                                tableModel.removeTag(
    //                                                            model.filter,
    //                                                            model.name,
    //                                                            model.value2)
    //                                            } else if (model.filter === "filter3") {
    //                                                console.log(filter)
    //                                                console.log(model.name,
    //                                                            model.value4)
    //                                                tableModel.removeTag(
    //                                                            model.filter,
    //                                                            model.name,
    //                                                            model.value4)
    //                                            }
    //                                            tagModel.remove(index)
    //                                            //console.log(model.name, model.color, model.value1, model.value2, model.value3, model.value4, model.compVal)
    //                                            //tableModel.removeTag(filter, name, value1)
    //                                        }
    //                                    }
    //                                }
    //                            }
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }

    //            Rectangle{
    //                id: searchRect
    //                color: Style.backgroundColor
    //                Layout.fillWidth: true
    //                Layout.fillHeight: true
    //                Layout.bottomMargin: 1

    //                TextField{
    //                    id: txtSearch
    //                    Layout.leftMargin: 20
    //                    Layout.topMargin: 20
    //                    implicitWidth: 300
    //                    implicitHeight: 25
    //                    placeholderText: "Search"
    //                    onTextChanged: {
    //                        //                        console.log(txtSearchType.text)
    //                        //                        tableModel.setFilterColor(txtSearchColor.text)
    //                        tableModel.filterString("filterAllTable", txtSearch.text)
    //                    }

    //                }

    //            }}
    Rectangle {
        id: categoryRect
        color: Style.backgroundColor
        width: rootItem.width
        height: 50
        anchors.top: rootItem.top
        anchors.topMargin: 30
        anchors.left: rootItem.left
        //        anchors.leftMargin: 20
        anchors.right: rootItem.right
        //anchors.rightMargin: 20
        //Layout.fillHeight: true
        //Layout.fillWidth: true
        //            Layout.bottomMargin: 1
        //Layout.leftMargin: 20
        TabBar {
            id: tabBar
            width: parent.width
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
            currentIndex: 0
            Component.onCompleted: {

                //tableModel.filterStringColumn(repeater.itemAt(0).text)
                //console.log(tabMain.modelData)
            }

            Repeater {
                id: repeater
                model: /*tableModel ? tableModel.getTabBarName() : undefined*/ [1, 2, 3]
                Component.onCompleted: {

                    //tableModel.filterStringColumn(repeater.itemAt(0).text)
                    //console.log(repeater)
                }

                TabButton {
                    id: tabMain
                    required property var model
                    required property var modelData

                    Component.onCompleted: {

                        tableModel.filterStringColumn(repeater.itemAt(0).text)
                        //console.log(repeater.itemAt(0).text)
                    }

                    text: modelData

                    onClicked: {
                        console.log("show table: ", txtTabbar.text)
                        tableModel.filterStringColumn(txtTabbar.text)
                        //console.log(index)
                    }
                    background: Rectangle {
                        id: tabBarBack
                        //implicitWidth: 20
                        //implicitHeight: 20
                        color: Style.backgroundColor
                        //radius: tabBarBacks.implicitWidth
                        Rectangle {
                            id: rectBackTabbar
                            width: parent.width
                            height: 2
                            color: tabBar.currentIndex
                                   === model.index ? Style.foregroundColor : Style.disableColor
                            anchors.bottom: parent.bottom
                        }
                    }
                    contentItem: Text {
                        id: txtTabbar
                        //anchors.fill: parent
                        //anchors.centerIn: parent
                        anchors.bottom: tabBarBack.bottom
                        anchors.bottomMargin: 5
                        text: tabMain.text
                        font.family: "Roboto"
                        font.pointSize: 17 / Style.monitorRatio
                        color: tabBar.currentIndex
                               === model.index ? Style.foregroundColor : Style.disableColor
                    }
                }
            }
        }
    }

    HorizontalHeaderView {
        anchors.left: rootItem.left
        anchors.leftMargin: 20
        anchors.bottom: scrollViewTable.top
        syncView: tableview
        clip: true
        delegate: Rectangle {
            id: rectHorizontalHeaderView

            implicitHeight: 30
            implicitWidth: 50 //parent.width
            color: "transparent" //"#DEE3E6"

            Rectangle {
                width: parent.width
                height: 2
                color: Style.foregroundColor
                anchors.bottom: parent.bottom
            }

            Text {
                text: display
                color: Style.foregroundColor
                font.family: Style.fontFamily
                font.pointSize: 17 / Style.monitorRatio
                //anchors.centerIn: parent
                anchors.left: model.column === 2 ? parent.left : undefined
                anchors.centerIn: model.column === 2 ? undefined : parent
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: model.column === 2 ? -tableview.columnZero
                                                         - tableview.columnIcons : 0

                visible: model.column !== 0 && model.column
                         !== 1 && model.column !== tableModel.columnCount()
                         - 1 && model.column !== tableModel.columnCount()
                         - 2 && model.column !== tableModel.columnCount() - 3
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    //                            console.log(model.index)
                    console.log(model.index) //model : data displayRole in c++
                    tableModel.sortTable(
                                model.index) // model : data index in c++
                }
            }
        }
    }
    ScrollView {
        id: scrollViewTable
        anchors.left: rootItem.left
        anchors.right: rootItem.right
        // width: rootItem.width
        // height: rootItem.height
        anchors.top: categoryRect.bottom
        anchors.topMargin: 20
        anchors.bottom: rootItem.bottom
        //anchors.topMargin: 5
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        TableView {
            id: tableview

            anchors.fill: parent //tableviewRect
            //Layout.alignment: Qt.AlignLeft
            anchors.leftMargin: 20
            //anchors.topMargin: 20
            selectionBehavior: TableView.SelectRows //tableview.SelectRows
            property int selectedRow: -1
            property int columnZero: 5 //tableview.width / tableModel.columnCount() / 7
            property int columnIcons: 35 //tableview.width / tableModel.columnCount() / 2
            property color attackRowColor: rootItem.fg20
            property int checkAttackIconRow: -1
            property int checkAttackIconColumn: -1

            //columnSpacing: 1
            rowSpacing: 1
            clip: true

            function oddRow(IndexRow) {
                return IndexRow % tableview.rows
                //                return tableview.rows % IndexRow
            }

            model: tableModel //tableModel

            //selectionModel: tableModel.selectModel()
            //            selectionModel: ItemSelectionModel {
            //                id: selectionID
            //                model: tableview.selectionModel
            //                //model: tableModel
            //            }
            delegate: Rectangle {
                id: rectDelegate

                //implicitWidth: 120 //rootItem.width / tableModel.columnCount() //120 //rootItem.height / 3
                implicitHeight: 32
                radius: model.column === 0 ? 10 : 0
                color: selected ? tableview.attackRowColor : background

                //anchors.right: column === tableModel.columnCount()-1 ? parent.right : undefined
                required property bool selected
                property int idxRow: model.row
                //property int rowCount: tableModel.getColumnCount()
                property int columnCnt: tableModel.columnCount()
                IconImage {
                    id: icons
                    anchors.centerIn: parent
                    color: (column === tableview.checkAttackIconColumn
                            && row === tableview.checkAttackIconRow) ? "#01AED6" : "#003569"
                    //source: model.column === 1 || model.column === 15 || model.column === 16 || model.column === 17 ? decorate : "qrc:/Resources/airplane.png" //"qrc:/Resources/airplane.png" //decorate
                    visible: model.column === 1 || model.column === tableModel.columnCount()
                             - 1 || model.column === tableModel.columnCount()
                             - 2 || model.column === tableModel.columnCount() - 3
                    source: decorate

                    width: 30
                    height: 30
                    //visible: model.column === 1 || model.column === 15 || model.column === 16 || model.column === 17
                }

                Text {
                    text: display /* === undefined ? 'blank' : display*/
                    font.pointSize: 17 / Style.monitorRatio
                    font.family: Style.fontFamily
                    color: Style.foregroundColor
                    //anchors.fill: parent
                    //anchors.centerIn: parent
                    anchors.left: model.column === 2 ? parent.left : undefined
                    anchors.centerIn: model.column === 2 ? undefined : parent
                    anchors.verticalCenter: parent.verticalCenter
                    visible: model.column !== 0 && model.column
                             !== 1 && model.column !== tableModel.columnCount()
                             - 3 && model.column !== tableModel.columnCount()
                             - 2 && model.column !== tableModel.columnCount() - 1
                }
                Rectangle {
                    width: parent.width
                    height: 1
                    color: Style.foregroundColor
                    opacity: 0.2
                    anchors.bottom: parent.bottom
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                        //selectionID.select(tableview.selectionModel.model.index(row,0), ItemSelectionModel.ClearAndSelect | ItemSelectionModel.Rows)
                        //tableview.selectionModel.select(tableview.selectionModel.index(0,2), ItemSelectionModel.Toggle | ItemSelectionModel.Rows)
                        //tableview.selectedRow = model.row
                        //tableModel.selectionRow(row, 18)
                        // console.log("Row index: ", model.row, "column", columnCnt)
                        //console.log("column count : ", rowCount)
                        //console.log("column filter count: ", tableModel.columnCount())
                        //model.column === 11
                        if (decorate === "qrc:/Resources/more-icon.jpg") {
                            menuTable.popup()
                        } else if (decorate === "qrc:/Resources/battle-icon.jpg"
                                   && model.row === 0
                                   && tableview.checkAttackIconRow !== -1) {
                            console.log("chnage model")
                            tableModel.setChangeModel("")
                            tableview.checkAttackIconRow = -1
                            tableview.checkAttackIconColumn = -1
                            //tableview.selectedRow = model.row
                            //tableModel.selectionRow(tableview.selectedRow, 18)
                        } else if (decorate === "qrc:/Resources/battle-icon.jpg") {
                            //console.log(tableview.itemAtIndex(0,2))
                            tableModel.attacker(tableModel.data(tableview.index(
                                                                    model.row,
                                                                    2)))
                            tableModel.setChangeModel("attackerModel")
                            //tableview.model = tableModel ? tableModel : undefined
                            //change color attackIcon
                            tableview.checkAttackIconRow = 0
                            tableview.checkAttackIconColumn = column
                            selectionID.select(
                                        tableview.selectionModel.model.index(
                                            0, 0),
                                        ItemSelectionModel.ClearAndSelect | ItemSelectionModel.Rows)

                            //tableModel.selectionRow(row, 0)
                        }

                        if (decorate === "qrc:/Resources/target-icon.jpg") {
                            console.log(tableModel.data(tableview.index(
                                                            model.row, 2)))
                            tableview.index(0, 11)

                            //console.log(move)
                            //tableModel.moveAttackerToFirst(model.row)
                        }
                    }
                }
            }

            columnWidthProvider: function (column) {
                var columnProviderSize = rootItem.width / (tableModel.columnCount(
                                                               ) - 2)
                //console.log(rootItem.width / tableModel.columnCount())
                return column === 0 ? tableview.columnZero : (column === 1
                                                              || column === tableModel.columnCount(
                                                                  ) - 2
                                                              || column === tableModel.columnCount(
                                                                  ) - 3 ? tableview.columnIcons : columnProviderSize)
                //return tableview.width / tableModel.columnCount();
            } //rectDelegate.implicitWidth
            Menu {
                id: menuTable
                padding: 5
                Repeater {
                    model: ["Go To", "Track"]

                    MenuItem {
                        id: menuItem
                        //implicitWidth: 89
                        //implicitHeight: 24
                        text: modelData
                        contentItem: RowLayout {
                            anchors.fill: parent
                            spacing: 2
                            IconImage {
                                Layout.preferredHeight: 20
                                Layout.preferredWidth: 20
                                //color: "yellow"
                                //Layout.leftMargin: 15 / rootItem.monitorRatio
                                //Layout.rightMargin: 15 / rootItem.monitorRatio
                                source: model.index === 0 ? "qrc:/Resources/goto-icon.jpg" : "qrc:/Resources/track-icon.jpg"
                            }
                            Text {
                                //padding: 5
                                //leftPadding: menuItem.indicator.width
                                //rightPadding: menuItem.arrow.width
                                text: modelData //menuItem.text
                                //font: menuItem.font
                                //opacity: enabled ? 1.0 : 0.3
                                color: Style.foregroundColor //menuItem.highlighted ? "#ffffff" : "#21be2b"
                                //horizontalAlignment: Text.AlignLeft
                                font.family: Style.fontFamily
                                font.pixelSize: 17 / Style.monitorRatio
                                verticalAlignment: Text.AlignVCenter
                                //elide: Text.ElideRight
                            }
                        }

                        background: Rectangle {
                            implicitWidth: 89
                            implicitHeight: 24
                            color: menuItem.highlighted ? rootItem.fg20 : "transparent"
                            radius: 10
                        }
                    }
                }

                background: Rectangle {
                    implicitWidth: 92
                    implicitHeight: 24
                    color: Style.backgroundColor
                    border.color: rootItem.fg20
                    radius: 10
                }
            }
        }
    }
}

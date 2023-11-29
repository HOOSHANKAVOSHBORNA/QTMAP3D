import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"

Item{
    property var model
    property bool flag: false
    property int iconSize: 26/Style.monitorRatio
    readonly property color fg: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.50)
    id: rootItem
    width: searchRect.width
    height: 240/Style.monitorRatio
    Rectangle {
        id:searchRect
        width: flag? 340 /Style.monitorRatio: searchBtn.width + searchText.implicitWidth + (closeButton.visible? closeButton.width : 0)
        height: 40/Style.monitorRatio
        radius: height/2
        color:Style.backgroundColor
        RowLayout{
            id: rowLayout
            anchors.fill: parent
            spacing: 0

            RoundButton {
                id:searchBtn
                Layout.fillHeight: true
                background: Rectangle{
                    radius:height/2
                    color: Style.backgroundColor
                }

                icon{
                    source: "qrc:/Resources/search.png"
                    width: iconSize
                    height: iconSize
                }
                onClicked: {
                    icon.color = Style.disableColor
                    flag = true
                    closeButton.visible = true
                    textonFocus.running =true
                    searchText.focus = true

                }
            }

            TextField {
                id : searchText
                Layout.fillHeight: true
                Layout.fillWidth: true
                implicitWidth: 0
                placeholderText: implicitWidth? qsTr("Search ..." ) : ""
                color: Style.foregroundColor
                font.family: Style.fontFamily
                font.pointSize: Style.fontPointSize
                selectByMouse: true
                selectedTextColor: Style.foregroundColor
                selectionColor: Style.selectColor
                placeholderTextColor: Style.disableColor
                background: Rectangle{
                    color: Style.backgroundColor
                    radius: height/2
                }
                onTextChanged: {
                    rootItem.model.setFilterString(text)
                }

                PropertyAnimation {
                    id : textonFocus
                    target: searchText
                    properties: "implicitWidth"
                    to:(340-2*26)/Style.monitorRatio
                    duration: 150
                    easing.type: Easing.OutQuint
                }
                PropertyAnimation {
                    id : textlostFocus
                    target: searchText
                    properties: "implicitWidth"
                    to:0
                    duration: 150
                    easing.type: Easing.OutQuint
                }
            }

            RoundButton {
                id: closeButton
                Layout.fillHeight: true
                visible: false
                background: Rectangle{
                    color: Style.backgroundColor
                    radius: height/2
                }

                icon{
                    source: "qrc:/Resources/index.png"
                    width: iconSize
                    height: iconSize
                    color: Style.foregroundColor
                }
                onClicked: {
                    flag=false
                    closeButton.visible = false
                    textlostFocus.running =true
                    searchText.clear()
                    searchBtn.icon.color = Style.foregroundColor
                }
            }
        }
        //-------------------- search list ----------------------//
        Rectangle{
            id:dropDown
            color: Style.backgroundColor
            opacity: 0.5
            visible:flag
            width: closeButton.visible ? rootItem.width: 0
            height: rootItem.height
            z: -1
            radius: searchRect.radius
            ColumnLayout{
                anchors.fill: parent
                anchors.margins: 5
                anchors.topMargin: searchRect.height + 5
                Label {
                    background: Rectangle {
                        color: "transparent"
                    }

                    Text {
                        color: Style.disableColor
                        font.pixelSize: 14/Style.monitorRatio
                        text: "Shortcut To Find "
                        font.family: Style.fontFamily
                    }
                    visible: flag
                }

                Flow{
                    Layout.fillWidth: true
                    flow: GridLayout.LeftToRight

                    Repeater {
                        id:typesRepeater
                        model: rootItem.model.getTypeListModel()
                        Rectangle{
                            id:typeHolder
                            implicitHeight: 31/Style.monitorRatio
                            implicitWidth:  shortCut.implicitWidth
                            color:"transparent"
                            radius: 20
                            MouseArea {
                                    anchors.fill: parent
                                    onClicked: {

print(itemText.text)
                                        rootItem.model.setFilterString(itemText.text)
//                                        rootItem.model.filterByType(itemText.text)
                                    }
                                }
                            border{
                                color: Style.disableColor
                                width: 1

                            }
                            Component.onCompleted: {
                                if (typesRepeater.model.rowCount()){
                                    seperator.visible = true
                                    idLabel.visible = true
                                    nameLabel.visible=true
                                }
                            }
                            visible: flag

                            RowLayout{
                                id:shortCut

                                anchors.fill: parent
                                IconImage {

                                    id: searchIcon
                                    source: "qrc:/Resources/search.png"
                                    Layout.preferredHeight: 19/Style.monitorRatio
                                    Layout.preferredWidth: 19/Style.monitorRatio
                                    Layout.leftMargin: 4/Style.monitorRatio

                                    color: Style.foregroundColor
                                }
                                Text {
                                    id: itemText
                                    Layout.alignment: Qt.AlignLeft
                                    Layout.rightMargin: 6/Style.monitorRatio
                                    text:display
                                    font.family: Style.fontFamily
                                    font.pixelSize: 16/Style.monitorRatio
                                    color: Style.foregroundColor
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
                RowLayout{
                    Layout.leftMargin: 5
                    Layout.fillWidth: true
                    spacing: listView.width/3


                    Label {
                        id:idLabel
                        text: "id :"
                        font.pixelSize:16/Style.monitorRatio
                        font.family: Style.fontFamily
                        color: Style.foregroundColor
                        visible:typesRepeater.model.rowCount()
                    }
                    Label {
                        id:nameLabel
                        text: "Name :"
                        font.pixelSize:16/Style.monitorRatio
                        font.family: Style.fontFamily
                        color: Style.foregroundColor
                        visible:typesRepeater.model.rowCount()
                    }
                }
                Rectangle{
                    id:seperator
                    Layout.fillWidth: true
                    height: 1
                    color: Style.foregroundColor
                    opacity: .2
                    visible:typesRepeater.model.rowCount()
                }
                ScrollView{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ListView{
                        id:listView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: rootItem.model
                        delegate:
                            Button
                        {
                            id:delegateBtn
                            width: listView.width
                            height: 20
                            hoverEnabled: true

                            contentItem:Item {
                                anchors.fill: parent

                                Text {
                                    anchors.left: id.right
                                    anchors.leftMargin: delegateBtn.width/3
                                    text: model.display
                                    font.family: Style.fontFamily
                                    font.pointSize: Style.fontPointSize + 2
                                    color: Style.foregroundColor

                                    elide: Text.ElideRight
                                }
                                Text {
                                    id:id
                                    text: model.id_
                                    font.family: Style.fontFamily
                                    font.pointSize: Style.fontPointSize + 2
                                    color: Style.foregroundColor

                                    elide: Text.ElideRight
                                }
                            }
                            onClicked:{
                                rootItem.model.onNodeClicked(index)
                            }
                            background: Rectangle
                            {
                                color: parent.hovered ? Style.hoverColor : "transparent"
                                radius:height/2
                            }
                        }

                    }
                }
            }
        }
    }
}



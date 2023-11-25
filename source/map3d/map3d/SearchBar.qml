import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"

Item{
    property var model
    property int iconSize: 26/Style.monitorRatio
    readonly property color fg: Qt.rgba(Style.foregroundColor.r, Style.foregroundColor.g, Style.foregroundColor.b, 0.50)
//Rectangle{
//width: 250
//height:250
//y:30
//x:-300


//ListView{
//    anchors.fill: parent
//    model:rootItem.model.getTypeListModel()
//    delegate: Button{
//        width: 50
//        height: 50
//        text: model.display
//        onClicked: {
//            print("model.getTypeListModel")
//            print(rootItem.model.getTypeListModel())
//        }
//    }
//}

//color:"lightblue"
//}
    id: rootItem
    width: searchRect.width
    height: 240/Style.monitorRatio
    Rectangle {
        id:searchRect
        width: searchBtn.width + searchText.implicitWidth + (closeButton.visible? closeButton.width : 0)
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
            visible:true
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
                }
                Flow{
                    width: closeButton.visible ? 344/Style.monitorRatio: 0
                    spacing: 2

                    Repeater {
                        model: rootItem.model.getTypeListModel()
                        Rectangle{
                            implicitHeight: 31/Style.monitorRatio
                            implicitWidth:  shortCut.implicitWidth
                            color:"transparent"
                            radius: 20
                            border{
                                color: Style.disableColor
                                width: 1

                            }

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
                //                }

                ListView{
                    id:listView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    //                anchors.fill: parent
                    //                anchors.margins: 5
                    //                Layout.topMargin:  searchRect.height + 5
                    clip: true
                    model: rootItem.model
                    delegate: Button
                    {
                        id:delegateBtn
                        width: listView.width
                        hoverEnabled: true
                        contentItem: Text {
                            text: model.display
                            font.family: Style.fontFamily
                            font.pointSize: Style.fontPointSize + 2
                            color: Style.foregroundColor
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
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



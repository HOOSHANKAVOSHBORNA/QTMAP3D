import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"

Item{
    property var model
    property int iconSize: 26/Style.monitorRatio
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
            ScrollView{
                anchors.fill: parent


            ListView{
                id:listView
                anchors.fill: parent
                anchors.margins: 5
                anchors.topMargin: searchRect.height + 5
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



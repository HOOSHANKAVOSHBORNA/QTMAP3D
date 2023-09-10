import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"

Rectangle {
    property var model
    id: rootItem
    width: Style.containerSize + txtid.implicitWidth + (closeButton.visible? closeButton.width : 0)
    height: Style.containerSize
    radius: height/2
    color:Style.backgroundColor
    opacity: 0.85
    ColumnLayout{
        anchors.fill: parent
        //---------------------------searcbox---------------------------//
        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true
            layoutDirection: Qt.LeftToRight

            //------------------------------search-----------------------//

            Rectangle{
                id:searchContainer
                Layout.bottomMargin: 10
                Layout.leftMargin: 2
                color:"transparent"
                height: Style.containerSize
                width: Style.containerSize
                radius: 20

            Button {
                id:searchbtn
                anchors.fill: parent
                Layout.fillHeight: true
                width: Style.uiContainerSize

                background: Rectangle{
                    radius:20
                    color: Style.backgroundColor
                }
                Layout.leftMargin: 3

                icon{
                    source: "qrc:/Resources/search.png"
                    width: Style.uiBtnIconSize
                    height: Style.uiBtnIconSize
                }
                onClicked: {
                    textonFocus.running =true
                    closeRect.visible = true
                    txtid.focus = true

                }
            }

            }


            TextField {
                id : txtid
                implicitWidth : 0
                Layout.fillHeight: true
                Layout.bottomMargin: 10

                property  color colorDefault       : Style.backgroundColor
                property  color colorOnFocus       : Style.secondaryColor
                property  color colorMouseOver     : Style.textHoverColor
                property  color colorBorderOnFocus : Style.hoverColor

                placeholderText:  qsTr("Search By" )
                color: "black"
                font.family:closeButton.font.family
                //                    "Segoe UI"
                font.pointSize: 8
                clip: true
                selectByMouse: true
                selectedTextColor: Style.backgroundColor
                selectionColor: "#ffcc00"
                placeholderTextColor: "#81848c"
                background: Rectangle{
//                    color: Style.backgroundColor
                    color: Style.backgroundColor
                    opacity: 0.3
                }
                onTextChanged: {
                    rootItem.model.setFilterString(text)
                }

                PropertyAnimation {
                    id : textonFocus
                    target: txtid
                    properties: "implicitWidth"
                    to:200
                    duration: 150
                    easing.type: Easing.OutQuint
                }
                PropertyAnimation {
                    id : textlostFocus
                    target: txtid
                    properties: "implicitWidth"
                    to:0
                    duration: 150
                    easing.type: Easing.OutQuint
                    onFinished: {
                        closeRect.visible = false
                    }
                }
            }
            // close botton ------------------------------------------------------
            Rectangle {

                id: closeRect
//                color: Style.backgroundColor
                color:"transparent"
                height: Style.uiContainerSize
                width: Style.uiContainerSize
                radius: 20
                visible: false
                Layout.bottomMargin: 10
                Layout.leftMargin: -10
                Button {
                    id: closeButton
                    width: Style.uiContainerSize
                    height: Style.uiContainerSize
                    clip: true
                    background: Rectangle{
                        color: Style.backgroundColor
                        radius: 20
                    }
                    anchors.centerIn: parent

                    icon{
                        source: "qrc:/Resources/index.png"
                        width: Style.uiBtnIconSize
                        height: Style.uiBtnIconSize
                    }
                    onClicked: {
                        textlostFocus.running =true
                        txtid.clear()
                    }
                }
            }
        }
    }
    //-------------------- search list ----------------------//
    Rectangle{
        id:dropDown
        color: Style.backgroundColor
        visible:true
        width: closeButton.visible ? rootItem.width - 6: 0
        height:Math.min(bt.count *35, 200);
        clip:true
        anchors.top:  rootItem.bottom;
        anchors.left: rootItem.left
        anchors.topMargin: 2
        anchors.leftMargin: 2
        radius: 12
        ListView{
            id:bt
            anchors.fill: parent
            model: rootItem.model
            delegate: Button
            {
                width: bt.width
                height: 35
                text: model.display
                contentItem: Text {
                        text: textnew.text
                        font: textnew.font
//                        opacity: enabled ? 1.0 : 0.3
                        color: textnew.down ? "yellow" : "grey"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                id:textnew
                hoverEnabled: true
                onClicked:{
                    rootItem.model.onNodeClicked(index)
                }
                background: Rectangle
                {
                    color: parent.hovered ? Style.hoverColor : "transparent"
                    radius:12
                }
            }
        }
    }
}



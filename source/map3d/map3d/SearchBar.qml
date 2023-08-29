import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "style"


Rectangle {
    id: rootItem
    property var model

    width: 40 + txtid.implicitWidth + (closeButton.visible? closeButton.width : 0)
    height: 40
    radius: 20
//    color: Style.backgroundColor
    color:"white"
    opacity: 0.85
    ColumnLayout{

        anchors.fill: parent
        //---------------------------searcbox---------------------------//
        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true
            //            Layout.rightMargin: 100
            layoutDirection: Qt.LeftToRight

            //------------------------------search-----------------------//
            Rectangle{
                color:"grey"
                height: 40
                width: 40
                radius: 20

            Button {
                id:searchbtn
                anchors.fill: parent
                Layout.fillHeight: true
                width: 40

                background: Rectangle{
                    radius:20
                    color: "white"
                }
                Layout.leftMargin: 3

                icon{
                    source: "qrc:/Resources/search.png"
                    width: 26
                    height: 26
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

                property  color colorDefault       : Style.backgroundColor
                property  color colorOnFocus       : Style.secondaryColor
                property  color colorMouseOver     : Style.textHoverColor
                property  color colorBorderOnFocus : Style.hoverColor

                placeholderText:  qsTr("Search By" )
                color: "black"
                font.family:closeButton.font.family
                //                    "Segoe UI"
                font.pointSize: 10
                clip: true
                selectByMouse: true
                selectedTextColor: "white"
                selectionColor: "#ffcc00"
                placeholderTextColor: "#81848c"
                background: Rectangle{
//                    color: Style.backgroundColor
                    color: "white"
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
//                MultiEffect{
//                    source: closeRect
//                    anchors.fill: closeRect
//                    //                                autoPaddingEnabled: false
//                    paddingRect: Qt.rect(0, 0 , 40, 40)
//                    shadowBlur: 1.0
//                    shadowColor: 'black'
//                    shadowEnabled: true
//                    shadowHorizontalOffset: -8

//                }
                id: closeRect
//                color: Style.backgroundColor
                color:"grey"
                height: 40
                width: 40
                radius: 20
                visible: false
                Layout.leftMargin: -5
                Button {
                    id: closeButton
                    width: 40
                    height: 40
                    clip: true
                    background: Rectangle{
                        color: "white"
                        radius: 20
                    }
                    anchors.centerIn: parent

                    icon{
                        source: "qrc:/Resources/index.png"
                        width: 26
                        height: 26

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
                id:textnew
                hoverEnabled: true
                onClicked:{
                    rootItem.model.onNodeClicked(index)
                }
                background: Rectangle
                {
                    color: parent.hovered ? Style.hoverColor : "transparent"
                }
            }
        }
    }
}



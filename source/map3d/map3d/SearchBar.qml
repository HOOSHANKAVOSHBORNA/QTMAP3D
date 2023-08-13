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
    color: "#edf3f9"
    ColumnLayout{

        anchors.fill: parent
        //---------------------------searcbox---------------------------//
        RowLayout{
            Layout.fillHeight: true
            Layout.fillWidth: true
            //            Layout.rightMargin: 100
            layoutDirection: Qt.LeftToRight

            //------------------------------search-----------------------//
            Button {
                id:searchbtn
                Layout.fillHeight: true
                width: 40
                background: Rectangle{
                    color: "transparent"
                }
                Layout.leftMargin: 2

                icon{
                    source: "qrc:/Resources/search-icon.png"
                    width: 26
                    height: 26
                }
                onClicked: {
                    textonFocus.running =true
                    closeRect.visible = true
                    txtid.focus = true

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
                selectedTextColor: "black"
                selectionColor: "#ffcc00"
                placeholderTextColor: "#81848c"
                background: Rectangle{
                    color: "#edf3f9"
                }
                onTextChanged: {
                    rootItem.model.setFilterString(text)
                }



                onFocusChanged: {

                    if(!txtid.focus){
//                        closeButton.clicked()
                    }
                }

                PropertyAnimation {
                    id : textonFocus
                    target: txtid
                    properties: "implicitWidth"
                    to:300
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
                color: "#edf3f9"
                height: 40
                width: 40
                radius: 20
                visible: false
                Layout.leftMargin: -5
                //                border.color: "black"
                //                border.width: 1
                Button {
                    id: closeButton
//                    visible: false
                    width: 40
                    clip: true
                    background: Rectangle{
                        color: "transparent"
                        radius: 20
                    }
                    anchors.centerIn: parent

                    icon{
                        source: "qrc:/Resources/close-icon.png"
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
        color:"#edf3f9"
        visible:true
        width: rootItem.width-searchbtn.width - 39
        height:Math.min(bt.count *30, 200);
        clip:true;
        border.color: "#edf3f9"
        anchors.top:  rootItem.bottom;
        anchors.left: rootItem.left
        anchors.leftMargin: 41
        ListView{
            id:bt
            anchors.fill: parent
            model: rootItem.model

            delegate: Button
            {
                Layout.fillWidth: true
                text: "Ok"
                height: 30
                id:textnew
                onClicked:{}
                contentItem: Text
                {
                    text: model.display
                    color:"black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle
                {
                    id :recId
                    color: "#edf3f9"
                }
            }
        }
    }
}



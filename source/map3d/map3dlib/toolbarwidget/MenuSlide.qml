import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Rectangle{
    property bool valuePin: false
    property alias getstart: open
    property alias getstart1: close
    property int space: 3
    height: 420
    width: 200
    id:menu
    x:0
    color: "transparent"
    //state: "open"
    Connections{
        target: NamePlugin
        onClose:{
            if (!valuepin && back.state === "open"){
                close.start()
                widgetrack.state = "close"
            }

        }
    }
    Rectangle{
        id: back
        height: 23
        color: "transparent"
        anchors.top:menu.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 3
        anchors.rightMargin: 3
        state: "close"
        Rectangle{
            id:backmenu
            anchors.left: parent.left
            anchors.leftMargin: 1
            width: 30
            height: 30
            anchors.top: parent.top
            anchors.topMargin: 3
            radius: 100
            color: "#88000000"
            MouseArea{
                anchors.fill: parent

                onClicked: {
                    if (back.state==="open"){
                        close.start()
                        back.state="close"
                        NamePlugin.setSizeWidget(false)
                    }else
                    {
                        open.start()
                        back.state="open"

                        pin.visible =true
                        back.color = "#282A31"
                        backmenu.color = "transparent"
                        backmenu.anchors.topMargin =-4
                        backmenu.anchors.leftMargin= -5
                        NamePlugin.setSizeWidget(true)

                    }
                }
            }
            Image {
                id: iconname
                source: "qrc:/res/icon/toolbarmenu.png"
                width: 20
                height: 20
                anchors.centerIn: parent

            }
        }
        Image {
            id: pin
            visible: false
            source: "qrc:/res/icon/unpin.png"
            width: 15
            height: 15
            anchors.right: parent.right
            anchors.rightMargin: 3
            anchors.verticalCenter: parent.verticalCenter
            state: "setpin"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if (pin.state ==="setpin"){
                        pin.source = "qrc:/res/icon/pin.png"
                        pin.state = "setunpin"
                        valuePin = true
                        backmenu.enabled =false
                        NamePlugin.onPin(valuePin)

                    }else{
                        pin.source = "qrc:/res/icon/unpin.png"
                        pin.state = "setpin"
                        valuePin = false
                        backmenu.enabled =true
                        NamePlugin.onPin(valuePin)
                        NamePlugin.setSizeWidget(true)
                    }

                }
            }
        }
    }
    NumberAnimation{
        id:open
        target: toolbar
        property: "height"
        from:0
        to:400
        duration: 200

    }

    NumberAnimation{
        id:close
        target: toolbar
        property: "height"
        from:400
        to:0
        duration: 200
        onStopped: {
            backmenu.color = "#88000000"
            backmenu.anchors.topMargin = 3
            backmenu.anchors.leftMargin = 1
            back.rotation=0

            pin.visible =false
            back.color = "transparent"
        }

    }
    Rectangle{
        id:toolbar
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.top: back.bottom
        anchors.topMargin: 0
        clip : true
        color: "transparent"
        height: 0

        ScrollView {
            id :laout_back
            anchors.fill: parent


            Column{
                id:rootlayer
                anchors.fill: parent

                spacing: 0

                PanelItem{
                    title: "File"
                    width: laout_back.width
                    content:Panelfile{}
                }

                PanelItem {
                    title: "Draw"
                    width: laout_back.width
                    content:Paneldraw{}
                }
                PanelItem {
                    title: "Measures"
                    width: laout_back.width
                    content:Panelmeasure{}
                }
                PanelItem {
                    title: "Image"
                    width: laout_back.width
                    content:Panelimage{}
                }
                PanelItem {
                    title: "Terrain"
                    width: laout_back.width
                    content:Panelterrain{}
                }

                PanelItem {
                    title: "Model"
                    width: laout_back.width
                    content:Panelmodel{}
                }
                PanelItem {
                    title: "Feature"
                    width: laout_back.width
                    content:Panelfeature{}
                }
                PanelItem{
                    title: "Analyze"
                    width: laout_back.width
                    content:Panelanalyze{}
                }
                PanelItem{
                    title: "Effect"
                    width: laout_back.width
                    content:Paneleffect{}
                }

            }


        }

    }
}



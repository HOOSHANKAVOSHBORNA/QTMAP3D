import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
Item {
    id: mainroot
    readonly property int iconzie: 20
    readonly property int textsize: 10
    readonly property int iconsize2: 15
    property bool valuepin: false
    property var object:[]
    property int i: 0
    width: 200
    height: 270
    Connections{
        target: DetaliObject
        onChangeSize:{

            if (t){

                mainroot.height = 270
                mainroot.width  = 200

            }else{
                mainroot.height = 40
                mainroot.width  = 200

            }
        }
        onClose:{
            if (!valuepin && widgetrack.state === "open"){
                close.start()
                widgetrack.state = "close"
            }else if(backmenu.enabled){
                mainroot.height = 40
                mainroot.width  = 40
            }

        }
        onIsDock:{
            toolbar.height = height - 10
        }
        onModelAdded:{
            var component = Qt.createComponent("qrc:/trackwidget/PanelItem.qml");
            object[i] = component.createObject(rootlayer);
            object[i].title= type+ " : "+ name
            object[i].name = name
            object[i].type = type
            object[i].width= laout_back.width
            i+=1
        }
        onMinimize:{
            if (isMax){
                open.start()
                widgetrack.state = "open"
                bar.color = "#282A31"
                iconpin.visible = true
                backmenu.color = "transparent"
                backmenu.anchors.topMargin =-4
                backmenu.anchors.rightMargin =-5
                DetaliObject.changeSize(isMax)
                backmenu.radius= 100
                widgetrack.height= parent.height - bar.height

            }else{
                close.start()
                widgetrack.state = "close"
                bar.color = "transparent"
                iconpin.visible = false
                backmenu.color = "#88000000"
                backmenu.anchors.topMargin =1
                backmenu.anchors.rightMargin = 3
                DetaliObject.setChangeSize(isMax)
                widgetrack.height=0
                backmenu.radius =100
            }
        }
        onOnPin:{
            if (t){
                mainroot.height = parent.height

            }else{
                open.start()
                widgetrack.state = "open"
            }
        }

    }
    Rectangle{
        anchors.fill: parent
        id:root
        color: "transparent"


        Rectangle{
            id:bar
            height: 23
            width: parent.width
            anchors.top: root.top
            color: "#282A31"//
            Rectangle{
                id : backmenu
                width:30
                height: 30
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: -3
                color: "transparent"
                radius: 100
                //anchors.verticalCenter: parent.verticalCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        if (widgetrack.state === "close"){
                            open.start()
                            widgetrack.state = "open"
                            bar.color = "#282A31"

                            iconpin.visible = true
                            backmenu.color = "transparent"
                            backmenu.anchors.topMargin =-4
                            backmenu.anchors.rightMargin =-5
                            DetaliObject.changeSize(true)

                        }else{
                            close.start()
                            widgetrack.state = "close"


                        }
                    }
                }
                Image {
                    id: iconmenu
                    source: "qrc:/res/toolbarmenu.png"
                    anchors.centerIn: parent
                    width:iconzie
                    height: iconzie

                }
            }
            Image {
                id: iconpin
                source: "qrc:/res/unpin.png"
                width:iconsize2
                height: iconsize2
                visible: false
                anchors.left: parent.left
                anchors.leftMargin: 3
                anchors.verticalCenter: parent.verticalCenter
                state: "setpin"
                MouseArea{
                    anchors.fill :parent
                    onClicked: {
                        if (iconpin.state ==="setpin"){
                            iconpin.source = "qrc:/res/pin.png"
                            iconpin.state = "setunpin"
                            valuepin = true
                            backmenu.enabled = false
                            DetaliObject.onPin(valuepin)

                        }else{
                            iconpin.source = "qrc:/res/unpin.png"
                            iconpin.state = "setpin"
                            valuepin = false
                            backmenu.enabled =true
                            DetaliObject.onPin(valuepin)

                        }

                    }
                }
            }

        }
        NumberAnimation{
            id:open
            alwaysRunToEnd: true
            target: widgetrack
            property: "height"
            from:0
            to:270
            duration: 200
        }

        NumberAnimation{
            id:close
            alwaysRunToEnd: true
            target: widgetrack
            property: "height"
            from:270
            to:0
            duration: 200
            onStopped: {
                bar.color = "transparent"
                iconpin.visible = false
                backmenu.color = "#88000000"
                backmenu.anchors.topMargin =3
                backmenu.anchors.rightMargin = 3
                DetaliObject.changeSize(false)

            }

        }
        Rectangle{
            id :widgetrack

            width: parent.width
            clip: true

            anchors.top: bar.bottom
            color: "#282A31"
            border.color: "#282A31"
            border.width: 1
            ScrollView {
                id :laout_back
                anchors.fill: parent

                Column{
                    id:rootlayer
                    anchors.fill: parent

                    spacing: 3

                }
            }
        }
    }
}

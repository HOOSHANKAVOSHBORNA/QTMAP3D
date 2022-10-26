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
    property var object
    property var listobject: []

    width: 200
    height: 300
    Connections{
        target: DetaliObject
        onChangeSize:{

            if (t){

                mainroot.height = 300
                mainroot.width  = 200

            }else{
                mainroot.height = 40
                mainroot.width  = 200

            }
        }
        onUnTrackAll:{
            for (var i in listobject){
                if (listobject[i].selected ){
                    listobject[i].children[0].color = "#404142"
                    listobject[i].selected = false
                    DetaliObject.onModelClicked(listobject[i].type,listobject[i].name , false)
                }

            }
        }

        onClose:{


            if (!valuepin && widgetrack.state === "open"){
                close.start()
                widgetrack.state = "close"
            }else if(backmenu.enabled && widgetrack.state == "open"){
                mainroot.height = 40
                mainroot.width  = 40
            }


        }
        onIsDock:{
            widgetrack.height = height - 50
            mainroot.height  = height - 50

        }
        onModelRemove:{
            for(var i in listobject){
                if (listobject[i].name === name && listobject[i].type === type){
                    listobject[i].destroy()
                      listobject=Array.from(listobject).filter(r => r !== listobject[i])
                    break;
                }

        }
        }
        onModelAdded:{
            var component = Qt.createComponent("PanelItem.qml");
            object = component.createObject(rootlayer);
            object.title= type+ " : "+ name
            object.name = name
            object.type = type
            object.width= laout_back.width
            listobject.push(object)
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
            color: "#88000000"//
            Rectangle{
                id : backmenu
                width:30
                height: 30
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: -3
                color: "transparent"
                radius: 100
                anchors.leftMargin: 0
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
                    source: "qrc:/res/tracking.png"
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
                anchors.right: parent.right
                anchors.rightMargin: 3
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
            radius: 5
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

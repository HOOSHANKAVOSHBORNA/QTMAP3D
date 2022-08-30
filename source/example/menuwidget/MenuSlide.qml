import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
Rectangle{
    Timer {
        id: timer
    }
    //    function delay(delayTime, cb) {
    //        timer.interval = delayTime;
    //        timer.repeat = false;
    //        timer.triggered.connect(cb);
    //        timer.start();
    //    }
    property alias getstart: open
    property alias getstart1: close
    property int space: 3
    id:menu
    x:- width +5
    color: "#77000000"

    border.color : "white"
    border.width: 3
    state: "close"
    Image {
        id: back
        source: "qrc:/res/icons8-chevron-right-96.png"
        width: 20
        height: 40
        rotation: 0
        anchors.rightMargin: -9
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

    }
    MouseArea{
        anchors.right: parent.right
        width: 20
        height: parent.height
        onClicked: {
            if (menu.state==="open"){
                close.start()
                 back.rotation=0
                menu.state="close"
            }else
            {
                open.start()
                back.rotation=180
                menu.state="open"
            }


        }

    }
    NumberAnimation{
        id:open
        target: menu
        property: "x"
        from:- width +5
        to:0
        duration: 200

    }

    NumberAnimation{
        id:close
        target: menu
        property: "x"
        from:0
        to:-width +5
        duration: 200

    }

    Column{
        id :laout_back
        anchors.right: back.left
        anchors.rightMargin: 2
        anchors.left: parent.left
        anchors.leftMargin: 2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        anchors.top: parent.top
        anchors.topMargin: 2

        spacing: 5

        Rectangle{
            id : backtools
            width: parent.width
            height: 110
            border.color: "white"
            border.width: 1
            color: "transparent"
            radius: 5
            Column{
                anchors.fill: parent
                spacing: 0
                Rectangle{
                    width: parent.width
                    height: 25
                    radius: 5
                    color: "#99000000"
                    Text {
                        id: name
                        text: qsTr("Drow")
                        anchors.centerIn: parent
                        color: "white"
                        anchors.margins: 10
                    }
                }
                Rectangle{
                    width: backtools.width
                    height: backtools.height -25
                    color: "#88000000"
                    radius: 5
                    Tools{}
                }

            }
        }
        Rectangle{
            id : backtools1
            width: parent.width
            height: 110
            border.color: "white"
            border.width: 1
            color: "transparent"
            radius: 5
            Column{
                anchors.fill: parent
                spacing: 0
                Rectangle{
                    width: parent.width
                    height: 25
                    radius: 5
                    color: "#99000000"
                    Text {
                        text: qsTr("measurement")
                        anchors.centerIn: parent
                        color: "white"
                        anchors.margins: 10
                    }
                }
                Rectangle{
                    width: backtools.width
                    height: backtools.height -25
                    color: "#88000000"
                    radius: 5
                    Tools1{}
                }

            }
        }

//        PanelItem{
//            width: parent.width
//            title :"Drow"
//            content: Tools{}
//        }

        PanelItem {
            title: "Image"
            width: laout_back.width
            content:Panel1{}
        }
        PanelItem {
            title: "Terrain"
            width: laout_back.width
            content:Panel1{}
        }
        PanelItem {
            title: "Meature"
            width: laout_back.width
            content:Panel1{}
        }
        PanelItem {
            title: "Model"
            width: laout_back.width
            content:Panel1{}
        }

    }





}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

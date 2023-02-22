import QtQuick 2.0

Item {
    id: root
    x: parent.x

    property var nowItem: null
    function showInfo(item){
        if(nowItem)
            hideItem();
        item.parent = root
        item.anchors.top = root.top
        item.anchors.bottom = root.bottom
        nowItem = item
        infoItemHideAnimation.stop();
        infoItemShowAnimation.stop();
        infoItemShowAnimation.target = item;
        infoItemShowAnimation.from = 0;
        infoItemShowAnimation.to = 300 + (widgetsMargins * 2.0);
        infoItemShowAnimation.duration = 200;
        infoItemShowAnimation.start();
        maxmove.running = true
        sidePush.visible = true
    }

    Item {
            anchors.left: root.right
            id: sidePush
            visible: false
    //        Rectangle{
    //            id: siderec
    //            height: 25
    //            width:25
    //            radius: 18
    ////            anchors.left: parent.right
    //            y:10
    //            z: -1
    //            color: "#2F2E2C"
    //            opacity: 0.8


                Image {
                    id: minimize
                    source: "/Resources/sideTab.png"
                    sourceSize: ("25 x 100")
    //                anchors.top: parent.top
    //                anchors.top: parent.top
    //                anchors.horizontalCenter: parent.horizontalCenter
                    y:8
//                    x:-45
                    MouseArea{
                        anchors.fill: parent
                        onClicked: if(root.x !== -620){
                                       minix.running = true
                                       mintxt.running = true
                                   } else{
                                       maxix.running = true
                                       maxtxt.running = true
                                   }
                    }
                    Text {
                        id: sideInfoTxt
                        anchors.centerIn: parent
                        text: "10001"
                        color:"white"
                        font.pointSize: 12
                        visible: false
                        rotation: -90
                    }
                }
            PropertyAnimation {id:maxmove ; target: minimize ; property: "x" ; from :-300 ; to :0; duration: 200 ; easing.type: Easing.OutQuint ;running: false}
            PropertyAnimation {id:minix ;target:root ; property:"x";  to: -620 ; duration: 200 ; running: false}
            PropertyAnimation {id:maxix ;target:root ; property:"x";  to: -300 ; duration: 200 ; running: false}
            PropertyAnimation {id:mintxt; target: sideInfoTxt ; property: "visible" ;  to: true  ; duration: 200 ; running: false}
            PropertyAnimation {id:maxtxt; target: sideInfoTxt ; property: "visible" ;  to: false ; duration: 200 ; running: false}

        }

    PropertyAnimation {
        id: infoItemShowAnimation
        property: 'x'

        easing.type: Easing.OutQuint
    }

    PropertyAnimation {
        id: infoItemHideAnimation
        property: 'x'

        easing.type: Easing.InQuint
    }

    function hideItem(){
        if (nowItem){
            nowItem.parent = null
            nowItem.destroy()
            sidePush.visible = false
        }
    }
}

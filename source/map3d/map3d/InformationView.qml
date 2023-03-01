import QtQuick 2.0
import QtGraphicalEffects 1.13


Item {
    id: root
    x: parent.x

    property string titleText: "airplane"
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


                Image {
                    id: minimize
                    source: "/Resources/sideTab.png"
                    sourceSize: ("25 x 100")
                    y:8
                    MouseArea{
                        id: mousebtn
                        anchors.fill: parent
                        onClicked: if(root.x !== -620){
                                       minix.running = true
                                       mintxt.running = true
                                       minarrow.running = true

                                   } else{
                                       maxix.running = true
                                       maxtxt.running = true
                                       maxarrow.running = true

                                   }
                            hoverEnabled: true
                            onEntered:   if(root.x !== -620){
                                            glowimg.visible=true
                                         }
                            onExited:    glowimg.visible=false



                    }
                    Text {
                        id: sideInfoTxt
                        anchors.centerIn: parent
                        text: root.titleText
                        color:"white"
                        font.pointSize: 7
                        visible: false
                        rotation: -90
                    }

                    Image {
                        id: arrow
                        source: "/Resources/sidearrow.png"
                        sourceSize: ("25 x 25")
                        anchors.centerIn: parent



                    }

                    ColorOverlay {
                        id: glowimg
                        anchors.fill: arrow
                        color: "orange"
                        source: arrow
                        visible: false
                        }
                }
            PropertyAnimation {id:maxmove ; target: minimize ; property: "x" ; from :-300 ; to :0; duration: 200 ; easing.type: Easing.OutQuint ;running: false}
            PropertyAnimation {id:minix   ; target:root ; property:"x";  to: -620 ; duration: 200 ; running: false}
            PropertyAnimation {id:maxix   ; target:root ; property:"x";  to: -300 ; duration: 200 ; running: false}
            PropertyAnimation {id:mintxt  ; target: sideInfoTxt ; property: "visible" ;  to: true  ; duration: 200 ; running: false}
            PropertyAnimation {id:maxtxt  ; target: sideInfoTxt ; property: "visible" ;  to: false ; duration: 200 ; running: false}
            PropertyAnimation {id:minarrow; target: arrow ; property: "opacity" ; to:0 ; duration: 200 ; running: false}
            PropertyAnimation {id:maxarrow; target: arrow ; property: "opacity" ; to:1 ; duration: 200 ; running: false}

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
            sidePush.visible = false
            nowItem.parent = null
            nowItem.destroy()
            sidePush.visible = false

        }
    }
}



import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Effects


Item {
    id: root
    x: parent.x
    clip: false
//    property var m
//    function adjustRightContainer(screenwidth){ if(nowItem || layers.visible){
//            if(isMinimized === true){
//                root.x = screenwidth
//            }else{
//                root.x = screenwidth + widgetsMargins - (wnd.widgetsPositionFactor * (250 + (widgetsMargins *3)))
//            }
//        }
//    }


    property bool isMinimized: false
    property var nowItem: null
    function showProp(item){
        item.parent = leftObjects
        nowItem = item
        root.visible = true
        menuRightContainerLoad();
        leftObjects.visible = true;
    }

    function hideProp(item){
        if (nowItem){
            leftObjects.visible = false
            nowItem.parent = null
            nowItem = null
                menuRightContainerHide();
            leftObjects.visible = false
        }
    }


    function menuRightContainerLoad() {
        layersItemShowAnimation.to =  -300
        layersItemShowAnimation.duration = 200;
        layersItemShowAnimation.start();
    }

    function menuRightContainerHide(){
        layersItemHideAnimation.to = 0
        layersItemHideAnimation.duration = 200;
        layersItemHideAnimation.start();
    }

    function menuLayersButtonClicked(){
        if(layers.visible !== true){
            menuRightContainerLoad();
            layers.visible = true;
            root.visible = true
        }
        else if(nowItem !== null){
            layers.visible = false;
        }
        else {
            layers.visible = false;
            menuRightContainerHide();
        }
    }

    function minimizeBtn(){
        if(containerHolder.x === -300){
            miniLayer.running = true
            miniarrow.running = true
            isMinimized = true
            //                               maxiTXT.running = true
        } else{
            maxiLayer.running = true
            maxiarrow.running = true
            isMinimized = false
            //                               miniTXT.running = true
        }
    }


    Rectangle{
        id: containerHolder
        color: "black"
        opacity: 0.7
        width: parent.width
        height: parent.height
        radius: 10
        visible: true
        clip: false
    }

    SplitView{
        anchors.fill:containerHolder
        orientation: Qt.Vertical

//        LayersWidget{
//            id:layers
//            layersModel: root.m
//            SplitView.minimumHeight: root.height/4
//            onToggleLayerEnabled: function(layerIndex) {
//                wnd.toggleLayerEnabled(layerIndex);
//            }
//            visible: false
//        }

        Item {
            id: leftObjects
            width: parent.width
            SplitView.minimumHeight: root.height/4
            visible: true

        }
    }









    //        ////////////////////////minimize//////////////////////////////////
    Item {
        anchors.right: containerHolder.left
        id: sidePush
        visible: true
        Image {
            id: minimize
            source: "/Resources/sideTab.png"
            sourceSize: ("25 x 100")
            y:8
            anchors.right:  parent.left
            rotation: 180
            MouseArea{
                anchors.fill: parent
                onClicked: minimizeBtn();
                hoverEnabled: true
                onEntered:  if(containerHolder.x === -300){
                                glowimg.colorization=1
                                glowimg.visible = true
                            }else{
                                glowimg.colorization =1
                                glowimg.visible = true
                                glowimg.rotation = 180
                            }

                onExited:   if(containerHolder.x === -300){
                                glowimg.colorization=0
                                glowimg.visible = false
                            }else{
                                glowimg.colorization =0
                                glowimg.visible = false
                                glowimg.rotation = 0
                            }
            }
            Text {
                id: sideInfoTxt
                anchors.centerIn: parent
                text: "Layers"
                color:"white"
                font.pointSize: 8
                visible: false
                rotation: 90
            }
            Image {
                id: arrow
                source: "/Resources/sidearrow.png"
                sourceSize: ("25 x 25")
                anchors.centerIn: parent
                rotation: 0

            }
            MultiEffect {
                id: glowimg
                anchors.fill: arrow
                colorization: 0
                source: arrow
                colorizationColor: "orange"
                visible: false
            }
        }
        PropertyAnimation{id:miniLayer ; target: containerHolder ; property: "x" ; to: -40 ; duration:150;  running: false }
        PropertyAnimation{id:maxiLayer ; target: containerHolder ; property: "x" ; to: -300 ; duration: 150; running: false }
        PropertyAnimation{id:miniarrow ; target: arrow ; property: "rotation" ; to: 180 ; duration:300;  running: false ;easing.type: Easing.InQuint}
        PropertyAnimation{id:maxiarrow ; target: arrow ; property: "rotation" ; to: 0 ; duration:300;  running: false  ; easing.type: Easing.InQuint}
        PropertyAnimation{id: layersItemShowAnimation ; target: containerHolder;property: 'x';easing.type: Easing.OutQuint;}
        PropertyAnimation{id: layersItemHideAnimation ; target: containerHolder;property: 'x';easing.type: Easing.InQuint}
    }
}






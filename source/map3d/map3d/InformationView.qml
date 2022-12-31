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
        nowItem.parent = null
        nowItem.destroy()
    }
}

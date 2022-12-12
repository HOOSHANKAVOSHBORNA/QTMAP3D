
import QtQuick 2.13

Item {
    id: rootItem
    property ListModel sideItemsModel
    property int currentVisibleIndex: -1

    signal sideItemCreated(int index, Item item)
    implicitWidth: 600

    function hideAllItems() {
        for (var i = 0; i < rootItem.sideItemsModel.count; i++) {
            sideItemsRepeater.itemAt(i).x = 0;
        }
    }


    function menuWidgetItemClicked(index) {

        sideItemHideAnimation.stop();
        sideItemShowAnimation.stop();

        for (var i = 0; i < rootItem.sideItemsModel.count; i++) {
            if(i !== index) {
                sideItemsRepeater.itemAt(i).x = 0;
            }
        }

        if (index == rootItem.currentVisibleIndex) {
            sideItemHideAnimation.target = sideItemsRepeater.itemAt(index);
            sideItemHideAnimation.from = 300 + (widgetsMargins * 2.0);
            sideItemHideAnimation.to = 0;
            sideItemHideAnimation.duration = 200;
            rootItem.currentVisibleIndex = -1;
            sideItemHideAnimation.start();
        } else {
            sideItemShowAnimation.target = sideItemsRepeater.itemAt(index);
            sideItemShowAnimation.from = 0;
            sideItemShowAnimation.to = 300 + (widgetsMargins * 2.0);
            sideItemShowAnimation.duration = 200;
            rootItem.currentVisibleIndex = index;
            sideItemShowAnimation.start();
        }
    }


    PropertyAnimation {
        id: sideItemShowAnimation
        property: 'x'

        easing.type: Easing.OutQuint
    }
    PropertyAnimation {
        id: sideItemHideAnimation
        property: 'x'

        easing.type: Easing.InQuint
    }


    Repeater {
        id: sideItemsRepeater
        model: rootItem.sideItemsModel
        delegate: Item {
            anchors.top:parent.top
            anchors.bottom: parent.bottom
            x: 0
            width: 300

//            Rectangle {
//                anchors.fill: parent
//                color: "#404040"
//                opacity: 0.8
//                radius: 10
//            }

            Loader {
                anchors.fill: parent
                source: side_itemurl
                onLoaded: function() {
                    rootItem.sideItemCreated(index, item);
//                    wnd.sideItemCreated(index, item);

//                    if (index == 2) {
//                        item.toolboxModel = toolboxModel;
//                    }
                }
            }
        }
    }

}

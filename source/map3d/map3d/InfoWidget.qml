
import QtQuick 2.13
import QtQuick.Controls 2.13

Item {
    id: rootItem
    implicitWidth: 600


    property int currentShowingIndex: -1


    function hideAllItems() {
        for (var i = 0; i < infoItemsModel.count; i++) {
            infoItemsRepeater.itemAt(i).x = 0;
        }
    }

    function showInfoItem(itemTypeString) {

        infoItemHideAnimation.stop();
        infoItemShowAnimation.stop();


        rootItem.currentShowingIndex = -1;

        const modelCount = infoItemsModel.count;
        for (var i = 0; i < modelCount; i++) {
            if (infoItemsModel.get(i).typeString === itemTypeString) {
                rootItem.currentShowingIndex = i;
                break;
            }
        }

        if (rootItem.currentShowingIndex !== -1) {
            for (var i = 0; i < modelCount; i++) {
                infoItemsRepeater.itemAt(i).x = 0;
            }

            infoItemShowAnimation.target = infoItemsRepeater.itemAt(rootItem.currentShowingIndex);
            infoItemShowAnimation.from = 0;
            infoItemShowAnimation.to = 300 + (widgetsMargins * 2.0);
            infoItemShowAnimation.duration = 200;
            infoItemShowAnimation.start();
        }
    }

    function updateData(infoJSON) {
        var jsonObject = JSON.parse(infoJSON);

        if (rootItem.currentShowingIndex !== -1) {
           switch(infoItemsModel[rootItem.currentShowingIndex].typeString) {
           case "AIRPLANE":

               break;
           case "STATION":

               break;
           case "SYSTEM":

               break;
           }
        }
    }

    ListModel {
        id: infoItemsModel

        ListElement {
            typeString: "AIRPLANE"
            itemUrl: "qrc:///infowidget/AirplaneInfo.qml"
        }

        ListElement {
            typeString: "STATION"
            itemUrl: "qrc:///infowidget/StationInfo.qml"
        }

        ListElement {
            typeString: "SYSTEM"
            itemUrl: "qrc:///infowidget/SystemInfo.qml"
        }

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

    Repeater {
        id: infoItemsRepeater
        model: infoItemsModel

        delegate: Item {
            anchors.top:parent.top
            anchors.bottom: parent.bottom
            x: 0
            width: 300


            Loader {
                anchors.fill: parent
                source: itemUrl
            }
        }
    }

}

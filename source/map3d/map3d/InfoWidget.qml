
import QtQuick 2.13
import QtQuick.Controls 2.13

Item {
    id: rootItem
    implicitWidth: 600


    property int currentShowingIndex: -1

    function showInfoItem(itemTypeString) {

        rootItem.currentShowingIndex = -1;

        var modelCount = infoItemsModel.count;
        for (var i = 0; i < modelCount; i++) {
            if (infoItemsModel[rootItem.currentShowingIndex].typeString === itemTypeString) {
                rootItem.currentShowingIndex = i;
                break;
            }
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
        }

        ListElement {
            typeString: "STATION"
        }

        ListElement {
            typeString: "SYSTEM"
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
        id: infoItemsRepeater
        model: rootItem.infoItemsModel
        delegate: Item {

            Loader {
                anchors.fill: parent
                onLoaded: function() {

                }
            }
        }
    }



}

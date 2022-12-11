
import QtQuick 2.13

Item {
    id: rootItem
    property ListModel infoItemsModel

    signal infoItemCreated()

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


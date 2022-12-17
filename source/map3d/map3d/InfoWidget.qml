
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import "./infowidget"

Item {
    id: rootItem
    implicitWidth: 600


    signal view2D3DButtonClicked();
    signal routeButtonClicked();
    signal followButtonClicked();
    signal moreButtonClicked();




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
        //console.log(infoJSON);
        var jsonObject = JSON.parse(infoJSON);

        if (rootItem.currentShowingIndex !== -1) {
            infoItemsRepeater.itemAt(rootItem.currentShowingIndex).updateData(jsonObject);
        }
    }

    ListModel {
        id: infoItemsModel

        ListElement {
            typeString: "Airplane"
            itemUrl: "qrc:///infowidget/AirplaneInfo.qml"
        }

        ListElement {
            typeString: "Station"
            itemUrl: "qrc:///infowidget/StationInfo.qml"
        }

        ListElement {
            typeString: "System"
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
            id: delegItem
            anchors.top:parent.top
            anchors.bottom: parent.bottom
            x: 0
            width: 300

            property Item infoItem

            function updateData(jsonObject) {
                if (delegItem.infoItem) {
                    delegItem.infoItem.updateData(jsonObject);
                }
            }

            Rectangle{
                anchors.fill: parent
                color: "#404040"
                opacity: 0.8
                radius: 10
            }

            ColumnLayout {
                anchors.fill: parent

                spacing: 0
                Item {
                    Layout.fillWidth: true
                    Layout.minimumHeight: 60

                    Text {
                        anchors.centerIn: parent
                        text: typeString
                        color:"yellow"
                        font.pointSize: 20
                    }

                }
                Loader {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    source: itemUrl
                    onLoaded: function() {
                        delegItem.infoItem = item;
                    }
                }
                Item {
                    Layout.minimumHeight: 5
                }

                InfoButtonGroup {
                    Layout.fillWidth: true
                    Layout.minimumHeight: 60

                    onView2D3DButtonClicked: function() {
                        rootItem.view2D3DButtonClicked();
                    }
                    onRouteButtonClicked: function() {
                        rootItem.routeButtonClicked();
                    }
                    onFollowButtonClicked: function() {
                        rootItem.followButtonClicked();
                    }
                    onMoreButtonClicked: function() {
                        rootItem.moreButtonClicked();
                    }


                }
            }
        }
    }
}

import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls.Material 2.12
import QtQuick.Effects

import "style"

Item {
    id: mainItem

    //--properties--------------------------------------
    property var mainPageCpp
    property var mapItem

    property bool listWindowVisible: false

    //--------------------------------------------------
    // visible: true
    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    property string modeMap: "geocentric"
    readonly property color _colorHover: "#01AED6"
    readonly property color _colorPresed: "#003569"

    Rectangle {
        anchors.fill: parent

        Item {
            id: unPinContainer
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 5 / Style.monitorRatio
            z: 1
            width: sideBar.minWidth
            height: mainItem.height - (25 + 10) / Style.monitorRatio

            SideBar {
                id: sideBar

                anchors.fill: parent

                sideContainer.locationCpp: mainItem.mainPageCpp.getLocationManager()
                sideContainer.toolboxCpp: mainItem.mainPageCpp.getToolboxManager()
                sideContainer.layerCpp: mainItem.mainPageCpp.getLayerManager()
                sideContainer.bookmarkCpp: mainItem.mainPageCpp.getBookmarkManager()

                listWindowVisible: mainItem.listWindowVisible

                onListWindowVisibleChanged: mainItem.listWindowVisible = listWindowVisible

                //                pin: true
                onPinChanged: {
                    if (pin) {
                        parent = pinContainer
                        pinContainer.visible = true
                        unPinContainer.visible = false
                    } else {
                        parent = unPinContainer
                        pinContainer.visible = false
                        unPinContainer.visible = true
                    }
                }
            }
        }

        SplitView {
            id: mainItemSplitter
            anchors.fill: parent

            Item {
                id: pinContainer
                SplitView.preferredWidth: sideBar.minWidth
                SplitView.minimumWidth: sideBar.minWidth
                SplitView.maximumWidth: sideBar.minWidth
                visible: false
            }

            MapControllerItem {
                mapItem: mainItem.mapItem
                SplitView.fillWidth: true
                SplitView.fillHeight: true
            }
        }
    }

    ListWindow {
        visible: mainItem.listWindowVisible
    }
}

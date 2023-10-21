import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Crystal
import "style"

ColumnLayout {
    id: rootItem

    property ListModel sideModel: ListModel{}
    property alias currentItemIndex: tabBar.currentIndex
    property var names: {0: "Toolbox", 1: "LocationManager", 2: "Layers", 3: "Bookmark"}
    property var indexes: {"Toolbox": 0, "LocationManager": 1, "Layers": 2, "Bookmark": 3}

    signal modelEmpty

    function isInModel(objectName) {
        for (var i = 0; i < sideModel.count; i++) {
            if (sideModel.get(i).name === objectName)
                return i
        }

        return -1
    }

    function toggleItem(itemName) {
        var index = indexes[itemName]

        var i = isInModel(itemName)
        if (i !== -1) {
            sideModel.remove(i)
            if (sideModel.count === 0) modelEmpty()
            currentItemIndex = 0
            stackLayout.currentIndex = indexes[sideModel.get(0).name]
        } else {
            sideModel.append({name: itemName})
            currentItemIndex = sideModel.count - 1
            stackLayout.currentIndex = index
        }

        modelsInitialize();
    }

    function modelsInitialize() {
        toolbox.listModel = ToolboxInstance
        locationManager.listModel = LocatoinManagerInstance
        layers.layersModell = LayersInstance
        bookmark.model = BookmarkInstance
    }



    TabBar {
        id: tabBar

        contentWidth: rootItem.sideModel.count ? parent.width - 40 / Style.monitorRatio : 0
        Layout.leftMargin: rootItem.sideModel.count ? 18 / Style.monitorRatio : 0
        Material.accent: Style.foregroundColor

        background: Rectangle {
            color: Style.disableColor
            height: 2
            anchors.bottom: parent.bottom
        }

        Repeater {
            id: repeater

            model: rootItem.sideModel

            TabButton {
                id: tb

                width: tabBar.width / rootItem.sideModel.count

                contentItem: Text {
                    id: txt

                    text: name ?? "unknown"
                    font: Style.fontFamily
                    opacity: enabled ? 1.0 : 0.3
                    color: tabBar.currentIndex === index ? Style.foregroundColor : Style.disableColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    stackLayout.currentIndex = indexes[sideModel.get(currentItemIndex).name]
                }
            }
        }
    }

    StackLayout {
        id: stackLayout
        Layout.fillHeight: true
        currentIndex: 0

        ToolboxView { id: toolbox }
        LocationManager { id: locationManager }
        LayersWidget { id: layers }
        BookmarkItem { id: bookmark }
    }
}

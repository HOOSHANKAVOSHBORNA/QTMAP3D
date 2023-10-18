import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Crystal
import "style"

ColumnLayout {
    id: rootItem

    property ListModel model: ListModel{}
    property alias isOpen: stackLayout.visible
    property alias currentItemIndex: tabBar.currentIndex
    property int previousItemIndex: -1

    signal modelEmpty

    function isInModel(objectName) {
        for (var i = 0; i < model.count; i++) {
            if (model.get(i).name === objectName)
                return i
        }

        return -1
    }

    function toggleStackLayoutIndex(index) {
        if (isOpen && currentItemIndex === index) {
            if (previousItemIndex !== -1) {
                currentItemIndex = previousItemIndex
            } else {
                isOpen = false
            }
        } else {
            if (!isOpen) {
                isOpen = true
            }
            previousItemIndex = currentItemIndex
            currentItemIndex = index
        }
    }

    function toggleToolbox() {
        var i = isInModel("Toolbox")
        if (i !== -1) {
            model.remove(i)
            if (model.count === 0) modelEmpty()
        } else {
            model.append({name: "Toolbox"})

        }

        toggleStackLayoutIndex(0)
        toolbox.listModel = ToolboxInstance
    }

    function toggleLocationManager() {
        var i = isInModel("LocationManager");
        if (i !== -1) {
            model.remove(i)
            if (model.count === 0) modelEmpty()
        } else {
            model.append({name: "LocationManager"})
        }

        toggleStackLayoutIndex(1)
        locationManager.listModel = LocatoinManagerInstance
    }

    function toggleLayers() {
        var i = isInModel("Layers");
        if (i !== -1) {
            model.remove(i)
            if (model.count === 0) modelEmpty()
        } else {
            model.append({name: "Layers"})
        }

        toggleStackLayoutIndex(2)
        layers.layersModell = LayersInstance
    }

    function toggleBookmark() {
        var i = isInModel("Bookmark");
        if (i !== -1) {
            model.remove(i)
            if (model.count === 0) modelEmpty()
        } else {
            model.append({name: "Bookmark"})
        }

        toggleStackLayoutIndex(3)
        bookmark.model = BookmarkInstance
    }

    TabBar {
        id: tabBar

        contentWidth: rootItem.model.count ? parent.width - 40 / Style.monitorRatio : 0
        Layout.leftMargin: rootItem.model.count ? 18 / Style.monitorRatio : 0
        Material.accent: Style.foregroundColor

        background: Rectangle {
            color: Style.disableColor
            height: 2
            anchors.bottom: parent.bottom
        }

        Repeater {
            id: repeater

            model: rootItem.model

            TabButton {
                id: tb

                width: tabBar.width / rootItem.model.count

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

                onDoubleClicked: {
                    // coder here
                }
            }
        }
    }

    StackLayout {
        id: stackLayout
        Layout.fillHeight: true
        currentIndex: tabBar.currentIndex
        visible: false

        ToolboxView { id: toolbox }
        LocationManager { id: locationManager }
        LayersWidget { id: layers }
        BookmarkItem { id: bookmark }
    }
}
